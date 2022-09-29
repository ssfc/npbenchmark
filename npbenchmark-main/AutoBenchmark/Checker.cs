using System;
using System.Collections.Generic;
using System.Linq;

using RF = AutoBenchmark.Util.RectilinearForward;


namespace AutoBenchmark {
    public delegate void Check(string[] input, string output, Statistic statistic);


    public class Checker {
        public static readonly char[] InlineDelimiters = new char[] { ' ', '\t' };
        public static readonly char[] LineDelimiters = new char[] { '\r', '\n' };
        public static readonly char[] WhiteSpaceChars = new char[] { ' ', '\t', '\r', '\n' };


        struct Edge {
            public int src;
            public int dst;
        };
        public static void coloring(string[] input, string output, Statistic statistic) {
            int nodeNum = 0;
            List<Edge> edges = new List<Edge>();
            try { // load instance.
                string[] words = input[0].Split(InlineDelimiters, StringSplitOptions.RemoveEmptyEntries);
                nodeNum = int.Parse(words[0]);
                for (int l = 1; l < input.Length; ++l) {
                    words = input[l].Split(InlineDelimiters, StringSplitOptions.RemoveEmptyEntries);
                    if (words.Length < 2) { continue; }
                    edges.Add(new Edge { src = int.Parse(words[0]), dst = int.Parse(words[1]) });
                }
            } catch (Exception e) { Util.log("[error] checker load input fail due to " + e.ToString()); }

            HashSet<string> colors = new HashSet<string>();
            List<string> nodeColors = new List<string>();
            try { // load solution.
                string[] words = output.Split(WhiteSpaceChars, StringSplitOptions.RemoveEmptyEntries);
                for (int c = 0; c < words.Length; ++c) {
                    nodeColors.Add(words[c]);
                    colors.Add(words[c]);
                }
            } catch (Exception e) { Util.log("[error] checker load output fail due to " + e.ToString()); }

            int conflictNum = 0;
            try { // check.
                foreach (Edge edge in edges) {
                    if (nodeColors[edge.src] == nodeColors[edge.dst]) { ++conflictNum; }
                }
            } catch (Exception e) { Util.log("[error] checker check fail due to " + e.ToString()); }

            bool feasible = (conflictNum == 0) && (nodeColors.Count == nodeNum);
            statistic.obj = feasible ? colors.Count : Problem.MaxObjValue;
            statistic.info = conflictNum.ToString();
        }


        public static void pCenter(string[] input, string output, Statistic statistic) {
            int nodeNum = 0;
            int centerNum = 0;
            int maxRank = 0;
            int minRank = 0;
            List<List<int>> sets = new List<List<int>>();
            List<List<int>> setsWithdrops = new List<List<int>>();
            try { // load instance.
                string[] words = input[0].Split(InlineDelimiters, StringSplitOptions.RemoveEmptyEntries);
                nodeNum = int.Parse(words[0]);
                centerNum = int.Parse(words[1]);
                int l = 1;
                for (sets.Capacity = nodeNum; (l < input.Length) && (sets.Count < nodeNum); ++l) {
                    int coveredItemNum = int.Parse(input[l]);
                    words = input[++l].Split(InlineDelimiters, StringSplitOptions.RemoveEmptyEntries);
                    List<int> set = new List<int>(coveredItemNum);
                    foreach (var word in words) { set.Add(int.Parse(word)); }
                    sets.Add(set);
                }

                words = input[l++].Split(InlineDelimiters, StringSplitOptions.RemoveEmptyEntries);
                maxRank = int.Parse(words[0]);
                minRank = int.Parse(words[1]);
                for (setsWithdrops.Capacity = maxRank - minRank; l < input.Length; ++l) {
                    words = input[l].Split(InlineDelimiters, StringSplitOptions.RemoveEmptyEntries);
                    List<int> setsWithdrop = new List<int>(words.Length - 1);
                    for (int c = 1; c < words.Length; ++c) { setsWithdrop.Add(int.Parse(words[c])); }
                    setsWithdrops.Add(setsWithdrop);
                }
            } catch (Exception e) { Util.log("[error] checker load input fail due to " + e.ToString()); }

            List<int> pickedSets = new List<int>(centerNum);
            List<bool> notPickedSet = new List<bool>(Enumerable.Repeat(true, nodeNum));
            try { // load solution.
                string[] words = output.Split(WhiteSpaceChars, StringSplitOptions.RemoveEmptyEntries);
                foreach (string word in words) {
                    int s = int.Parse(word);
                    if ((s < 0) || (s >= nodeNum)) { continue; }
                    pickedSets.Add(s);
                    notPickedSet[s] = false;
                }
            } catch (Exception e) { Util.log("[error] checker load output fail due to " + e.ToString()); }

            int uncoveredItemNum = nodeNum;
            int rank = maxRank + 1;
            try { // check.
                List<int> coveringSetNumOfItems = new List<int>(Enumerable.Repeat(0, nodeNum));
                foreach (var s in pickedSets) {
                    foreach (var item in sets[s]) {
                        if (coveringSetNumOfItems[item]++ < 1) { --uncoveredItemNum; }
                    }
                }

                if (uncoveredItemNum == 0) {
                    for (int r = 0; (r < setsWithdrops.Count) && (rank > maxRank); ++r) {
                        foreach (var s in setsWithdrops[r]) {
                            if (notPickedSet[s]) { continue; }
                            int dropItem = sets[s].Last();
                            if (--coveringSetNumOfItems[dropItem] < 1) { rank = maxRank - r; break; }
                            sets[s].pop();
                        }
                    }
                }
            } catch (Exception e) { Util.log("[error] checker check fail due to " + e.ToString()); }

            bool feasible = (uncoveredItemNum == 0) && (pickedSets.Count <= centerNum);
            statistic.obj = feasible ? rank : Problem.MaxObjValue;
            statistic.info = pickedSets.Count.ToString() + BenchmarkCfg.LogDelim + uncoveredItemNum.ToString();
        }


