using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.Serialization;


namespace AutoBenchmark {
    [DataContract]
    public class Rank {
        [DataMember] public Dictionary<string, Problem> problems = new Dictionary<string, Problem>();
    }

    [DataContract]
    public class Problem {
        public const double MaxObjValue = 1E9;

        public int instanceNum {
            get {
                int n = 0;
                foreach (Dataset d in datasets) { n += d.instances.Count; }
                return n;
            }
        }

        public double normalizeObj(double obj) { return minimize ? obj : -obj; }
        public double restoreObj(double obj) { return minimize ? obj : -obj; }

        [DataMember] public bool minimize = true; // for the maximizing problems, the objective value is turned to its inverse number (negative).
        [DataMember] public List<Dataset> datasets = new List<Dataset>(); // ordered by complexity. the benchmark may stop if the solver fails on easy datasets.
    }

    [DataContract]
    public class Dataset {
        [DataMember] public double minFeasibleRate = 0; // the benchmark may stop if the feasible rate on this dataset is below `minFeasibleRate`.
        [DataMember] public double minOptRate = 0; // the benchmark may stop if the optimality rate on this dataset is below `minOptRate`.
        [DataMember] public double maxTimeoutRate = 1; // the benchmark may stop if the timeout rate on this dataset is above `maxTimeoutRate`.
        [DataMember] public Dictionary<string, Instance> instances = new Dictionary<string, Instance>();
    }

    [DataContract]
    public class Instance {
        public bool matchRecord(double obj) { return (Math.Abs(obj) < Problem.MaxObjValue) && ((results.Count <= 0) || (obj <= results.Min.obj)); }
        public bool isNewRecord(double obj) { return (Math.Abs(obj) < Problem.MaxObjValue) && ((results.Count <= 0) || (obj < results.Min.obj)); }

        [DataMember] public int repeat = 10;
        [DataMember] public long secTimeout = 999;
        [DataMember] public SortedSet<Result> results = new SortedSet<Result>();
        public string[] data; // load from file when necessary.
        public char[] data1; // single-line data.
    }

    [DataContract]
    public class Result : IComparable<Result> {
        public int CompareTo(Result other) {
            int objDiff = obj.CompareTo(other.obj);
            if (objDiff != 0) { return objDiff; }
            double durationDiff = duration - other.duration;
            if (Math.Abs(durationDiff) > 1) { return (int)durationDiff; }
            return date.CompareTo(other.date);
        }

        [DataMember(Order = 1)] public double obj;
        [DataMember(Order = 2)] public double duration;

        [DataMember(Order = 11)] public string author;
        [DataMember(Order = 12)] public string date;
    }


    public class Records : IComparable<Records> {
        public int CompareTo(Records other) {
            int objDiff = score.CompareTo(other.score);
            if (objDiff != 0) { return objDiff; }
            return date.CompareTo(other.date);
        }

        public string author;
        public string date;
        public double score; // performance over all instances.
        public double[] objs;
    }

    // `Leaderboard[s][i]` is the objective value for submission `s`, instance `i`. 
    public class Leaderboard {
        public List<Records> records = new List<Records>();

        public void sort() {
            foreach (Records r in records) {
                for (int i = 0; i < r.objs.Length; ++i) { Util.updateMin(ref records[0].objs[i], r.objs[i]); }
            }
            foreach (Records r in records) {
                r.score = 0;
                for (int i = 0; i < r.objs.Length; ++i) {
                    r.score += ((r.objs[i] - records[0].objs[i]) / Math.Abs(records[0].objs[i]));
                }
            }
            records.Sort();
        }

        public bool load(string problemName, Problem problem) {
            string[][] table = Util.loadCsv(CommonCfg.rankCsvPath(problemName), BenchmarkCfg.LeaderboardDelim);
            if (table == null) { return false; }

            int instanceNum = table.Length - BenchmarkCfg.LeaderboardHeaderRowNum;
            int submissionNum = table[0].Length;
            for (int s = 1; s < submissionNum; ++s) {
                int i = 0;
                Records r = new Records();
                r.author = table[i++][s];
                r.date = table[i++][s];
                r.score = double.Parse(table[i++][s]);
                r.objs = new double[instanceNum];
                for (int j = 0; j < instanceNum; ++j) { r.objs[j] = problem.normalizeObj(double.Parse(table[i++][s])); }
                records.Add(r);
            }
            return true;
        }

        public static void loadFromLog(string problemName, string year) {
            Problem p = BenchmarkCfg.rank.problems[problemName];
            string[] lines = Util.readLines(CommonCfg.logPath(problemName, year));
            Dictionary<string, Dictionary<string, double>> best = new Dictionary<string, Dictionary<string, double>>(); // `best[author][instance]`.
            for (int l = 1; l < lines.Length; ++l) {
                string[] words = lines[l].Split(BenchmarkCfg.LogDelim);
                string instance = words[2];
                string author = words[0];
                double obj = double.Parse(words[3]);
                best.TryAdd(author, new Dictionary<string, double>());
                if (best[author].ContainsKey(instance)) {
                    if ((obj < best[author][instance]) == p.minimize) { best[author][instance] = obj; }
                } else {
                    best[author].Add(instance, obj);
                }
            }

            foreach (var r in best) {
                double[] objs = new double[p.instanceNum];
                int i = 0;
                foreach (var dataset in p.datasets) {
                    foreach (var instance in dataset.instances) {
                        objs[i++] = r.Value.ContainsKey(instance.Key) ? r.Value[instance.Key] : Problem.MaxObjValue;
                    }
                }
                BenchmarkCfg.leaderboards[problemName].records.Add(new Records {
                    author = r.Key.toSafeCsvStr(), date = $"{year}",
                    score = 0, objs = objs
                });
            }
        }
        public static void loadFromLog(Rank rank) {
            foreach (var p in rank.problems) {
                if (BenchmarkCfg.leaderboards.TryAdd(p.Key, new Leaderboard())) {
                    BenchmarkCfg.leaderboards[p.Key].records.Add(new Records {
                        author = "Best", date = "0", score = 0,
                        objs = Enumerable.Repeat(Problem.MaxObjValue, p.Value.instanceNum).ToArray()
                    });
                }

                for (int y = 2020; y <= 2022; ++y) { loadFromLog(p.Key, $"{y}"); }
                PageGenerator.generateCsv(p.Key, p.Value);
            }
        }
    }


    public class Submission {
        public string problem;
        public string author;
        public string date;
        public string email;
        public string exePath;
    }

    public class Statistic {
        public double obj;
        public double duration;
        public int seed;
        public string info;
    }
}
