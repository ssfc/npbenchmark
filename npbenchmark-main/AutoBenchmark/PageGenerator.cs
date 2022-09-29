using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;


namespace AutoBenchmark {
    public class PageGenerator {
        public static void gitSync() {
            Util.run("git", "pull origin data");
            Util.run("git", "commit -a -m a");
            Util.run("git", "push origin data");
        }

        public static void generateMarkdown(string problemName, Problem problem) {
            using (StreamWriter sw = File.CreateText(CommonCfg.rankMarkdownPath(problemName))) {
                sw.WriteLine($"# {problemName} Results");
                foreach (var dataset in problem.datasets) {
                    foreach (var instance in dataset.instances) {
                        sw.WriteLine($"## {instance.Key}");
                        sw.WriteLine("| Rank |    Author    |    Obj    |       Date       |   Duration (s)  |");
                        sw.WriteLine("| ---- | ------------ | --------- | ---------------- | --------------- |");
                        int count = 0;
                        foreach (var r in instance.Value.results) {
                            sw.WriteLine($"| {count} | {r.author} | {problem.restoreObj(r.obj)} | {r.date} | {r.duration} |");
                            ++count;
                        }
                        sw.WriteLine();
                    }
                }
            }
        }
        public static void generateMarkdowns(Rank rank) {
            using (StreamWriter sw = File.CreateText(CommonCfg.ReadMePath)) {
                sw.WriteLine("# NPBenchmark Results");
                sw.WriteLine("| Problem | Rank | Leaderboard | Best |");
                sw.WriteLine("| ------- | ---- | ----------- | ---- |");
                foreach (var p in rank.problems) {
                    string r = CommonCfg.rankMarkdownPath(p.Key);
                    string c = CommonCfg.rankCsvPath(p.Key);
                    string b = CommonCfg.bestCsvPath(p.Key);
                    sw.WriteLine($"| {p.Key} | [{r}]({r}) | [{c}]({c}) | [{b}]({b}) |");
                }
            }
        }

        public static void generateCsv(Leaderboard lb, Problem problem, string path) {
            lb.sort();

            string[,] table = new string[problem.instanceNum + BenchmarkCfg.LeaderboardHeaderRowNum, lb.records.Count + 1];

            int i = 0;
            table[i++, 0] = "Author";
            table[i++, 0] = "Date";
            table[i++, 0] = "Score";
            foreach (var dataset in problem.datasets) {
                foreach (var instance in dataset.instances) { table[i++, 0] = instance.Key; }
            }

            int s = 1;
            foreach (Records r in lb.records) {
                i = 0;
                table[i++, s] = r.author;
                table[i++, s] = r.date;
                table[i++, s] = $"{r.score}";
                foreach (double obj in r.objs) { table[i++, s] = $"{problem.restoreObj(obj)}"; }
                ++s;
            }

            Util.saveCsv(path, table, BenchmarkCfg.LeaderboardDelim);
        }
        public static void generateCsv(string problemName, Problem problem) {
            generateCsv(BenchmarkCfg.leaderboards[problemName], problem, CommonCfg.rankCsvPath(problemName));
        }

        public static void generateBestCsv(string problemName, Problem problem) {
            Leaderboard lb = BenchmarkCfg.leaderboards[problemName];
            Dictionary<string, Records> bestRecords = new Dictionary<string, Records>();
            foreach (var r in lb.records) {
                if (bestRecords.ContainsKey(r.author)) {
                    Records br = bestRecords[r.author];
                    for (int o = 0; o < r.objs.Length; ++o) {
                        if (Util.updateMin(ref br.objs[o], r.objs[o])) { Util.updateMax(ref br.date, r.date); }
                    }
                } else {
                    bestRecords.Add(r.author, new Records { author = r.author, date = r.date, objs = r.objs.ToArray() });
                }
            }

            generateCsv(new Leaderboard { records = bestRecords.Values.ToList() }, problem, CommonCfg.bestCsvPath(problemName));
        }

        public static void appendQueue(Submission s, CommonCfg.QueueState state) {
            Util.appendLine(CommonCfg.QueuePath, $"| {Util.friendlyDateTime()} | {s.problem} | {s.author} | {s.date} | {state} |");
        }

        public static void generateHtml(Rank rank) {
            using (StreamWriter sw = File.CreateText(CommonCfg.RankPagePath)) {
                sw.WriteLine("<!DOCTYPE html>");
                sw.WriteLine("<html>");
                sw.WriteLine("<head>");
                sw.WriteLine("<meta charset='utf-8' />");
                sw.WriteLine("<title>NPBenchmark Results</title>");
                sw.WriteLine("<link rel='stylesheet' href='base.css' />");
                sw.WriteLine("</head>");
                sw.WriteLine("<body>");
                sw.WriteLine($"<h1>NPBenchmark Results</h1>");
                sw.WriteLine("<ol>");
                foreach (var problem in rank.problems) {
                    sw.WriteLine($"<li id='{problem.Key}'><a href='#{problem.Key}'>{problem.Key}</a><ol>");
                    foreach (var dataset in problem.Value.datasets) {
                        foreach (var instance in dataset.instances) {
                            sw.WriteLine($"<li id='{problem.Key}-{instance.Key}'><a href='#{problem.Key}-{instance.Key}'>{instance.Key}</a><table>");
                            sw.WriteLine("<tr><th>Rank</th><th>Author</th><th>Obj</th><th>Date</th><th>Duration</th></tr>");
                            int count = 0;
                            foreach (var r in instance.Value.results) {
                                sw.WriteLine($"<tr><td>{count}</td><td id='auth'>{r.author}</td><td>{problem.Value.restoreObj(r.obj)}</td><td>{r.date}</td><td>{r.duration}</td></tr>");
                                ++count;
                            }
                            sw.WriteLine("</table></li>");
                        }
                    }
                    sw.WriteLine("</ol></li>");
                }
                sw.WriteLine("</ol>");
                sw.WriteLine("</body>");
                sw.WriteLine("</html>");
            }
        }

        public static void generateStatistics() {
            StringBuilder sb = new StringBuilder();
            foreach (var pn in Enum.GetNames(typeof(ProblemName))) {
                sb.Append(pn);
                foreach (var r in BenchmarkCfg.leaderboards[pn].records) { sb.Append(',').Append(Util.subStr(r.author, 0, '-')); }
                sb.AppendLine();
            }
            Util.writeText(CommonCfg.StatisticsPath, sb.ToString());
        }
    }
}