        // name mapping:
        // ---------------------------------------
        //  job shop scheduling | task scheduling
        // ---------------------+-----------------
        //  job                 | batch
        //  operation           | job
        //  machine             | worker
        // ---------------------+-----------------
        class Job {
            // `succeedingJobs[j]` is the jobs which can only begin after job `j` finishes.
            public List<int> succeedingJobs = new List<int>();
            // `candidateWorkers[w]` is the processing time of this job by worker `w`.
            public Dictionary<int, int> candidateWorkers = new Dictionary<int, int>();
        }
        public static void jobshop(string[] input, string output, Statistic statistic) {
            int batchNum = 0;
            int workerNum = 0;
            int maxCandidateWorkerNum = 0;
            List<Job> jobs = new List<Job>();
            List<List<int>> jobIdMap = new List<List<int>>(); // `jobIdMap[b][k]` is the `k`_th job (operation) in batch `b`.
            try { // load instance.
                string[] words = input[0].Split(InlineDelimiters, StringSplitOptions.RemoveEmptyEntries);
                batchNum = int.Parse(words[0]);
                workerNum = int.Parse(words[1]);
                maxCandidateWorkerNum = int.Parse(words[2]);
                for (int l = 1; l < input.Length; ++l) { // for each batch.
                    words = input[l].Split(InlineDelimiters, StringSplitOptions.RemoveEmptyEntries);
                    int opNum = int.Parse(words[0]);
                    List<int> idMap = new List<int>(opNum);
                    for (int c = 1; c < words.Length; ++c) { // for each job in the batch.
                        Job job = new Job();
                        int candidateWorkerNum = int.Parse(words[c]);
                        for (int w = 0; w < candidateWorkerNum; ++w) { // for each candidate worker.
                            int worker = int.Parse(words[++c]);
                            int duration = int.Parse(words[++c]);
                            job.candidateWorkers.Add(worker, duration);
                        }
                        idMap.Add(jobs.Count);
                        jobs.Add(job);
                        job.succeedingJobs.Add(jobs.Count); // succeeding job of the same batch.
                    }
                    jobs.Last().succeedingJobs.pop();
                    jobIdMap.Add(idMap);
                }
            } catch (Exception e) { Util.log("[error] checker load input fail due to " + e.ToString()); }

            List<List<int>> jobsOnWorkers = new List<List<int>>(workerNum);
            try { // load solution.
                string[] lines = output.Split(LineDelimiters, StringSplitOptions.RemoveEmptyEntries);
                for (int l = 0; l < lines.Length; ++l) {
                    string[] words = lines[l].Split(WhiteSpaceChars, StringSplitOptions.RemoveEmptyEntries);
                    int jobNumOnWorker = int.Parse(words[0]);
                    List<int> jobsOnWorker = new List<int>(jobNumOnWorker);
                    for (int c = 1; c < words.Length; ++c) {
                        int batch = int.Parse(words[c]);
                        int job = int.Parse(words[++c]);
                        jobsOnWorker.Add(jobIdMap[batch][job]);
                    }
                    jobsOnWorkers.Add(jobsOnWorker);
                }
            } catch (Exception e) { Util.log("[error] checker load output fail due to " + e.ToString()); }

            int makespan = (int)Problem.MaxObjValue;
            int restJobNum = jobs.Count;
            try { // check.
                List<int> jobExeDurations = new List<int>(Enumerable.Repeat(0, jobs.Count));
                for (int w = 0; w < workerNum; ++w) {
                    if (jobsOnWorkers[w].Count <= 0) { continue; }
                    int prevJob = jobsOnWorkers[w][0];
                    jobExeDurations[prevJob] = jobs[prevJob].candidateWorkers[w];
                    for (int j = 1; j < jobsOnWorkers[w].Count; ++j) {
                        int thisJob = jobsOnWorkers[w][j];
                        jobs[prevJob].succeedingJobs.Add(thisJob); // succeeding job on the same worker.
                        jobExeDurations[thisJob] = jobs[thisJob].candidateWorkers[w];
                        prevJob = thisJob;
                    }
                }
                List<int> preceedingJobNums = new List<int>(Enumerable.Repeat(0, jobs.Count));
                foreach (var job in jobs) {
                    foreach (var succeedingJob in job.succeedingJobs) {
                        ++preceedingJobNums[succeedingJob];
                    }
                }

                Queue<int> freeJobs = new Queue<int>(jobs.Count);
                List<int> earliestFinishTimes = new List<int>(Enumerable.Repeat(0, jobs.Count));
                for (int j = 0; j < jobs.Count; ++j) {
                    if (preceedingJobNums[j] > 0) { continue; }
                    freeJobs.Enqueue(j);
                    earliestFinishTimes[j] = jobExeDurations[j];
                }
                for (; freeJobs.Count > 0; --restJobNum) {
                    int j = freeJobs.Dequeue();
                    foreach (var succeedingJob in jobs[j].succeedingJobs) {
                        int newFinishTime = earliestFinishTimes[j] + jobExeDurations[succeedingJob];
                        if (earliestFinishTimes[succeedingJob] < newFinishTime) { earliestFinishTimes[succeedingJob] = newFinishTime; }
                        if (--preceedingJobNums[succeedingJob] <= 0) { freeJobs.Enqueue(succeedingJob); }
                    }
                }

                makespan = earliestFinishTimes.Max();
            } catch (Exception e) { Util.log("[error] checker check fail due to " + e.ToString()); }

            bool feasible = (restJobNum == 0);
            statistic.obj = feasible ? makespan : Problem.MaxObjValue;
            statistic.info = restJobNum.ToString();
        }


        public static void rwa(string[] input, string output, Statistic statistic) {
            Dictionary<string, Dictionary<string, HashSet<string>>> edges = new Dictionary<string, Dictionary<string, HashSet<string>>>();
            List<string[]> traffics = new List<string[]>();
            try { // load instance.
                string[] nums = input[0].Split(InlineDelimiters, StringSplitOptions.RemoveEmptyEntries);
                int nodeNum = int.Parse(nums[0]);
                int edgeNum = int.Parse(nums[1]);
                int trafficNum = int.Parse(nums[2]);
                int l = 1;
                for (int n = 0; (n < edgeNum) && (l < input.Length); ++l, ++n) {
                    string[] nodes = input[l].Split(InlineDelimiters, StringSplitOptions.RemoveEmptyEntries);
                    edges.TryAdd(nodes[0], new Dictionary<string, HashSet<string>>());
                    edges[nodes[0]].Add(nodes[1], new HashSet<string>());
                }

                traffics.Capacity = trafficNum;
                for (int t = 0; (t < trafficNum) && (l < input.Length); ++l, ++t) {
                    traffics.Add(input[l].Split(InlineDelimiters, StringSplitOptions.RemoveEmptyEntries));
                }
            } catch (Exception e) { Util.log("[error] checker load input fail due to " + e.ToString()); }

            int brokenPathNum = 0;
            int conflictNum = 0;
            HashSet<string> colors = new HashSet<string>();
            try { // load solution and check.
                string[] lines = output.Split(LineDelimiters, StringSplitOptions.RemoveEmptyEntries);
                brokenPathNum = traffics.Count - lines.Length;
                for (int l = 0; l < lines.Length; ++l) {
                    List<string> nums = lines[l].Split(InlineDelimiters, StringSplitOptions.RemoveEmptyEntries).ToList();
                    nums.Add(traffics[l][1]);
                    string color = nums[0];
                    colors.Add(color);
                    string src = traffics[l][0];
                    for (int i = 2; i < nums.Count; ++i) {
                        string dst = nums[i];
                        if (dst == src) { continue; }
                        if (!edges.ContainsKey(src) || !edges[src].ContainsKey(dst)) { ++brokenPathNum; break; }
                        if (edges[src][dst].Contains(color)) { ++conflictNum; }
                        edges[src][dst].Add(color);
                        src = nums[i];
                    }
                }
            } catch (Exception e) { Util.log("[error] checker check fail due to " + e.ToString()); }

            bool feasible = (brokenPathNum == 0) && (conflictNum == 0);
            statistic.obj = feasible ? colors.Count : Problem.MaxObjValue;
            statistic.info = brokenPathNum.ToString() + BenchmarkCfg.LogDelim + conflictNum.ToString();
        }


        public static void rectPacking(string[] input, string output, Statistic statistic) {
            int rectNum = 0;
            List<int[]> rects = new List<int[]>();
            try { // load instance.
                rectNum = int.Parse(input[0]);
                for (int l = 1; l < input.Length; ++l) {
                    string[] words = input[l].Split(InlineDelimiters, StringSplitOptions.RemoveEmptyEntries);
                    if (words.Length < 2) { continue; }
                    rects.Add(new int[2] { int.Parse(words[0]), int.Parse(words[1]) });
                }
            } catch (Exception e) { Util.log("[error] checker load input fail due to " + e.ToString()); }

            List<int[]> positions = new List<int[]>();
            try { // load solution.
                string[] lines = output.Split(LineDelimiters, StringSplitOptions.RemoveEmptyEntries);
                for (int l = 0; l < lines.Length; ++l) {
                    string[] words = lines[l].Split(InlineDelimiters, StringSplitOptions.RemoveEmptyEntries);
                    if (words.Length < 3) { continue; }
                    if (words[2] != "0") { Util.swap(ref rects[positions.Count][0], ref rects[positions.Count][1]); }
                    positions.Add(new int[2] { int.Parse(words[0]), int.Parse(words[1]) });
                }
            } catch (Exception e) { Util.log("[error] checker load output fail due to " + e.ToString()); }

            int restRectNum = rectNum - positions.Count;
            int conflictNum = 0;
            int xMin = int.MaxValue;
            int yMin = int.MaxValue;
            int xMax = int.MinValue;
            int yMax = int.MinValue;
            try { // check.
                for (int i = 0; i < rectNum; ++i) {
                    Util.updateMin(ref xMin, positions[i][0]);
                    Util.updateMin(ref yMin, positions[i][1]);
                    Util.updateMax(ref xMax, positions[i][0] + rects[i][0]);
                    Util.updateMax(ref yMax, positions[i][1] + rects[i][1]);
                    for (int j = 0; j < i; ++j) {
                        if (positions[i][0] >= positions[j][0] + rects[j][0]) { continue; }
                        if (positions[j][0] >= positions[i][0] + rects[i][0]) { continue; }
                        if (positions[i][1] >= positions[j][1] + rects[j][1]) { continue; }
                        if (positions[j][1] >= positions[i][1] + rects[i][1]) { continue; }
                        ++conflictNum; // OPT[szx][5]: line sweep + indexed tree, divide and conquer.
                    }
                }
            } catch (Exception e) { Util.log("[error] checker check fail due to " + e.ToString()); }

            bool feasible = (restRectNum == 0) && (conflictNum == 0);
            statistic.obj = feasible ? ((xMax - xMin) * (yMax - yMin)) : Problem.MaxObjValue;
            statistic.info = restRectNum.ToString() + BenchmarkCfg.LogDelim + conflictNum.ToString();
        }


        class Node2d {
            public const int Amp1 = 10;
            public const int Amp3 = 1000;

            public double[] coords;
            public int demand;
            public int minStayTime;
            public int timeWindowBegin;
            public int timeWindowEnd;

            public static int t(Node2d src, Node2d dst, int amp) {
                return (int)(Math.Sqrt(Util.powerSum(src.coords[0] - dst.coords[0], src.coords[1] - dst.coords[1])) * amp);
            }
        }
        public static void vrptw2d(string[] input, string output, Statistic statistic) {
            const int Amp = Node2d.Amp1;

            List<Node2d> nodes = new List<Node2d>();
            int nodeNum = 0;
            int maxVehicleNum = 0;
            int vehicleCapacity = 0;
            try { // load instance.
                string[] words = input[0].Split(InlineDelimiters, StringSplitOptions.RemoveEmptyEntries);
                nodeNum = int.Parse(words[0]);
                maxVehicleNum = int.Parse(words[1]);
                vehicleCapacity = int.Parse(words[2]);
                int l = 1;
                for (int n = 0; (n < nodeNum) && (l < input.Length); ++l, ++n) {
                    words = input[l].Split(InlineDelimiters, StringSplitOptions.RemoveEmptyEntries);
                    nodes.Add(new Node2d { coords = new double[2] { double.Parse(words[0]), double.Parse(words[1]) },
                        demand = int.Parse(words[2]), minStayTime = int.Parse(words[3]) * Amp,
                        timeWindowBegin = int.Parse(words[4]) * Amp, timeWindowEnd = int.Parse(words[5]) * Amp
                    });
                }
            } catch (Exception e) { Util.log("[error] checker load input fail due to " + e.ToString()); }

            int uncoverNum = nodeNum; // unvisited nodes.
            int conflictNum = 0; // revisited nodes.
            int vehicleNum = 0;
            int overload = 0;
            int delay = 0;
            int cost = 0;
            int[] visited = new int[nodeNum];
            try { // load solution and check.
                string[] lines = output.Split(LineDelimiters, StringSplitOptions.RemoveEmptyEntries);
                for (int l = 0; l < lines.Length; ++l) {
                    string[] words = lines[l].Split(InlineDelimiters, StringSplitOptions.RemoveEmptyEntries);
                    int capacity = vehicleCapacity;
                    int t = 0;
                    int src = 0;
                    for (int i = (words[0] == "0") ? 1 : 0; i < words.Length; ++i) {
                        int dst = int.Parse(words[i]);
                        if (dst == 0) { break; }
                        if (visited[dst]++ <= 0) {
                            --uncoverNum; // check uncover.
                            capacity -= nodes[dst].demand;
                        } else {
                            ++conflictNum; // check conflict.
                        }
                        int c = Node2d.t(nodes[src], nodes[dst], Amp);
                        t += c;
                        cost += c;
                        if (t < nodes[dst].timeWindowBegin) {
                            t = nodes[dst].timeWindowBegin;
                        } else if (t > nodes[dst].timeWindowEnd) { // check delay.
                            delay += (t - nodes[dst].timeWindowEnd);
                        }
                        t += nodes[dst].minStayTime;
                        src = dst;
                    }

                    int rc = Node2d.t(nodes[src], nodes[0], Amp);
                    t += rc;
                    cost += rc;
                    if (t > nodes[0].timeWindowEnd) { delay += (t - nodes[0].timeWindowEnd); } // check delay.

                    ++vehicleNum;
                    if (capacity < 0) { overload -= capacity; } // check overload.
                }
            } catch (Exception e) { Util.log("[error] checker load output or check fail due to " + e.ToString()); }

            bool feasible = (vehicleNum <= maxVehicleNum) && (uncoverNum == 0) && (conflictNum == 0) && (overload == 0) && (delay == 0);
            statistic.obj = feasible ? cost : Problem.MaxObjValue;
            statistic.info = vehicleNum.ToString() + BenchmarkCfg.LogDelim + uncoverNum.ToString() + BenchmarkCfg.LogDelim + conflictNum.ToString() + BenchmarkCfg.LogDelim + overload.ToString() + BenchmarkCfg.LogDelim + delay.ToString();
        }


        [Flags] enum NodeState : byte {
            Free = 0x0,
            Obstacle = 0x1,
            Included = 0x2,
        }
        public static void oarsmtEfficientRepresentation(string[] input, string output, Statistic statistic) {
            int nodeNum = 0;
            //int obstacleNum = 0;
            List<int[]> nodes = new List<int[]>();
            List<int[]> obstacles = new List<int[]>();
            Dictionary<int, int> xIndex = new Dictionary<int, int>();
            Dictionary<int, int> yIndex = new Dictionary<int, int>();
            try { // load instance.
                string[] words = input[0].Split(InlineDelimiters, StringSplitOptions.RemoveEmptyEntries);
                nodeNum = int.Parse(words[0]);
                //obstacleNum = int.Parse(words[1]);
                int l = 1;
                for (int n = 0; n < nodeNum; ++n, ++l) {
                    words = input[l].Split(InlineDelimiters, StringSplitOptions.RemoveEmptyEntries);
                    int[] o = new int[2] { int.Parse(words[0]), int.Parse(words[1]) };
                    nodes.Add(o);
                    Util.tryAdd(xIndex, o[0]);
                    Util.tryAdd(yIndex, o[1]);
                }
                for (; l < input.Length; ++l) {
                    words = input[l].Split(InlineDelimiters, StringSplitOptions.RemoveEmptyEntries);
                    int[] o = new int[4] { int.Parse(words[0]), int.Parse(words[1]), int.Parse(words[2]), int.Parse(words[3]) };
                    if (o[0] > o[2]) { Util.swap(ref o[0], ref o[2]); }
                    if (o[1] > o[3]) { Util.swap(ref o[1], ref o[3]); }
                    obstacles.Add(o); // the rectangles are forward, i.e., from (xMin, yMin) to (xMax, yMax).
                    Util.tryAdd(xIndex, o[0] - 1);
                    Util.tryAdd(yIndex, o[1] - 1);
                    Util.tryAdd(xIndex, o[2] + 1);
                    Util.tryAdd(yIndex, o[3] + 1);
                }
            } catch (Exception e) { Util.log("[error] checker load input fail due to " + e.ToString()); }

            List<int[][]> segments = new List<int[][]>();
            try { // load solution.
                string[] lines = output.Split(LineDelimiters, StringSplitOptions.RemoveEmptyEntries);
                foreach (var line in lines) {
                    string[] words = line.Split(InlineDelimiters, StringSplitOptions.RemoveEmptyEntries);
                    int[] pos = new int[2] { int.Parse(words[0]), int.Parse(words[1]) };
                    for (int i = 2; i < words.Length; i += 2) {
                        int d = int.Parse(words[i + 1]);
                        if (d == 0) { continue; }
                        int[] newPos = new int[2] { pos[0], pos[1] };
                        if (words[i] == "x") { newPos[0] += d; } else { newPos[1] += d; }
                        segments.Add(new int[2][] { ((d > 0) ? pos : newPos), ((d > 0) ? newPos : pos) });
                        pos = newPos; // the segments are forward, i.e., from (xMin, yMin) to (xMax, yMax).
                    }
                }
            } catch (Exception e) { Util.log("[error] checker load output fail due to " + e.ToString()); }

            int invasionNum = 0;
            int subgraphNum = 0;
            long wireLen = 0;
            try { // check.
                int[] xs = Util.mapBack(xIndex);
                int[] ys = Util.mapBack(yIndex);
                NodeState[,] grid = new NodeState[xs.Length, ys.Length];
                foreach (var o in obstacles) {
                    int xMax = Util.lowerBound(xs, o[2]);
                    if (xs[xMax] > o[2]) { --xMax; }
                    int yMax = Util.lowerBound(ys, o[3]);
                    if (ys[yMax] > o[3]) { --yMax; }
                    for (int xMin = Util.lowerBound(xs, o[0]); xMin <= xMax; ++xMin) {
                        for (int yMin = Util.lowerBound(ys, o[1]); yMin <= yMax; ++yMin) {
                            grid[xMin, xMax] = NodeState.Obstacle;
                        }
                    }
                }
                foreach (var s in segments) { // check obstacle invasion.
                    wireLen += (s[1][0] - s[0][0] + s[1][1] - s[0][1]); // for forward rectilinear segments only.
                    if (!xIndex.ContainsKey(s[0][0]) || !yIndex.ContainsKey(s[0][1])
                        || !xIndex.ContainsKey(s[1][0]) || !yIndex.ContainsKey(s[1][1])) {
                        ++invasionNum;
                        continue;
                    }
                    for (int x = xIndex[s[0][0]]; x <= xIndex[s[1][0]]; ++x) {
                        for (int y = yIndex[s[0][1]]; y <= yIndex[s[1][1]]; ++y) {
                            if (grid[x, y].HasFlag(NodeState.Obstacle)) { ++invasionNum; }
                            if (grid[x, y].HasFlag(NodeState.Included)) { continue; }
                            ++subgraphNum;
                            grid[x, y] |= NodeState.Included;
                        }
                    }
                }
                if (invasionNum == 0) { // check connectivity.
                    foreach (var n in nodes) { grid[xIndex[n[0]], yIndex[n[1]]] |= NodeState.Included; }

                    Queue<int[]> q = new Queue<int[]>();
                    Action<int, int> adj = (x, y) => {
                        if (!grid[x, y].HasFlag(NodeState.Included)) { return; }
                        grid[x, y] ^= NodeState.Included;
                        q.Enqueue(new int[] { x, y });
                        --subgraphNum;
                    };
                    q.Enqueue(new int[2] { xIndex[segments[0][0][0]], yIndex[segments[0][0][1]] });
                    grid[xIndex[segments[0][0][0]], yIndex[segments[0][0][1]]] ^= NodeState.Included;
                    while (q.Count > 0) {
                        int[] n = q.Dequeue();
                        adj(n[0], n[1] + 1);
                        adj(n[0], n[1] - 1);
                        adj(n[0] + 1, n[1]);
                        adj(n[0] - 1, n[1]);
                    }
                }
            } catch (Exception e) { Util.log("[error] checker check fail due to " + e.ToString()); }

            bool feasible = (invasionNum == 0) && (subgraphNum == 1);
            statistic.obj = feasible ? wireLen : Problem.MaxObjValue;
            statistic.info = invasionNum.ToString() + BenchmarkCfg.LogDelim + subgraphNum.ToString() + BenchmarkCfg.LogDelim;
        }
        public static void oarsmt(string[] input, string output, Statistic statistic) {
            int nodeNum = 0;
            //int obstacleNum = 0;
            List<int[]> nodes = new List<int[]>();
            List<int[]> obstacles = new List<int[]>();
            try { // load instance.
                string[] words = input[0].Split(InlineDelimiters, StringSplitOptions.RemoveEmptyEntries);
                nodeNum = int.Parse(words[0]);
                //obstacleNum = int.Parse(words[1]);
                int l = 1;
                for (int n = 0; n < nodeNum; ++n, ++l) {
                    words = input[l].Split(InlineDelimiters, StringSplitOptions.RemoveEmptyEntries);
                    nodes.Add(new int[2] { int.Parse(words[0]), int.Parse(words[1]) });
                }
                for (; l < input.Length; ++l) {
                    words = input[l].Split(InlineDelimiters, StringSplitOptions.RemoveEmptyEntries);
                    int[] o = new int[4] { int.Parse(words[0]), int.Parse(words[1]), int.Parse(words[2]), int.Parse(words[3]) };
                    if (o[0] > o[2]) { Util.swap(ref o[0], ref o[2]); }
                    if (o[1] > o[3]) { Util.swap(ref o[1], ref o[3]); }
                    obstacles.Add(o); // the rectangles are forward, i.e., from (xMin, yMin) to (xMax, yMax).
                }
            } catch (Exception e) { Util.log("[error] checker load input fail due to " + e.ToString()); }

            List<int[][]> segments = new List<int[][]>();
            Dictionary<int, List<int>> segmentsAtX = new Dictionary<int, List<int>>(); // `segmentsAtX[c]` is the list of segments which are parallel to line x = c.
            Dictionary<int, List<int>> segmentsAtY = new Dictionary<int, List<int>>(); // `segmentsAtY[c]` is the list of segments which are parallel to line y = c.
            try { // load solution.
                string[] lines = output.Split(LineDelimiters, StringSplitOptions.RemoveEmptyEntries);
                foreach (var line in lines) {
                    string[] words = line.Split(InlineDelimiters, StringSplitOptions.RemoveEmptyEntries);
                    int[] pos = new int[2] { int.Parse(words[0]), int.Parse(words[1]) };
                    for (int i = 2; i < words.Length; i += 2) {
                        int d = int.Parse(words[i + 1]);
                        if (d == 0) { continue; }
                        int[] newPos = new int[2] { pos[0], pos[1] };
                        if (words[i] == "x") {
                            newPos[0] += d;
                            Util.tryAdd(segmentsAtY, pos[1]).Add(segments.Count);
                        } else {
                            newPos[1] += d;
                            Util.tryAdd(segmentsAtX, pos[0]).Add(segments.Count);
                        } // the segments are forward, i.e., from (xMin, yMin) to (xMax, yMax).
                        segments.Add(new int[2][] { ((d > 0) ? pos : newPos), ((d > 0) ? newPos : pos) });
                        pos = newPos;
                    }
                }
            } catch (Exception e) { Util.log("[error] checker load output fail due to " + e.ToString()); }

            int invasionNum = 0;
            int subgraphNum = nodeNum;
            long wireLen = 0;
            try { // check.
                foreach (var s in segments) { // check obstacle invasion.
                    wireLen += (s[1][0] - s[0][0] + s[1][1] - s[0][1]); // for forward rectilinear segments only.
                    foreach (var o in obstacles) {
                        if (RF.segmentBoxInterfering(s, o)) { ++invasionNum; break; }
                    }
                }
                foreach (var n in nodes) { // check node inclusion.
                    bool included = false;
                    if (segmentsAtX.ContainsKey(n[0])) {
                        foreach (var s in segmentsAtX[n[0]]) {
                            if (RF.between(n[1], segments[s][0][1], segments[s][1][1])) { --subgraphNum; included = true; break; }
                        }
                    }
                    if (included || !segmentsAtY.ContainsKey(n[1])) { continue; }
                    foreach (var s in segmentsAtY[n[1]]) {
                        if (RF.between(n[0], segments[s][0][0], segments[s][1][0])) { --subgraphNum; break; }
                    }
                }
                if (subgraphNum == 0) { // check connectivity (if each node is covered by at least one segment).
                    int[] xs = Util.orderedKeys(segmentsAtX);
                    int[] ys = Util.orderedKeys(segmentsAtY);

                    Queue<int> q = new Queue<int>();
                    bool[] included = Enumerable.Repeat(false, segments.Count).ToArray();
                    Action<int, int> adj = (int s0, int s1) => {
                        if (included[s1] || (s0 == s1)) { return; }
                        if (!RF.segmentsInterfering(segments[s0], segments[s1])) { return; }
                        q.Enqueue(s1);
                        included[s1] = true;
                    };
                    for (int s = 0; s < segments.Count; ++s) { // BFS.
                        if (included[s]) { continue; }
                        if (++subgraphNum > 1) { break; }
                        q.Enqueue(s);
                        included[s] = true;
                        while (q.Count > 0) {
                            int s0 = q.Dequeue();
                            int xMin = Array.BinarySearch(xs, segments[s0][0][0]);
                            int xMax = Array.BinarySearch(xs, segments[s0][1][0]);
                            if (xMin > xMax) { Util.swap(ref xMin, ref xMax); }
                            if (xMax < xs.Length) {
                                for (; xMin <= xMax; ++xMin) {
                                    foreach (var s1 in segmentsAtX[xs[xMin]]) { adj(s0, s1); }
                                }
                            }
                            int yMin = Array.BinarySearch(ys, segments[s0][0][1]);
                            int yMax = Array.BinarySearch(ys, segments[s0][1][1]);
                            if (yMin > yMax) { Util.swap(ref yMin, ref yMax); }
                            if (yMax < ys.Length) {
                                for (; yMin <= yMax; ++yMin) {
                                    foreach (var s1 in segmentsAtY[ys[yMin]]) { adj(s0, s1); }
                                }
                            }
                        }
                    }
                }
            } catch (Exception e) { Util.log("[error] checker check fail due to " + e.ToString()); }

            bool feasible = (invasionNum == 0) && (subgraphNum == 1);
            statistic.obj = feasible ? wireLen : Problem.MaxObjValue;
            statistic.info = invasionNum.ToString() + BenchmarkCfg.LogDelim + subgraphNum.ToString() + BenchmarkCfg.LogDelim;
        }
        
        public static void dfvsp(string[] input, string output, Statistic statistic) {
            int nodeNum = 0;
            int arcNum = 0;
            List<List<int>> adjList = new List<List<int>>(); // `adjList[s][d]` is the `d`_th out-degree of node `s`.
            try { // load instance.
                string[] words = input[0].Split(InlineDelimiters, StringSplitOptions.RemoveEmptyEntries);
                nodeNum = int.Parse(words[0]);
                arcNum = int.Parse(words[1]);
                for (int l = 1; l < input.Length; ++l) { // for each node.
                    words = input[l].Split(InlineDelimiters, StringSplitOptions.RemoveEmptyEntries);
                    List<int> adjNodes = new List<int>();
                    foreach (var n in words) { adjNodes.Add(int.Parse(n)); }
                    adjList.Add(adjNodes);
                }
            } catch (Exception e) { Util.log("[error] checker load input fail due to " + e.ToString()); }

            List<int> removedNodes = new List<int>();
            bool[] nodeRemoved = Enumerable.Repeat(false, nodeNum).ToArray();
            try { // load solution.
                string[] words = output.Split(WhiteSpaceChars, StringSplitOptions.RemoveEmptyEntries);
                foreach (string word in words) {
                    int n = int.Parse(word);
                    if ((n < 0) || (n >= nodeNum) || nodeRemoved[n]) { continue; }
                    removedNodes.Add(n);
                    nodeRemoved[n] = true;
                }
            } catch (Exception e) { Util.log("[error] checker load output fail due to " + e.ToString()); }

            int unsortNodeNum = nodeNum - removedNodes.Count;
            try { // check.
                List<int> preceedingNodeNums = new List<int>(Enumerable.Repeat(0, nodeNum));
                for (int src = 0; src < nodeNum; ++src) {
                    if (nodeRemoved[src]) { continue; }
                    foreach (var dst in adjList[src]) {
                        if (!nodeRemoved[dst]) { ++preceedingNodeNums[dst]; }
                    }
                }

                Queue<int> freeNodes = new Queue<int>(nodeNum);
                for (int n = 0; n < nodeNum; ++n) {
                    if (!nodeRemoved[n] && (preceedingNodeNums[n] <= 0)) { freeNodes.Enqueue(n); }
                }
                for (; freeNodes.Count > 0; --unsortNodeNum) {
                    int src = freeNodes.Dequeue();
                    foreach (var dst in adjList[src]) {
                        if (nodeRemoved[dst]) { continue; }
                        if (--preceedingNodeNums[dst] <= 0) { freeNodes.Enqueue(dst); }
                    }
                }
            } catch (Exception e) { Util.log("[error] checker check fail due to " + e.ToString()); }

            bool feasible = (unsortNodeNum == 0);
            statistic.obj = feasible ? removedNodes.Count : Problem.MaxObjValue;
            statistic.info = unsortNodeNum.ToString();
        }


        public static void mcdsp(string[] input, string output, Statistic statistic) {
            int nodeNum = 0;
            List<int>[] adjList = null; // `adjList[s][d]` is the `d`_th out-degree of node `s`.
            try { // load instance.
                string[] words = input[0].Split(InlineDelimiters, StringSplitOptions.RemoveEmptyEntries);
                nodeNum = int.Parse(words[0]);
                adjList = new List<int>[nodeNum];
                for (int n = 0; n < nodeNum; ++n) { adjList[n] = new List<int>(); }
                for (int l = 1; l < input.Length; ++l) {
                    words = input[l].Split(InlineDelimiters, StringSplitOptions.RemoveEmptyEntries);
                    if (words.Length < 2) { continue; }
                    int src = int.Parse(words[0]);
                    int dst = int.Parse(words[1]);
                    adjList[src].Add(dst);
                    adjList[dst].Add(src);
                }
            } catch (Exception e) { Util.log("[error] checker load input fail due to " + e.ToString()); }

            List<int> pickedNodes = new List<int>();
            bool[] isNodePicked = Enumerable.Repeat(false, nodeNum).ToArray();
            try { // load solution.
                string[] words = output.Split(WhiteSpaceChars, StringSplitOptions.RemoveEmptyEntries);
                for (int c = 0; c < words.Length; ++c) {
                    int node = int.Parse(words[c]);
                    pickedNodes.Add(node);
                    isNodePicked[node] = true;
                }
            } catch (Exception e) { Util.log("[error] checker load output fail due to " + e.ToString()); }

            int uncoveredNodeNum = 0;
            int subgraphNum = 0;
            try { // check.
                bool[] isNodeCovered = Enumerable.Repeat(false, nodeNum).ToArray();
                foreach (var node in pickedNodes) {
                    isNodeCovered[node] = true;
                    foreach (var neighbor in adjList[node]) {
                        isNodeCovered[neighbor] = true;
                    }
                }
                foreach (var covered in isNodeCovered) {
                    if (!covered) { ++uncoveredNodeNum; }
                }

                Queue<int> q = new Queue<int>(pickedNodes.Count);
                bool[] included = Enumerable.Repeat(false, nodeNum).ToArray();
                foreach (var node in pickedNodes) { // BFS.
                    if (included[node]) { continue; }
                    if (++subgraphNum > 1) { break; }
                    q.Enqueue(node);
                    included[node] = true;
                    while (q.Count > 0) {
                        int src = q.Dequeue();
                        foreach (var dst in adjList[src]) {
                            if (!isNodePicked[dst] || included[dst]) { continue; }
                            q.Enqueue(dst);
                            included[dst] = true;
                        }
                    }
                }

            } catch (Exception e) { Util.log("[error] checker check fail due to " + e.ToString()); }

            bool feasible = (uncoveredNodeNum == 0) && (subgraphNum == 1);
            statistic.obj = feasible ? pickedNodes.Count : Problem.MaxObjValue;
            statistic.info = uncoveredNodeNum.ToString() + BenchmarkCfg.LogDelim + subgraphNum;
        }


        public static void peccp(string[] input, string output, Statistic statistic) {
            int circleNum = 0;
            double err = 0;
            try { // load instance.
                string[] words = input[0].Split(InlineDelimiters, StringSplitOptions.RemoveEmptyEntries);
                circleNum = int.Parse(words[0]);
                err = double.Parse(words[1]);
            } catch (Exception e) { Util.log("[error] checker load input fail due to " + e.ToString()); }

            List<double[]> positions = new List<double[]>();
            try { // load solution.
                string[] lines = output.Split(LineDelimiters, StringSplitOptions.RemoveEmptyEntries);
                for (int l = 0; l < lines.Length; ++l) {
                    string[] words = lines[l].Split(InlineDelimiters, StringSplitOptions.RemoveEmptyEntries);
                    positions.Add(new double[2] { double.Parse(words[0]), double.Parse(words[1]) });
                }
            } catch (Exception e) { Util.log("[error] checker load output fail due to " + e.ToString()); }

            int restCircleNum = circleNum - positions.Count;
            int conflictNum = 0;
            double ratio = Problem.MaxObjValue;
            try { // check.
                double r = 0; // max center distance between the container and each euqal circles (correlated to the radius of the container).
                double d = 4; // min center distance between each pair of equal circles (diameter of equal circles).
                for (int i = 0; i < circleNum; ++i) {
                    Util.updateMax(ref r, Util.powerSum(positions[i][0], positions[i][1]));
                    for (int j = 0; j < i; ++j) { // OPT[szx][5]: line sweep + indexed tree, divide and conquer.
                        Util.updateMin(ref d, Util.powerSum(positions[i][0] - positions[j][0], positions[i][1] - positions[j][1]));
                    }
                }
                r = Math.Sqrt(r);
                d = Math.Sqrt(d);
                //if (r < 1) { // the radius of the container is 1.
                //    if ((r + d / 2) > (1 + err)) { ++conflictNum; } // some circles exceed the container.
                //    ratio = 2 / d;
                //} else { // the radius of each circle is 1.
                //    if (d < (2 - err)) { ++conflictNum; } // some circles overlap.
                //    ratio = r + 1;
                //}
                ratio = (r + d / 2) * (2 / d); // scaling the circles instead of reporting infeasibility.
            } catch (Exception e) { Util.log("[error] checker check fail due to " + e.ToString()); }

            bool feasible = (restCircleNum == 0) && (conflictNum == 0);
            statistic.obj = feasible ? ratio : Problem.MaxObjValue;
            statistic.info = restCircleNum.ToString() + BenchmarkCfg.LogDelim + conflictNum.ToString();
        }


        public static void darp2d(string[] input, string output, Statistic statistic) {
            const int Amp = Node2d.Amp3;

            List<Node2d> nodes = new List<Node2d>();
            int requestNum = 0;
            int nodeNum = 0;
            int maxVehicleNum = 0;
            int vehicleCapacity = 0;
            int maxTravelTime = 0;
            int maxRideTime = 0;
            try { // load instance.
                string[] words = input[0].Split(InlineDelimiters, StringSplitOptions.RemoveEmptyEntries);
                requestNum = int.Parse(words[0]);
                nodeNum = 2 * requestNum + 1;
                maxVehicleNum = int.Parse(words[1]);
                vehicleCapacity = int.Parse(words[2]);
                maxTravelTime = int.Parse(words[3]) * Amp;
                maxRideTime = int.Parse(words[4]) * Amp;
                int l = 1;
                for (int n = 0; (n < nodeNum) && (l < input.Length); ++l, ++n) {
                    words = input[l].Split(InlineDelimiters, StringSplitOptions.RemoveEmptyEntries);
                    nodes.Add(new Node2d {
                        coords = new double[2] { double.Parse(words[0]), double.Parse(words[1]) },
                        demand = int.Parse(words[2]), minStayTime = int.Parse(words[3]) * Amp,
                        timeWindowBegin = int.Parse(words[4]) * Amp, timeWindowEnd = int.Parse(words[5]) * Amp
                    });
                }
            } catch (Exception e) { Util.log("[error] checker load input fail due to " + e.ToString()); }

            int uncoverNum = nodeNum; // unvisited nodes.
            int conflictNum = 0; // revisited nodes.
            int vehicleNum = 0;
            int overload = 0;
            int delay = 0;
            int cost = 0;
            int overRide = 0;
            int overTravel = 0;
            int disorderNum = 0;
            int[] visited = Enumerable.Repeat(-1, nodeNum).ToArray();
            try { // load solution and check.
                string[] lines = output.Split(LineDelimiters, StringSplitOptions.RemoveEmptyEntries);
                for (int l = 0; l < lines.Length; ++l) {
                    string[] words = lines[l].Split(InlineDelimiters, StringSplitOptions.RemoveEmptyEntries);
                    int capacity = vehicleCapacity;
                    List<int> path = new List<int>();
                    for (int i = (words[0] == "0") ? 1 : 0; i < words.Length; ++i) {
                        int dst = int.Parse(words[i]);
                        if (dst == 0) { break; }
                        path.Add(dst);
                    }
                    path.Add(0);

                    int src = 0;
                    for (int d = 0; d < path.Count; ++d) {
                        int dst = path[d];
                        if (visited[dst] < 0) {
                            visited[dst] = l;
                            --uncoverNum; // check uncover.
                            capacity -= nodes[dst].demand;
                            if (capacity < 0) { overload -= capacity; } // check overload.
                        } else if (dst != 0) {
                            ++conflictNum; // check conflict.
                        }
                        if (dst > requestNum) {
                            if (visited[dst - requestNum] != l) { ++disorderNum; } // check disorder.
                        }
                        cost += Node2d.t(nodes[src], nodes[dst], Amp);
                        src = dst;
                    }
                    ++vehicleNum;

                    int[] arrTimes = new int[nodeNum];
                    int t = nodes[0].timeWindowBegin;
                    Action<int, int, int, int> calcMinArrTimes = (t, src, d, stop) => {
                        arrTimes[src] = t;
                        t += nodes[src].minStayTime;
                        for (; d < path.Count; ++d) {
                            int dst = path[d];
                            t += Node2d.t(nodes[src], nodes[dst], Amp);
                            Util.updateMax(ref t, nodes[dst].timeWindowBegin);
                            arrTimes[dst] = t;
                            if (dst == stop) { break; }
                            t += nodes[dst].minStayTime;
                            src = dst;
                        }
                    };

                    calcMinArrTimes(nodes[0].timeWindowBegin, 0, 0, 0);
                    for (int p = path.Count - 2; p >= 0; --p) {
                        int pickup = path[p];
                        if (pickup > requestNum) { continue; }
                        int delivery = pickup + requestNum;
                        int minPickupTime = arrTimes[delivery] - maxRideTime - nodes[pickup].minStayTime;

                        // the min pickup time of the first cusomter should also respect the max travel time.
                        if (p == 0) { Util.updateMax(ref minPickupTime, arrTimes[0] - maxTravelTime + Node2d.t(nodes[0], nodes[pickup], Amp)); }

                        if (arrTimes[pickup] >= minPickupTime) { continue; }
                        calcMinArrTimes(minPickupTime, pickup, p + 1, delivery);
                        if (arrTimes[delivery] - arrTimes[pickup] - nodes[pickup].minStayTime <= maxRideTime) { continue; } // the delivery is also postponed.
                        overRide += (arrTimes[delivery] - arrTimes[pickup] - maxRideTime); // check override.
                    }
                    for (int d = 0; d < path.Count; ++d) {
                        int dst = path[d];
                        if (arrTimes[dst] > nodes[dst].timeWindowEnd) { delay += (arrTimes[dst] - nodes[dst].timeWindowEnd); }
                    }
                    
                    int travel = arrTimes[0] - arrTimes[path[0]] + Node2d.t(nodes[0], nodes[path[0]], Amp) - maxTravelTime;
                    if (travel > 0) { overTravel += travel; } // check travel time.
                }
            } catch (Exception e) { Util.log("[error] checker load output or check fail due to " + e.ToString()); }

            bool feasible = (vehicleNum <= maxVehicleNum) && (uncoverNum == 0) && (conflictNum == 0) && (overload == 0) && (delay == 0) && (overTravel == 0) && (overRide == 0) && (disorderNum == 0);
            statistic.obj = feasible ? cost : Problem.MaxObjValue;
            statistic.info = vehicleNum.ToString() + BenchmarkCfg.LogDelim + uncoverNum.ToString() + BenchmarkCfg.LogDelim + conflictNum.ToString() + BenchmarkCfg.LogDelim + overload.ToString() + BenchmarkCfg.LogDelim + delay.ToString();
        }

    }
}
