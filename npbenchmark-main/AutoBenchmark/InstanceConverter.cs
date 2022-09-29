using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;


namespace AutoBenchmark {
    public class InstanceConverter {
        public class GCP {
            public static void convertAll() {
                convertDimacs(@"DSJC125.1.col", @"DSJC0125.1.txt", 5);
                convertDimacs(@"DSJC125.5.col", @"DSJC0125.5.txt", 17);
                convertDimacs(@"DSJC125.9.col", @"DSJC0125.9.txt", 44);
                convertDimacs(@"DSJC250.1.col", @"DSJC0250.1.txt", 8);
                convertDimacs(@"DSJC250.9.col", @"DSJC0250.9.txt", 72);
                convertDimacs(@"DSJC250.5.col", @"DSJC0250.5.txt", 28);
                convertDimacs(@"DSJC500.1.col", @"DSJC0500.1.txt", 12);
                convertDimacs(@"DSJC500.5.col", @"DSJC0500.5.txt", 49);
                convertDimacs(@"DSJC500.9.col", @"DSJC0500.9.txt", 126);
                convertDimacs(@"DSJC1000.1.col", @"DSJC1000.1.txt", 20);
                convertDimacs(@"DSJC1000.5.col", @"DSJC1000.5.txt", 83);
                convertDimacs(@"DSJC1000.9.col", @"DSJC1000.9.txt", 224);
            }

            class Arc {
                public int src;
                public int dst;
            }
            static void convertDimacs(string oldPath, string newPath, int colorNum) {
                int nodeNum = 0;
                List<Arc> edges = new List<Arc>();
                string[] lines = File.ReadAllLines(oldPath);

                foreach (string line in lines) {
                    if (line.Length <= 0) { continue; }
                    if (line[0] == 'c') { continue; }

                    string[] words = line.Split(Checker.InlineDelimiters, StringSplitOptions.RemoveEmptyEntries);
                    if (line[0] == 'p') {
                        nodeNum = int.Parse(words[2]);
                        edges.Capacity = int.Parse(words[3]);
                    } else if (line[0] == 'e') {
                        edges.Add(new Arc { src = int.Parse(words[1]) - 1, dst = int.Parse(words[2]) - 1 });
                    }
                }

                StringBuilder sb = new StringBuilder();
                sb.Append(nodeNum).Append(' ').Append(edges.Count).Append(' ').Append(colorNum).Append('\n');
                foreach (var edge in edges) {
                    sb.Append(edge.src).Append(' ').Append(edge.dst).Append('\n');
                }

                File.WriteAllText(newPath, sb.ToString());
            }
        }

        public class PCP {
            public static void convertAll() {
                convertTsplib(@"pcb3038.tsp", @"pcb3038p010r729.txt", 10, 729);
                convertTsplib(@"pcb3038.tsp", @"pcb3038p020r494.txt", 20, 494);
                convertTsplib(@"pcb3038.tsp", @"pcb3038p030r394.txt", 30, 394);
                convertTsplib(@"pcb3038.tsp", @"pcb3038p040r337.txt", 40, 337);
                convertTsplib(@"pcb3038.tsp", @"pcb3038p050r299.txt", 50, 299);
                convertTsplib(@"pcb3038.tsp", @"pcb3038p100r207.txt", 100, 207);
                convertTsplib(@"pcb3038.tsp", @"pcb3038p150r165.txt", 150, 165);
                convertTsplib(@"pcb3038.tsp", @"pcb3038p200r141.txt", 200, 141);
                convertTsplib(@"pcb3038.tsp", @"pcb3038p250r123.txt", 250, 123);
                convertTsplib(@"pcb3038.tsp", @"pcb3038p300r116.txt", 300, 116);
                convertTsplib(@"pcb3038.tsp", @"pcb3038p350r105.txt", 350, 105);
                convertTsplib(@"pcb3038.tsp", @"pcb3038p400r97.txt", 400, 97);
                convertTsplib(@"pcb3038.tsp", @"pcb3038p450r89.txt", 450, 89);
                convertTsplib(@"pcb3038.tsp", @"pcb3038p500r85.txt", 500, 85);

                update(@"pmed01.n100p5.txt");
                update(@"pmed02.n100p10.txt");
                update(@"pmed03.n100p10.txt");
                update(@"pmed04.n100p20.txt");
                update(@"pmed05.n100p33.txt");
                update(@"pmed06.n200p5.txt");
                update(@"pmed07.n200p10.txt");
                update(@"pmed08.n200p20.txt");
                update(@"pmed09.n200p40.txt");
                update(@"pmed10.n200p67.txt");
                update(@"pmed11.n300p5.txt");
                update(@"pmed12.n300p10.txt");
                update(@"pmed13.n300p30.txt");
                update(@"pmed14.n300p60.txt");
                update(@"pmed15.n300p100.txt");
                update(@"pmed16.n400p5.txt");
                update(@"pmed17.n400p10.txt");
                update(@"pmed18.n400p40.txt");
                update(@"pmed19.n400p80.txt");
                update(@"pmed20.n400p133.txt");
                update(@"pmed21.n500p5.txt");
                update(@"pmed22.n500p10.txt");
                update(@"pmed23.n500p50.txt");
                update(@"pmed24.n500p100.txt");
                update(@"pmed25.n500p167.txt");
                update(@"pmed26.n600p5.txt");
                update(@"pmed27.n600p10.txt");
                update(@"pmed28.n600p60.txt");
                update(@"pmed29.n600p120.txt");
                update(@"pmed30.n600p200.txt");
                update(@"pmed31.n700p5.txt");
                update(@"pmed32.n700p10.txt");
                update(@"pmed33.n700p70.txt");
                update(@"pmed34.n700p140.txt");
                update(@"pmed35.n800p5.txt");
                update(@"pmed36.n800p10.txt");
                update(@"pmed37.n800p80.txt");
                update(@"pmed38.n900p5.txt");
                update(@"pmed39.n900p10.txt");
                update(@"pmed40.n900p90.txt");
                update(@"rl1323p010r3077.30.txt");
                update(@"rl1323p020r2016.40.txt");
                update(@"rl1323p030r1631.50.txt");
                update(@"rl1323p040r1352.36.txt");
                update(@"rl1323p050r1187.27.txt");
                update(@"rl1323p060r1063.01.txt");
                update(@"rl1323p070r971.93.txt");
                update(@"rl1323p080r895.06.txt");
                update(@"rl1323p090r832.00.txt");
                update(@"rl1323p100r789.70.txt");
                update(@"u1060p010r2273.08.txt");
                update(@"u1060p020r1580.80.txt");
                update(@"u1060p030r1207.77.txt");
                update(@"u1060p040r1020.56.txt");
                update(@"u1060p050r904.92.txt");
                update(@"u1060p060r781.17.txt");
                update(@"u1060p070r710.75.txt");
                update(@"u1060p080r652.16.txt");
                update(@"u1060p090r607.87.txt");
                update(@"u1060p100r570.01.txt");
                update(@"u1060p110r538.84.txt");
                update(@"u1060p120r510.27.txt");
                update(@"u1060p130r499.65.txt");
                update(@"u1060p140r452.46.txt");
                update(@"u1060p150r447.01.txt");
                update(@"u1817p010r457.91.txt");
                update(@"u1817p020r309.01.txt");
                update(@"u1817p030r240.99.txt");
                update(@"u1817p040r209.45.txt");
                update(@"u1817p050r184.91.txt");
                update(@"u1817p060r162.64.txt");
                update(@"u1817p070r148.11.txt");
                update(@"u1817p080r136.77.txt");
                update(@"u1817p090r129.51.txt");
                update(@"u1817p100r126.99.txt");
                update(@"u1817p110r109.25.txt");
                update(@"u1817p120r107.76.txt");
                update(@"u1817p130r104.73.txt");
                update(@"u1817p140r101.60.txt");
                update(@"u1817p150r91.60.txt");
            }

            static double[,] loadNodeCoords(string geoGraphPath) {
                string[] lines = File.ReadAllLines(geoGraphPath);

                int nodeNum = lines.Length;
                int l = 0;

                string[] header = lines[0].Split(Checker.InlineDelimiters, StringSplitOptions.RemoveEmptyEntries);
                if (header.Length == 1) {
                    nodeNum = int.Parse(header[0]);
                    l = 1;
                }

                double[,] coords = new double[nodeNum, 2];
                for (int n = 0; l < lines.Length; ++l, ++n) {
                    string[] words = lines[l].Split(Checker.InlineDelimiters, StringSplitOptions.RemoveEmptyEntries);
                    if (words.Length < 2) { continue; }
                    coords[n, 0] = double.Parse(words[words.Length - 2]);
                    coords[n, 1] = double.Parse(words[words.Length - 1]);
                }

                return coords;
            }

            static double[,] calcAdjMat(double[,] nodeCoords) {
                int nodeNum = nodeCoords.GetLength(0);
                double[,] adjMat = new double[nodeNum, nodeNum];
                for (int s = 0; s < nodeNum; ++s) {
                    for (int d = 0; d < s; ++d) {
                        double dx = nodeCoords[s, 0] - nodeCoords[d, 0];
                        double dy = nodeCoords[s, 1] - nodeCoords[d, 1];
                        adjMat[s, d] = adjMat[d, s] = Math.Sqrt(dx * dx + dy * dy);
                    }
                }
                return adjMat;
            }

            static double[] calcDistances(double[,] adjMat) {
                int nodeNum = adjMat.GetLength(0);
                SortedSet<double> distanceSet = new SortedSet<double>();
                for (int s = 0; s < nodeNum; ++s) {
                    for (int d = 0; d < s; ++d) {
                        distanceSet.Add(adjMat[s, d]);
                    }
                }
                return distanceSet.ToArray();
            }

            static void tryInc(SortedDictionary<int, int> dictionary, int key) {
                dictionary.TryAdd(key, 0);
                ++dictionary[key];
            }

            // there exist `centerNum` nodes that the number of covered nodes exceeds the `nodeNum`.
            static int estimateMinDistanceRankConservative(double[] distances, double[,] adjMat, List<List<int>> adjList, int centerNum) {
                int nodeNum = adjList.Count;
                int r = 0;
                int[] adjNodeNums = new int[nodeNum];
                for (int adjNodeNumSum = 0; adjNodeNumSum < nodeNum; ++r) {
                    double radius = distances[r];
                    for (int s = 0; s < nodeNum; ++s) {
                        for (int d = adjNodeNums[s]; (d < adjList[s].Count) && (adjMat[s, adjList[s][d]] <= radius); ++d) { ++adjNodeNums[s]; }
                    }

                    SortedDictionary<int, int> topAdjNodeNums = new SortedDictionary<int, int>();
                    for (int n = 0; n < centerNum; ++n) { tryInc(topAdjNodeNums, adjNodeNums[n]); }
                    int tail = topAdjNodeNums.First().Key;
                    for (int n = centerNum; n < nodeNum; ++n) {
                        if (adjNodeNums[n] <= tail) { continue; }
                        if (--topAdjNodeNums[tail] <= 0) { topAdjNodeNums.Remove(tail); }
                        tryInc(topAdjNodeNums, adjNodeNums[n]);
                        tail = topAdjNodeNums.First().Key;
                    }

                    adjNodeNumSum = 0;
                    foreach (var n in topAdjNodeNums) { adjNodeNumSum += (n.Key * n.Value); }
                }
                return r;
            }
            // the average number of nodes covered by each node exceeds `nodeNum / centerNum`.
            static int estimateMinDistanceRankAggressive(double[] distances, double[,] adjMat, List<List<int>> adjList, int centerNum) {
                int nodeNum = adjList.Count;
                int r = 0;
                int[] adjNodeNums = new int[nodeNum];
                int totalAdjNodeNum = nodeNum * nodeNum / centerNum; // average node number covered by a node times node number.
                for (int adjNodeNumSum = 0; adjNodeNumSum < totalAdjNodeNum; ++r) {
                    double radius = distances[r];
                    for (int s = 0; s < nodeNum; ++s) {
                        for (int d = adjNodeNums[s]; (d < nodeNum) && (adjMat[s, adjList[s][d]] <= radius); ++d) {
                            ++adjNodeNums[s];
                            ++adjNodeNumSum;
                        }
                    }
                }
                return r;
            }

            static void convertTsplib(string geoGraphPath, string uscpPath, int centerNum, double maxRadius) {
                double[,] nodeCoords = loadNodeCoords(geoGraphPath);
                int nodeNum = nodeCoords.GetLength(0);
                double[,] adjMat = calcAdjMat(nodeCoords);
                double[] distances = calcDistances(adjMat);

                List<List<int>> adjList = new List<List<int>>(nodeNum);
                for (int s = 0; s < nodeNum; ++s) {
                    List<int> adjNodes = new List<int>(nodeNum);
                    for (int d = 0; d < nodeNum; ++d) {
                        if ((adjMat[s, d] <= maxRadius) || (s == d)) { adjNodes.Add(d); }
                    }
                    adjNodes.Sort((int u, int v) => { return adjMat[s, u].CompareTo(adjMat[s, v]); });
                    adjList.Add(adjNodes);
                }

                int maxRank = distances.Length - 1;
                for (; (maxRank >= 0) && (distances[maxRank] > maxRadius); --maxRank) { }
                maxRadius = distances[maxRank];
                int minRank = estimateMinDistanceRankConservative(distances, adjMat, adjList, centerNum);

                StringBuilder sb = new StringBuilder();
                sb.Append(nodeNum).Append(' ').Append(centerNum).Append('\n');
                for (int s = 0; s < nodeNum; ++s) {
                    sb.Append(adjList[s].Count).Append('\n');
                    for (int d = 0; d < adjList[s].Count; ++d) {
                        sb.Append(adjList[s][d]).Append(' ');
                    }
                    sb.Remove(sb.Length - 1, 1);
                    sb.Append('\n');
                }

                sb.Append(maxRank).Append(' ').Append(minRank).Append('\n');
                for (int r = maxRank - 1; r >= minRank; --r) {
                    double radius = distances[r];
                    StringBuilder rsb = new StringBuilder();
                    int dropNodeNumSum = 0;
                    for (int s = 0; s < nodeNum; ++s) {
                        int dropNodeNum = 0;
                        for (int d = adjList[s].Count - 1; (d >= 0) && (adjMat[s, adjList[s][d]] > radius); --d) { ++dropNodeNum; }
                        if (dropNodeNum == 0) { continue; }
                        adjList[s].RemoveRange(adjList[s].Count - dropNodeNum, dropNodeNum);
                        dropNodeNumSum += dropNodeNum;
                        while (dropNodeNum-- > 0) { rsb.Append(s).Append(' '); }
                    }
                    rsb.Remove(rsb.Length - 1, 1);
                    sb.Append(dropNodeNumSum).Append('\t').AppendLine(rsb.ToString());
                }

                File.WriteAllText(uscpPath, sb.ToString());
            }

            static void update(string path) {
                string[] lines = File.ReadAllLines(path);

                StringBuilder sb = new StringBuilder();
                for (int l = 0; l < lines.Length; ++l) { sb.AppendLine(lines[l].TrimEnd()); }
                sb.AppendLine("0 0");

                File.WriteAllText(path, sb.ToString());
            }
        }

        public class FJSP {
            public static void convertAll() {
                convertJspTa("tai15_15.txt", 1);
                convertJspTa("tai20_15.txt", 11);
                convertJspTa("tai20_20.txt", 21);
                convertJspTa("tai30_15.txt", 31);
                convertJspTa("tai30_20.txt", 41);
                convertJspTa("tai50_15.txt", 51);
                convertJspTa("tai50_20.txt", 61);
                convertJspTa("tai100_20.txt", 71);
            }

            static void convertJspTa(string path, int index) {
                string[] lines = File.ReadAllLines(path);
                for (int l = 0; l < lines.Length; ++l) {
                    if (lines[l++].Length <= 0) { break; }
                    string[] words = lines[l++].Split(Checker.InlineDelimiters, StringSplitOptions.RemoveEmptyEntries);
                    if (words.Length <= 0) { break; }
                    int batchNum = int.Parse(words[0]);
                    int workerNum = int.Parse(words[1]);
                    int ub = int.Parse(words[4]);
                    int lb = int.Parse(words[5]);
                    string[][] times = new string[batchNum][];
                    for (int b = 0; b < batchNum; ++b) {
                        times[b] = lines[++l].Split(Checker.InlineDelimiters, StringSplitOptions.RemoveEmptyEntries);
                    }
                    ++l;
                    string[][] workers = new string[batchNum][];
                    for (int b = 0; b < batchNum; ++b) {
                        workers[b] = lines[++l].Split(Checker.InlineDelimiters, StringSplitOptions.RemoveEmptyEntries);
                    }

                    StringBuilder sb = new StringBuilder();
                    sb.Append(batchNum).Append(' ').Append(workerNum).AppendLine(" 1");
                    for (int b = 0; b < batchNum; ++b) {
                        sb.Append(workerNum);
                        for (int w = 0; w < workerNum; ++w) {
                            sb.Append("    1  ").Append(int.Parse(workers[b][w]) - 1)
                                .Append(' ').Append(times[b][w]);
                        }
                        sb.Append('\n');
                    }

                    path = "jsp.TA" + index++ + ".m" + workerNum + "j" + batchNum + "c1";
                    Console.WriteLine(path + " " + ub + " " + lb);
                    File.WriteAllText(path + ".txt", sb.ToString());
                }
            }
        }

        public class RWA {
            public static void convertAll() {
                convertXyz("ATT.net", "ATT.trf", "ATT");
                convertXyz("ATT2.net", "ATT2.trf", "ATT2");
                convertXyz("brasil.net", "brasil.trf", "brasil");
                convertXyz("EON.net", "EON.trf", "EON");
                convertXyz("Finland.net", "Finland.trf", "Finland");
                convertXyz("NSF.net", "NSF.1.trf", "NSF-01");
                convertXyz("NSF.net", "NSF.3.trf", "NSF-03");
                convertXyz("NSF.net", "NSF.12.trf", "NSF-12");
                convertXyz("NSF.net", "NSF.48.trf", "NSF-48");
                convertXyz("NSF2.net", "NSF.1.trf", "NSF2-01");
                convertXyz("NSF2.net", "NSF.3.trf", "NSF2-03");
                convertXyz("NSF2.net", "NSF.12.trf", "NSF2-12");
                convertXyz("NSF2.net", "NSF.48.trf", "NSF2-48");

                convertXyz("Y.3_seed=1.net", "Y.3.20_seed=1.trf", "Y3-020-1");
                convertXyz("Y.3_seed=2.net", "Y.3.20_seed=2.trf", "Y3-020-2");
                convertXyz("Y.3_seed=3.net", "Y.3.20_seed=3.trf", "Y3-020-3");
                convertXyz("Y.3_seed=4.net", "Y.3.20_seed=4.trf", "Y3-020-4");
                convertXyz("Y.3_seed=5.net", "Y.3.20_seed=5.trf", "Y3-020-5");
                convertXyz("Y.3_seed=1.net", "Y.3.40_seed=1.trf", "Y3-040-1");
                convertXyz("Y.3_seed=2.net", "Y.3.40_seed=2.trf", "Y3-040-2");
                convertXyz("Y.3_seed=3.net", "Y.3.40_seed=3.trf", "Y3-040-3");
                convertXyz("Y.3_seed=4.net", "Y.3.40_seed=4.trf", "Y3-040-4");
                convertXyz("Y.3_seed=5.net", "Y.3.40_seed=5.trf", "Y3-040-5");
                convertXyz("Y.3_seed=1.net", "Y.3.60_seed=1.trf", "Y3-060-1");
                convertXyz("Y.3_seed=2.net", "Y.3.60_seed=2.trf", "Y3-060-2");
                convertXyz("Y.3_seed=3.net", "Y.3.60_seed=3.trf", "Y3-060-3");
                convertXyz("Y.3_seed=4.net", "Y.3.60_seed=4.trf", "Y3-060-4");
                convertXyz("Y.3_seed=5.net", "Y.3.60_seed=5.trf", "Y3-060-5");
                convertXyz("Y.3_seed=1.net", "Y.3.80_seed=1.trf", "Y3-080-1");
                convertXyz("Y.3_seed=2.net", "Y.3.80_seed=2.trf", "Y3-080-2");
                convertXyz("Y.3_seed=3.net", "Y.3.80_seed=3.trf", "Y3-080-3");
                convertXyz("Y.3_seed=4.net", "Y.3.80_seed=4.trf", "Y3-080-4");
                convertXyz("Y.3_seed=5.net", "Y.3.80_seed=5.trf", "Y3-080-5");
                convertXyz("Y.3_seed=1.net", "Y.3.100_seed=1.trf", "Y3-100-1");
                convertXyz("Y.3_seed=2.net", "Y.3.100_seed=2.trf", "Y3-100-2");
                convertXyz("Y.3_seed=3.net", "Y.3.100_seed=3.trf", "Y3-100-3");
                convertXyz("Y.3_seed=4.net", "Y.3.100_seed=4.trf", "Y3-100-4");
                convertXyz("Y.3_seed=5.net", "Y.3.100_seed=5.trf", "Y3-100-5");
                convertXyz("Y.4_seed=1.net", "Y.4.20_seed=1.trf", "Y4-020-1");
                convertXyz("Y.4_seed=2.net", "Y.4.20_seed=2.trf", "Y4-020-2");
                convertXyz("Y.4_seed=3.net", "Y.4.20_seed=3.trf", "Y4-020-3");
                convertXyz("Y.4_seed=4.net", "Y.4.20_seed=4.trf", "Y4-020-4");
                convertXyz("Y.4_seed=5.net", "Y.4.20_seed=5.trf", "Y4-020-5");
                convertXyz("Y.4_seed=1.net", "Y.4.40_seed=1.trf", "Y4-040-1");
                convertXyz("Y.4_seed=2.net", "Y.4.40_seed=2.trf", "Y4-040-2");
                convertXyz("Y.4_seed=3.net", "Y.4.40_seed=3.trf", "Y4-040-3");
                convertXyz("Y.4_seed=4.net", "Y.4.40_seed=4.trf", "Y4-040-4");
                convertXyz("Y.4_seed=5.net", "Y.4.40_seed=5.trf", "Y4-040-5");
                convertXyz("Y.4_seed=1.net", "Y.4.60_seed=1.trf", "Y4-060-1");
                convertXyz("Y.4_seed=2.net", "Y.4.60_seed=2.trf", "Y4-060-2");
                convertXyz("Y.4_seed=3.net", "Y.4.60_seed=3.trf", "Y4-060-3");
                convertXyz("Y.4_seed=4.net", "Y.4.60_seed=4.trf", "Y4-060-4");
                convertXyz("Y.4_seed=5.net", "Y.4.60_seed=5.trf", "Y4-060-5");
                convertXyz("Y.4_seed=1.net", "Y.4.80_seed=1.trf", "Y4-080-1");
                convertXyz("Y.4_seed=2.net", "Y.4.80_seed=2.trf", "Y4-080-2");
                convertXyz("Y.4_seed=3.net", "Y.4.80_seed=3.trf", "Y4-080-3");
                convertXyz("Y.4_seed=4.net", "Y.4.80_seed=4.trf", "Y4-080-4");
                convertXyz("Y.4_seed=5.net", "Y.4.80_seed=5.trf", "Y4-080-5");
                convertXyz("Y.4_seed=1.net", "Y.4.100_seed=1.trf", "Y4-100-1");
                convertXyz("Y.4_seed=2.net", "Y.4.100_seed=2.trf", "Y4-100-2");
                convertXyz("Y.4_seed=3.net", "Y.4.100_seed=3.trf", "Y4-100-3");
                convertXyz("Y.4_seed=4.net", "Y.4.100_seed=4.trf", "Y4-100-4");
                convertXyz("Y.4_seed=5.net", "Y.4.100_seed=5.trf", "Y4-100-5");
                convertXyz("Y.5_seed=1.net", "Y.5.20_seed=1.trf", "Y5-020-1");
                convertXyz("Y.5_seed=2.net", "Y.5.20_seed=2.trf", "Y5-020-2");
                convertXyz("Y.5_seed=3.net", "Y.5.20_seed=3.trf", "Y5-020-3");
                convertXyz("Y.5_seed=4.net", "Y.5.20_seed=4.trf", "Y5-020-4");
                convertXyz("Y.5_seed=5.net", "Y.5.20_seed=5.trf", "Y5-020-5");
                convertXyz("Y.5_seed=1.net", "Y.5.40_seed=1.trf", "Y5-040-1");
                convertXyz("Y.5_seed=2.net", "Y.5.40_seed=2.trf", "Y5-040-2");
                convertXyz("Y.5_seed=3.net", "Y.5.40_seed=3.trf", "Y5-040-3");
                convertXyz("Y.5_seed=4.net", "Y.5.40_seed=4.trf", "Y5-040-4");
                convertXyz("Y.5_seed=5.net", "Y.5.40_seed=5.trf", "Y5-040-5");
                convertXyz("Y.5_seed=1.net", "Y.5.60_seed=1.trf", "Y5-060-1");
                convertXyz("Y.5_seed=2.net", "Y.5.60_seed=2.trf", "Y5-060-2");
                convertXyz("Y.5_seed=3.net", "Y.5.60_seed=3.trf", "Y5-060-3");
                convertXyz("Y.5_seed=4.net", "Y.5.60_seed=4.trf", "Y5-060-4");
                convertXyz("Y.5_seed=5.net", "Y.5.60_seed=5.trf", "Y5-060-5");
                convertXyz("Y.5_seed=1.net", "Y.5.80_seed=1.trf", "Y5-080-1");
                convertXyz("Y.5_seed=2.net", "Y.5.80_seed=2.trf", "Y5-080-2");
                convertXyz("Y.5_seed=3.net", "Y.5.80_seed=3.trf", "Y5-080-3");
                convertXyz("Y.5_seed=4.net", "Y.5.80_seed=4.trf", "Y5-080-4");
                convertXyz("Y.5_seed=5.net", "Y.5.80_seed=5.trf", "Y5-080-5");
                convertXyz("Y.5_seed=1.net", "Y.5.100_seed=1.trf", "Y5-100-1");
                convertXyz("Y.5_seed=2.net", "Y.5.100_seed=2.trf", "Y5-100-2");
                convertXyz("Y.5_seed=3.net", "Y.5.100_seed=3.trf", "Y5-100-3");
                convertXyz("Y.5_seed=4.net", "Y.5.100_seed=4.trf", "Y5-100-4");
                convertXyz("Y.5_seed=5.net", "Y.5.100_seed=5.trf", "Y5-100-5");

                convertXyz("Z.4x25.net", "Z.4x25.20.trf", "Z4x25-020");
                convertXyz("Z.4x25.net", "Z.4x25.40.trf", "Z4x25-040");
                convertXyz("Z.4x25.net", "Z.4x25.60.trf", "Z4x25-060");
                convertXyz("Z.4x25.net", "Z.4x25.80.trf", "Z4x25-080");
                convertXyz("Z.4x25.net", "Z.4x25.100.trf", "Z4x25-100");
                convertXyz("Z.5x20.net", "Z.5x20.20.trf", "Z5x20-020");
                convertXyz("Z.5x20.net", "Z.5x20.40.trf", "Z5x20-040");
                convertXyz("Z.5x20.net", "Z.5x20.60.trf", "Z5x20-060");
                convertXyz("Z.5x20.net", "Z.5x20.80.trf", "Z5x20-080");
                convertXyz("Z.5x20.net", "Z.5x20.100.trf", "Z5x20-100");
                convertXyz("Z.6x17.net", "Z.6x17.20.trf", "Z6x17-020");
                convertXyz("Z.6x17.net", "Z.6x17.40.trf", "Z6x17-040");
                convertXyz("Z.6x17.net", "Z.6x17.60.trf", "Z6x17-060");
                convertXyz("Z.6x17.net", "Z.6x17.80.trf", "Z6x17-080");
                convertXyz("Z.6x17.net", "Z.6x17.100.trf", "Z6x17-100");
                convertXyz("Z.8x13.net", "Z.8x13.20.trf", "Z8x13-020");
                convertXyz("Z.8x13.net", "Z.8x13.40.trf", "Z8x13-040");
                convertXyz("Z.8x13.net", "Z.8x13.60.trf", "Z8x13-060");
                convertXyz("Z.8x13.net", "Z.8x13.80.trf", "Z8x13-080");
                convertXyz("Z.8x13.net", "Z.8x13.100.trf", "Z8x13-100");
                convertXyz("Z.10x10.net", "Z.10x10.20.trf", "Z10x10-020");
                convertXyz("Z.10x10.net", "Z.10x10.40.trf", "Z10x10-040");
                convertXyz("Z.10x10.net", "Z.10x10.60.trf", "Z10x10-060");
                convertXyz("Z.10x10.net", "Z.10x10.80.trf", "Z10x10-080");
                convertXyz("Z.10x10.net", "Z.10x10.100.trf", "Z10x10-100");
            }

            static void convertXyz(string netPath, string trafficPath, string instancePath) {
                string[] netLines = File.ReadAllLines(netPath);
                string[] netNums = netLines[0].Split(Checker.InlineDelimiters, StringSplitOptions.RemoveEmptyEntries);

                string[] trafficLines = File.ReadAllLines(trafficPath);

                StringBuilder sb = new StringBuilder();
                sb.Append(netLines[0]).Append(' ').AppendLine(trafficLines[0]);
                for (int l = 1; l < netLines.Length; ++l) { sb.AppendLine(netLines[l]); }
                for (int l = 1; l < trafficLines.Length; ++l) { sb.AppendLine(trafficLines[l]); }

                File.WriteAllText(instancePath + ".n" + netNums[0] + "e" + netNums[1] + "t" + trafficLines[0] + ".txt", sb.ToString());
            }
        }

        public class VRPTW2d {
            public static void convertAll() {
                convertDimacs("solomon.c101");
                convertDimacs("solomon.c102");
                convertDimacs("solomon.c103");
                convertDimacs("solomon.c104");
                convertDimacs("solomon.c105");
                convertDimacs("solomon.c106");
                convertDimacs("solomon.c107");
                convertDimacs("solomon.c108");
                convertDimacs("solomon.c109");
                convertDimacs("solomon.c201");
                convertDimacs("solomon.c202");
                convertDimacs("solomon.c203");
                convertDimacs("solomon.c204");
                convertDimacs("solomon.c205");
                convertDimacs("solomon.c206");
                convertDimacs("solomon.c207");
                convertDimacs("solomon.c208");
                convertDimacs("solomon.r101");
                convertDimacs("solomon.r102");
                convertDimacs("solomon.r103");
                convertDimacs("solomon.r104");
                convertDimacs("solomon.r105");
                convertDimacs("solomon.r106");
                convertDimacs("solomon.r107");
                convertDimacs("solomon.r108");
                convertDimacs("solomon.r109");
                convertDimacs("solomon.r110");
                convertDimacs("solomon.r111");
                convertDimacs("solomon.r112");
                convertDimacs("solomon.r201");
                convertDimacs("solomon.r202");
                convertDimacs("solomon.r203");
                convertDimacs("solomon.r204");
                convertDimacs("solomon.r205");
                convertDimacs("solomon.r206");
                convertDimacs("solomon.r207");
                convertDimacs("solomon.r208");
                convertDimacs("solomon.r209");
                convertDimacs("solomon.r210");
                convertDimacs("solomon.r211");
                convertDimacs("solomon.rc101");
                convertDimacs("solomon.rc102");
                convertDimacs("solomon.rc103");
                convertDimacs("solomon.rc104");
                convertDimacs("solomon.rc105");
                convertDimacs("solomon.rc106");
                convertDimacs("solomon.rc107");
                convertDimacs("solomon.rc108");
                convertDimacs("solomon.rc201");
                convertDimacs("solomon.rc202");
                convertDimacs("solomon.rc203");
                convertDimacs("solomon.rc204");
                convertDimacs("solomon.rc205");
                convertDimacs("solomon.rc206");
                convertDimacs("solomon.rc207");
                convertDimacs("solomon.rc208");
                convertDimacs("homberger.c10201");
                convertDimacs("homberger.c10202");
                convertDimacs("homberger.c10203");
                convertDimacs("homberger.c10204");
                convertDimacs("homberger.c10205");
                convertDimacs("homberger.c10206");
                convertDimacs("homberger.c10207");
                convertDimacs("homberger.c10208");
                convertDimacs("homberger.c10209");
                convertDimacs("homberger.c10210");
                convertDimacs("homberger.c10401");
                convertDimacs("homberger.c10402");
                convertDimacs("homberger.c10403");
                convertDimacs("homberger.c10404");
                convertDimacs("homberger.c10405");
                convertDimacs("homberger.c10406");
                convertDimacs("homberger.c10407");
                convertDimacs("homberger.c10408");
                convertDimacs("homberger.c10409");
                convertDimacs("homberger.c10410");
                convertDimacs("homberger.c10601");
                convertDimacs("homberger.c10602");
                convertDimacs("homberger.c10603");
                convertDimacs("homberger.c10604");
                convertDimacs("homberger.c10605");
                convertDimacs("homberger.c10606");
                convertDimacs("homberger.c10607");
                convertDimacs("homberger.c10608");
                convertDimacs("homberger.c10609");
                convertDimacs("homberger.c10610");
                convertDimacs("homberger.c10801");
                convertDimacs("homberger.c10802");
                convertDimacs("homberger.c10803");
                convertDimacs("homberger.c10804");
                convertDimacs("homberger.c10805");
                convertDimacs("homberger.c10806");
                convertDimacs("homberger.c10807");
                convertDimacs("homberger.c10808");
                convertDimacs("homberger.c10809");
                convertDimacs("homberger.c10810");
                convertDimacs("homberger.c11001");
                convertDimacs("homberger.c11002");
                convertDimacs("homberger.c11003");
                convertDimacs("homberger.c11004");
                convertDimacs("homberger.c11005");
                convertDimacs("homberger.c11006");
                convertDimacs("homberger.c11007");
                convertDimacs("homberger.c11008");
                convertDimacs("homberger.c11009");
                convertDimacs("homberger.c11010");
                convertDimacs("homberger.c20201");
                convertDimacs("homberger.c20202");
                convertDimacs("homberger.c20203");
                convertDimacs("homberger.c20204");
                convertDimacs("homberger.c20205");
                convertDimacs("homberger.c20206");
                convertDimacs("homberger.c20207");
                convertDimacs("homberger.c20208");
                convertDimacs("homberger.c20209");
                convertDimacs("homberger.c20210");
                convertDimacs("homberger.c20401");
                convertDimacs("homberger.c20402");
                convertDimacs("homberger.c20403");
                convertDimacs("homberger.c20404");
                convertDimacs("homberger.c20405");
                convertDimacs("homberger.c20406");
                convertDimacs("homberger.c20407");
                convertDimacs("homberger.c20408");
                convertDimacs("homberger.c20409");
                convertDimacs("homberger.c20410");
                convertDimacs("homberger.c20601");
                convertDimacs("homberger.c20602");
                convertDimacs("homberger.c20603");
                convertDimacs("homberger.c20604");
                convertDimacs("homberger.c20605");
                convertDimacs("homberger.c20606");
                convertDimacs("homberger.c20607");
                convertDimacs("homberger.c20608");
                convertDimacs("homberger.c20609");
                convertDimacs("homberger.c20610");
                convertDimacs("homberger.c20801");
                convertDimacs("homberger.c20802");
                convertDimacs("homberger.c20803");
                convertDimacs("homberger.c20804");
                convertDimacs("homberger.c20805");
                convertDimacs("homberger.c20806");
                convertDimacs("homberger.c20807");
                convertDimacs("homberger.c20808");
                convertDimacs("homberger.c20809");
                convertDimacs("homberger.c20810");
                convertDimacs("homberger.c21001");
                convertDimacs("homberger.c21002");
                convertDimacs("homberger.c21003");
                convertDimacs("homberger.c21004");
                convertDimacs("homberger.c21005");
                convertDimacs("homberger.c21006");
                convertDimacs("homberger.c21007");
                convertDimacs("homberger.c21008");
                convertDimacs("homberger.c21009");
                convertDimacs("homberger.c21010");
                convertDimacs("homberger.r10201");
                convertDimacs("homberger.r10202");
                convertDimacs("homberger.r10203");
                convertDimacs("homberger.r10204");
                convertDimacs("homberger.r10205");
                convertDimacs("homberger.r10206");
                convertDimacs("homberger.r10207");
                convertDimacs("homberger.r10208");
                convertDimacs("homberger.r10209");
                convertDimacs("homberger.r10210");
                convertDimacs("homberger.r10401");
                convertDimacs("homberger.r10402");
                convertDimacs("homberger.r10403");
                convertDimacs("homberger.r10404");
                convertDimacs("homberger.r10405");
                convertDimacs("homberger.r10406");
                convertDimacs("homberger.r10407");
                convertDimacs("homberger.r10408");
                convertDimacs("homberger.r10409");
                convertDimacs("homberger.r10410");
                convertDimacs("homberger.r10601");
                convertDimacs("homberger.r10602");
                convertDimacs("homberger.r10603");
                convertDimacs("homberger.r10604");
                convertDimacs("homberger.r10605");
                convertDimacs("homberger.r10606");
                convertDimacs("homberger.r10607");
                convertDimacs("homberger.r10608");
                convertDimacs("homberger.r10609");
                convertDimacs("homberger.r10610");
                convertDimacs("homberger.r10801");
                convertDimacs("homberger.r10802");
                convertDimacs("homberger.r10803");
                convertDimacs("homberger.r10804");
                convertDimacs("homberger.r10805");
                convertDimacs("homberger.r10806");
                convertDimacs("homberger.r10807");
                convertDimacs("homberger.r10808");
                convertDimacs("homberger.r10809");
                convertDimacs("homberger.r10810");
                convertDimacs("homberger.r11001");
                convertDimacs("homberger.r11002");
                convertDimacs("homberger.r11003");
                convertDimacs("homberger.r11004");
                convertDimacs("homberger.r11005");
                convertDimacs("homberger.r11006");
                convertDimacs("homberger.r11007");
                convertDimacs("homberger.r11008");
                convertDimacs("homberger.r11009");
                convertDimacs("homberger.r11010");
                convertDimacs("homberger.r20201");
                convertDimacs("homberger.r20202");
                convertDimacs("homberger.r20203");
                convertDimacs("homberger.r20204");
                convertDimacs("homberger.r20205");
                convertDimacs("homberger.r20206");
                convertDimacs("homberger.r20207");
                convertDimacs("homberger.r20208");
                convertDimacs("homberger.r20209");
                convertDimacs("homberger.r20210");
                convertDimacs("homberger.r20401");
                convertDimacs("homberger.r20402");
                convertDimacs("homberger.r20403");
                convertDimacs("homberger.r20404");
                convertDimacs("homberger.r20405");
                convertDimacs("homberger.r20406");
                convertDimacs("homberger.r20407");
                convertDimacs("homberger.r20408");
                convertDimacs("homberger.r20409");
                convertDimacs("homberger.r20410");
                convertDimacs("homberger.r20601");
                convertDimacs("homberger.r20602");
                convertDimacs("homberger.r20603");
                convertDimacs("homberger.r20604");
                convertDimacs("homberger.r20605");
                convertDimacs("homberger.r20606");
                convertDimacs("homberger.r20607");
                convertDimacs("homberger.r20608");
                convertDimacs("homberger.r20609");
                convertDimacs("homberger.r20610");
                convertDimacs("homberger.r20801");
                convertDimacs("homberger.r20802");
                convertDimacs("homberger.r20803");
                convertDimacs("homberger.r20804");
                convertDimacs("homberger.r20805");
                convertDimacs("homberger.r20806");
                convertDimacs("homberger.r20807");
                convertDimacs("homberger.r20808");
                convertDimacs("homberger.r20809");
                convertDimacs("homberger.r20810");
                convertDimacs("homberger.r21001");
                convertDimacs("homberger.r21002");
                convertDimacs("homberger.r21003");
                convertDimacs("homberger.r21004");
                convertDimacs("homberger.r21005");
                convertDimacs("homberger.r21006");
                convertDimacs("homberger.r21007");
                convertDimacs("homberger.r21008");
                convertDimacs("homberger.r21009");
                convertDimacs("homberger.r21010");
                convertDimacs("homberger.rc10201");
                convertDimacs("homberger.rc10202");
                convertDimacs("homberger.rc10203");
                convertDimacs("homberger.rc10204");
                convertDimacs("homberger.rc10205");
                convertDimacs("homberger.rc10206");
                convertDimacs("homberger.rc10207");
                convertDimacs("homberger.rc10208");
                convertDimacs("homberger.rc10209");
                convertDimacs("homberger.rc10210");
                convertDimacs("homberger.rc10401");
                convertDimacs("homberger.rc10402");
                convertDimacs("homberger.rc10403");
                convertDimacs("homberger.rc10404");
                convertDimacs("homberger.rc10405");
                convertDimacs("homberger.rc10406");
                convertDimacs("homberger.rc10407");
                convertDimacs("homberger.rc10408");
                convertDimacs("homberger.rc10409");
                convertDimacs("homberger.rc10410");
                convertDimacs("homberger.rc10601");
                convertDimacs("homberger.rc10602");
                convertDimacs("homberger.rc10603");
                convertDimacs("homberger.rc10604");
                convertDimacs("homberger.rc10605");
                convertDimacs("homberger.rc10606");
                convertDimacs("homberger.rc10607");
                convertDimacs("homberger.rc10608");
                convertDimacs("homberger.rc10609");
                convertDimacs("homberger.rc10610");
                convertDimacs("homberger.rc10801");
                convertDimacs("homberger.rc10802");
                convertDimacs("homberger.rc10803");
                convertDimacs("homberger.rc10804");
                convertDimacs("homberger.rc10805");
                convertDimacs("homberger.rc10806");
                convertDimacs("homberger.rc10807");
                convertDimacs("homberger.rc10808");
                convertDimacs("homberger.rc10809");
                convertDimacs("homberger.rc10810");
                convertDimacs("homberger.rc11001");
                convertDimacs("homberger.rc11002");
                convertDimacs("homberger.rc11003");
                convertDimacs("homberger.rc11004");
                convertDimacs("homberger.rc11005");
                convertDimacs("homberger.rc11006");
                convertDimacs("homberger.rc11007");
                convertDimacs("homberger.rc11008");
                convertDimacs("homberger.rc11009");
                convertDimacs("homberger.rc11010");
                convertDimacs("homberger.rc20201");
                convertDimacs("homberger.rc20202");
                convertDimacs("homberger.rc20203");
                convertDimacs("homberger.rc20204");
                convertDimacs("homberger.rc20205");
                convertDimacs("homberger.rc20206");
                convertDimacs("homberger.rc20207");
                convertDimacs("homberger.rc20208");
                convertDimacs("homberger.rc20209");
                convertDimacs("homberger.rc20210");
                convertDimacs("homberger.rc20401");
                convertDimacs("homberger.rc20402");
                convertDimacs("homberger.rc20403");
                convertDimacs("homberger.rc20404");
                convertDimacs("homberger.rc20405");
                convertDimacs("homberger.rc20406");
                convertDimacs("homberger.rc20407");
                convertDimacs("homberger.rc20408");
                convertDimacs("homberger.rc20409");
                convertDimacs("homberger.rc20410");
                convertDimacs("homberger.rc20601");
                convertDimacs("homberger.rc20602");
                convertDimacs("homberger.rc20603");
                convertDimacs("homberger.rc20604");
                convertDimacs("homberger.rc20605");
                convertDimacs("homberger.rc20606");
                convertDimacs("homberger.rc20607");
                convertDimacs("homberger.rc20608");
                convertDimacs("homberger.rc20609");
                convertDimacs("homberger.rc20610");
                convertDimacs("homberger.rc20801");
                convertDimacs("homberger.rc20802");
                convertDimacs("homberger.rc20803");
                convertDimacs("homberger.rc20804");
                convertDimacs("homberger.rc20805");
                convertDimacs("homberger.rc20806");
                convertDimacs("homberger.rc20807");
                convertDimacs("homberger.rc20808");
                convertDimacs("homberger.rc20809");
                convertDimacs("homberger.rc20810");
                convertDimacs("homberger.rc21001");
                convertDimacs("homberger.rc21002");
                convertDimacs("homberger.rc21003");
                convertDimacs("homberger.rc21004");
                convertDimacs("homberger.rc21005");
                convertDimacs("homberger.rc21006");
                convertDimacs("homberger.rc21007");
                convertDimacs("homberger.rc21008");
                convertDimacs("homberger.rc21009");
                convertDimacs("homberger.rc21010");
            }

            static void convertDimacs(string oldPath) {
                string[] lines = File.ReadAllLines(oldPath + ".txt");

                string[] words = lines[4].Split(Checker.InlineDelimiters, StringSplitOptions.RemoveEmptyEntries);
                string vehicleNum = words[0];
                string vehicleCapacity = words[1];

                int nodeNum = 0;
                StringBuilder sb = new StringBuilder();
                for (int l = 9; l < lines.Length; ++l) {
                    words = lines[l].Split(Checker.InlineDelimiters, StringSplitOptions.RemoveEmptyEntries);
                    if (words.Length < 7) { continue; }
                    ++nodeNum;
                    sb.Append(words[1]).Append(' ').Append(words[2]).Append(' ')
                        .Append(words[3]).Append(' ').Append(words[6]).Append(' ')
                        .Append(words[4]).Append(' ').AppendLine(words[5]);
                }

                StringBuilder header = new StringBuilder();
                header.Append(nodeNum).Append(' ').Append(vehicleNum).Append(' ').AppendLine(vehicleCapacity);
                StringBuilder fn = new StringBuilder();
                fn.Append(".n").Append(nodeNum).Append('v').Append(vehicleNum).Append('c').Append(vehicleCapacity).Append(".txt");
                File.WriteAllText(oldPath + fn.ToString(), header.ToString() + sb.ToString());
            }
        }

        public class OARSMT {
            public static void convertAll() {
                convertDimacs(@"Bonn_109_101");
                convertDimacs(@"Bonn_23292_54");
                convertDimacs(@"Bonn_35574_158");
                convertDimacs(@"Bonn_46269_127");
                convertDimacs(@"Bonn_108500_141");
                convertDimacs(@"Bonn_129399_210");
                convertDimacs(@"Bonn_639639_382");
                convertDimacs(@"Bonn_783352_175");
                convertDimacs(@"IND1");
                convertDimacs(@"IND2");
                convertDimacs(@"IND3");
                convertDimacs(@"IND4");
                convertDimacs(@"IND5");
                convertDimacs(@"RC01");
                convertDimacs(@"RC02");
                convertDimacs(@"RC03");
                convertDimacs(@"RC04");
                convertDimacs(@"RC05");
                convertDimacs(@"RC06");
                convertDimacs(@"RC07");
                convertDimacs(@"RC08");
                convertDimacs(@"RC09");
                convertDimacs(@"RC10");
                convertDimacs(@"RC11");
                convertDimacs(@"RC12");
                convertDimacs(@"RL01");
                convertDimacs(@"RL02");
                convertDimacs(@"RL03");
                convertDimacs(@"RL04");
                convertDimacs(@"RL05");
                convertDimacs(@"RT01");
                convertDimacs(@"RT02");
                convertDimacs(@"RT03");
                convertDimacs(@"RT04");
                convertDimacs(@"RT05");
            }

            static void convertDimacs(string oldPath) {
                string[] lines = File.ReadAllLines(oldPath + ".stp");
                string nodeNum = "";
                string obstacleNum = "";
                StringBuilder sb = new StringBuilder();
                for (int l = 0; l < lines.Length; ++l) {
                    string[] words = lines[l].Split(Checker.InlineDelimiters, StringSplitOptions.RemoveEmptyEntries);
                    if (words.Length < 2) { continue; }
                    if (words[0] == "DD") {
                        sb.Append(words[2]).Append(' ').AppendLine(words[3]);
                    } else if (words[0] == "RR") {
                        sb.Append(int.Parse(words[1]) + 1).Append(' ').Append(int.Parse(words[2]) + 1).Append(' ')
                            .Append(int.Parse(words[3]) - 1).Append(' ').Append(int.Parse(words[4]) - 1).Append('\n');
                    } else if (words[0] == "Nodes") {
                        nodeNum = words[1];
                    } else if (words[0] == "Obstacles") {
                        obstacleNum = words[1];
                    }
                }

                StringBuilder header = new StringBuilder();
                header.Append(nodeNum).Append(' ').AppendLine(obstacleNum);
                StringBuilder fn = new StringBuilder();
                fn.Append(".n").Append(nodeNum).Append('o').Append(obstacleNum).Append(".txt");
                File.WriteAllText(oldPath.subStr(0, '_').ToLower() + fn.ToString(), header.ToString() + sb.ToString());
            }
        }

        public class DFVSP {
            public static void convertAll() {
                convertPardalos(@"P50-100");
                convertPardalos(@"P50-150");
                convertPardalos(@"P50-200");
                convertPardalos(@"P50-250");
                convertPardalos(@"P50-300");
                convertPardalos(@"P50-500");
                convertPardalos(@"P50-600");
                convertPardalos(@"P50-700");
                convertPardalos(@"P50-800");
                convertPardalos(@"P50-900");
                convertPardalos(@"P100-200");
                convertPardalos(@"P100-300");
                convertPardalos(@"P100-400");
                convertPardalos(@"P100-500");
                convertPardalos(@"P100-600");
                convertPardalos(@"P100-1000");
                convertPardalos(@"P100-1100");
                convertPardalos(@"P100-1200");
                convertPardalos(@"P100-1300");
                convertPardalos(@"P100-1400");
                convertPardalos(@"P500-1000");
                convertPardalos(@"P500-1500");
                convertPardalos(@"P500-2000");
                convertPardalos(@"P500-2500");
                convertPardalos(@"P500-3000");
                convertPardalos(@"P500-5000");
                convertPardalos(@"P500-5500");
                convertPardalos(@"P500-6000");
                convertPardalos(@"P500-6500");
                convertPardalos(@"P500-7000");
                convertPardalos(@"P1000-3000");
                convertPardalos(@"P1000-3500");
                convertPardalos(@"P1000-4000");
                convertPardalos(@"P1000-4500");
                convertPardalos(@"P1000-5000");
                convertPardalos(@"P1000-10000");
                convertPardalos(@"P1000-15000");
                convertPardalos(@"P1000-20000");
                convertPardalos(@"P1000-25000");
                convertPardalos(@"P1000-30000");
                convertPace(@"h_001");
                convertPace(@"h_003");
                convertPace(@"h_005");
                convertPace(@"h_007");
                convertPace(@"h_009");
                convertPace(@"h_011");
                convertPace(@"h_013");
                convertPace(@"h_015");
                convertPace(@"h_017");
                convertPace(@"h_019");
                convertPace(@"h_021");
                convertPace(@"h_023");
                convertPace(@"h_025");
                convertPace(@"h_027");
                convertPace(@"h_029");
                convertPace(@"h_031");
                convertPace(@"h_033");
                convertPace(@"h_035");
                convertPace(@"h_037");
                convertPace(@"h_039");
                convertPace(@"h_041");
                convertPace(@"h_043");
                convertPace(@"h_045");
                convertPace(@"h_047");
                convertPace(@"h_049");
                convertPace(@"h_051");
                convertPace(@"h_053");
                convertPace(@"h_055");
                convertPace(@"h_057");
                convertPace(@"h_059");
                convertPace(@"h_061");
                convertPace(@"h_063");
                convertPace(@"h_065");
                convertPace(@"h_067");
                convertPace(@"h_069");
                convertPace(@"h_071");
                convertPace(@"h_073");
                convertPace(@"h_075");
                convertPace(@"h_077");
                convertPace(@"h_079");
                convertPace(@"h_081");
                convertPace(@"h_083");
                convertPace(@"h_085");
                convertPace(@"h_087");
                convertPace(@"h_089");
                convertPace(@"h_091");
                convertPace(@"h_093");
                convertPace(@"h_095");
                convertPace(@"h_097");
                convertPace(@"h_099");
                convertPace(@"h_101");
                convertPace(@"h_103");
                convertPace(@"h_105");
                convertPace(@"h_107");
                convertPace(@"h_109");
                convertPace(@"h_111");
                convertPace(@"h_113");
                convertPace(@"h_115");
                convertPace(@"h_117");
                convertPace(@"h_119");
                convertPace(@"h_121");
                convertPace(@"h_123");
                convertPace(@"h_125");
                convertPace(@"h_127");
                convertPace(@"h_129");
                convertPace(@"h_131");
                convertPace(@"h_133");
                convertPace(@"h_135");
                convertPace(@"h_137");
                convertPace(@"h_139");
                convertPace(@"h_141");
                convertPace(@"h_143");
                convertPace(@"h_145");
                convertPace(@"h_147");
                convertPace(@"h_149");
                convertPace(@"h_151");
                convertPace(@"h_153");
                convertPace(@"h_155");
                convertPace(@"h_157");
                convertPace(@"h_159");
                convertPace(@"h_161");
                convertPace(@"h_163");
                convertPace(@"h_165");
                convertPace(@"h_167");
                convertPace(@"h_169");
                convertPace(@"h_171");
                convertPace(@"h_173");
                convertPace(@"h_175");
                convertPace(@"h_177");
                convertPace(@"h_179");
                convertPace(@"h_181");
                convertPace(@"h_183");
                convertPace(@"h_185");
                convertPace(@"h_187");
                convertPace(@"h_189");
                convertPace(@"h_191");
                convertPace(@"h_193");
                convertPace(@"h_195");
                convertPace(@"h_197");
                convertPace(@"h_199");
                convertPace(@"e_001");
                convertPace(@"e_003");
                convertPace(@"e_005");
                convertPace(@"e_007");
                convertPace(@"e_009");
                convertPace(@"e_011");
                convertPace(@"e_013");
                convertPace(@"e_015");
                convertPace(@"e_017");
                convertPace(@"e_019");
                convertPace(@"e_021");
                convertPace(@"e_023");
                convertPace(@"e_025");
                convertPace(@"e_027");
                convertPace(@"e_029");
                convertPace(@"e_031");
                convertPace(@"e_033");
                convertPace(@"e_035");
                convertPace(@"e_037");
                convertPace(@"e_039");
                convertPace(@"e_041");
                convertPace(@"e_043");
                convertPace(@"e_045");
                convertPace(@"e_047");
                convertPace(@"e_049");
                convertPace(@"e_051");
                convertPace(@"e_053");
                convertPace(@"e_055");
                convertPace(@"e_057");
                convertPace(@"e_059");
                convertPace(@"e_061");
                convertPace(@"e_063");
                convertPace(@"e_065");
                convertPace(@"e_067");
                convertPace(@"e_069");
                convertPace(@"e_071");
                convertPace(@"e_073");
                convertPace(@"e_075");
                convertPace(@"e_077");
                convertPace(@"e_079");
                convertPace(@"e_081");
                convertPace(@"e_083");
                convertPace(@"e_085");
                convertPace(@"e_087");
                convertPace(@"e_089");
                convertPace(@"e_091");
                convertPace(@"e_093");
                convertPace(@"e_095");
                convertPace(@"e_097");
                convertPace(@"e_099");
                convertPace(@"e_101");
                convertPace(@"e_103");
                convertPace(@"e_105");
                convertPace(@"e_107");
                convertPace(@"e_109");
                convertPace(@"e_111");
                convertPace(@"e_113");
                convertPace(@"e_115");
                convertPace(@"e_117");
                convertPace(@"e_119");
                convertPace(@"e_121");
                convertPace(@"e_123");
                convertPace(@"e_125");
                convertPace(@"e_127");
                convertPace(@"e_129");
                convertPace(@"e_131");
                convertPace(@"e_133");
                convertPace(@"e_135");
                convertPace(@"e_137");
                convertPace(@"e_139");
                convertPace(@"e_141");
                convertPace(@"e_143");
                convertPace(@"e_145");
                convertPace(@"e_147");
                convertPace(@"e_149");
                convertPace(@"e_151");
                convertPace(@"e_153");
                convertPace(@"e_155");
                convertPace(@"e_157");
                convertPace(@"e_159");
                convertPace(@"e_161");
                convertPace(@"e_163");
                convertPace(@"e_165");
                convertPace(@"e_167");
                convertPace(@"e_169");
                convertPace(@"e_171");
                convertPace(@"e_173");
                convertPace(@"e_175");
                convertPace(@"e_177");
                convertPace(@"e_179");
                convertPace(@"e_181");
                convertPace(@"e_183");
                convertPace(@"e_185");
                convertPace(@"e_187");
                convertPace(@"e_189");
                convertPace(@"e_191");
                convertPace(@"e_193");
                convertPace(@"e_195");
                convertPace(@"e_197");
                convertPace(@"e_199");
            }

            static void convertPardalos(string oldPath) {
                string[] lines = File.ReadAllLines(oldPath + ".dat");
                string[] words = lines[0].Split(Checker.InlineDelimiters, StringSplitOptions.RemoveEmptyEntries);
                string nodeNum = words[0];
                string arcNum = words[1];
                List<int>[] adjList = new List<int>[int.Parse(nodeNum)];
                for (int i = 0; i < adjList.Length; ++i) { adjList[i] = new List<int>(); }
                for (int l = 1; l < lines.Length; ++l) {
                    words = lines[l].Split(Checker.InlineDelimiters, StringSplitOptions.RemoveEmptyEntries);
                    adjList[int.Parse(words[0]) - 1].Add(int.Parse(words[1]) - 1);
                }

                StringBuilder sb = new StringBuilder();
                sb.Append(nodeNum).Append(' ').AppendLine(arcNum);
                for (int i = 0; i < adjList.Length; ++i) {
                    if (adjList[i].Count > 0) {
                        foreach (var n in adjList[i]) {
                            sb.Append(n).Append(' ');
                        }
                        --sb.Length;
                    }
                    sb.Append('\n');
                }

                StringBuilder fn = new StringBuilder();
                fn.Append("pardalos.n").Append(nodeNum).Append('e').Append(arcNum).Append(".txt");
                File.WriteAllText(fn.ToString(), sb.ToString());
            }

            static void convertPace(string oldPath) {
                string[] lines = File.ReadAllLines(oldPath);
                string[] words = lines[0].Split(Checker.InlineDelimiters, StringSplitOptions.RemoveEmptyEntries);
                string nodeNum = words[0];
                string arcNum = words[1];
                List<int>[] adjList = new List<int>[int.Parse(nodeNum)];
                for (int i = 0; i < adjList.Length; ++i) { adjList[i] = new List<int>(); }
                for (int l = 1; l < lines.Length; ++l) {
                    words = lines[l].Split(Checker.InlineDelimiters, StringSplitOptions.RemoveEmptyEntries);
                    foreach (var w in words) { adjList[l - 1].Add(int.Parse(w) - 1); }
                }

                StringBuilder sb = new StringBuilder();
                sb.Append(nodeNum).Append(' ').AppendLine(arcNum);
                for (int i = 0; i < adjList.Length; ++i) {
                    if (adjList[i].Count > 0) {
                        foreach (var n in adjList[i]) {
                            sb.Append(n).Append(' ');
                        }
                        --sb.Length;
                    }
                    sb.Append('\n');
                }

                StringBuilder fn = new StringBuilder();
                fn.Append("pace.").Append(Path.GetFileName(oldPath).Replace("_", ""))
                    .Append(".n").Append(nodeNum).Append('e').Append(arcNum).Append(".txt");
                File.WriteAllText(fn.ToString(), sb.ToString());
            }
        }

        public class MCDSP {
            public static void convertAll() {
                convertEdgeList("n1000_ep0007.rg", "BOBL");
                convertEdgeList("n1000_ep0014.rg", "BOBL");
                convertEdgeList("n1000_ep0028.rg", "BOBL");
                convertEdgeList("n1000_ep0056.rg", "BOBL");
                convertEdgeList("n1000_ep0112.rg", "BOBL");
                convertEdgeList("n1000_ep0224.rg", "BOBL");
                convertEdgeList("n1000_r0048.rgg", "BOBL");
                convertEdgeList("n1000_r0070.rgg", "BOBL");
                convertEdgeList("n1000_r0100.rgg", "BOBL");
                convertEdgeList("n1000_r0140.rgg", "BOBL");
                convertEdgeList("n1000_r0207.rgg", "BOBL");
                convertEdgeList("n1000_r0308.rgg", "BOBL");
                convertEdgeList("n5000_ep0007.rg", "BOBL");
                convertEdgeList("n5000_ep0014.rg", "BOBL");
                convertEdgeList("n5000_ep0028.rg", "BOBL");
                convertEdgeList("n5000_ep0056.rg", "BOBL");
                convertEdgeList("n5000_ep0112.rg", "BOBL");
                convertEdgeList("n5000_ep0224.rg", "BOBL");
                convertEdgeList("n5000_r0048.rgg", "BOBL");
                convertEdgeList("n5000_r0070.rgg", "BOBL");
                convertEdgeList("n5000_r0100.rgg", "BOBL");
                convertEdgeList("n5000_r0140.rgg", "BOBL");
                convertEdgeList("n5000_r0207.rgg", "BOBL");
                convertEdgeList("n5000_r0308.rgg", "BOBL");

                convertEdgeList("ieee_14_bus.txt", "BPFTC");
                convertEdgeList("ieee_30_bus.txt", "BPFTC");
                convertEdgeList("ieee_57_bus.txt", "BPFTC");
                convertEdgeList("ieee_118_bus.txt", "BPFTC");
                convertEdgeList("ieee_300_bus.txt", "BPFTC");
                convertEdgeList("rts96.txt", "BPFTC");

                convertEdgeList("v30_d10.dat", "LMS");
                convertEdgeList("v30_d20.dat", "LMS");
                convertEdgeList("v30_d30.dat", "LMS");
                convertEdgeList("v30_d50.dat", "LMS");
                convertEdgeList("v30_d70.dat", "LMS");
                convertEdgeList("v50_d5.dat", "LMS");
                convertEdgeList("v50_d10.dat", "LMS");
                convertEdgeList("v50_d20.dat", "LMS");
                convertEdgeList("v50_d30.dat", "LMS");
                convertEdgeList("v50_d50.dat", "LMS");
                convertEdgeList("v50_d70.dat", "LMS");
                convertEdgeList("v70_d5.dat", "LMS");
                convertEdgeList("v70_d10.dat", "LMS");
                convertEdgeList("v70_d20.dat", "LMS");
                convertEdgeList("v70_d30.dat", "LMS");
                convertEdgeList("v70_d50.dat", "LMS");
                convertEdgeList("v70_d70.dat", "LMS");
                convertEdgeList("v100_d5.dat", "LMS");
                convertEdgeList("v100_d10.dat", "LMS");
                convertEdgeList("v100_d20.dat", "LMS");
                convertEdgeList("v100_d30.dat", "LMS");
                convertEdgeList("v100_d50.dat", "LMS");
                convertEdgeList("v100_d70.dat", "LMS");
                convertEdgeList("v120_d5.dat", "LMS");
                convertEdgeList("v120_d10.dat", "LMS");
                convertEdgeList("v120_d20.dat", "LMS");
                convertEdgeList("v120_d30.dat", "LMS");
                convertEdgeList("v120_d50.dat", "LMS");
                convertEdgeList("v120_d70.dat", "LMS");
                convertEdgeList("v150_d5.dat", "LMS");
                convertEdgeList("v150_d10.dat", "LMS");
                convertEdgeList("v150_d20.dat", "LMS");
                convertEdgeList("v150_d30.dat", "LMS");
                convertEdgeList("v150_d50.dat", "LMS");
                convertEdgeList("v150_d70.dat", "LMS");
                convertEdgeList("v200_d5.dat", "LMS");
                convertEdgeList("v200_d10.dat", "LMS");
                convertEdgeList("v200_d20.dat", "LMS");
                convertEdgeList("v200_d30.dat", "LMS");
                convertEdgeList("v200_d50.dat", "LMS");
                convertEdgeList("v200_d70.dat", "LMS");

                convertEdgeList("rand_40_200_1235844018_0_k=5_sat.gph", "LPNMR");
                convertEdgeList("rand_45_250_1235855661_0_k=5_sat.gph", "LPNMR");
                convertEdgeList("rand_50_250_1235336870_0_k=8_sat.gph", "LPNMR");
                convertEdgeList("rand_50_250_1235842610_0_k=7_sat.gph", "LPNMR");
                convertEdgeList("rand_55_250_1235855873_0_k=8_sat.gph", "LPNMR");
                convertEdgeList("rand_60_400_1235843083_0_k=7_sat.gph", "LPNMR");
                convertEdgeList("rand_70_250_1235844252_0_k=13_sat.gph", "LPNMR");
                convertEdgeList("rand_80_500_1235848939_0_k=9_sat.gph", "LPNMR");
                convertEdgeList("rand_90_600_1235859689_0_k=10_sat.gph", "LPNMR");

                convertEdgeList("n400_80_r60.txt", "RGG");
                convertEdgeList("n400_80_r70.txt", "RGG");
                convertEdgeList("n400_80_r80.txt", "RGG");
                convertEdgeList("n400_80_r90.txt", "RGG");
                convertEdgeList("n400_80_r100.txt", "RGG");
                convertEdgeList("n400_80_r110.txt", "RGG");
                convertEdgeList("n400_80_r120.txt", "RGG");
                convertEdgeList("n600_100_r80.txt", "RGG");
                convertEdgeList("n600_100_r90.txt", "RGG");
                convertEdgeList("n600_100_r100.txt", "RGG");
                convertEdgeList("n600_100_r110.txt", "RGG");
                convertEdgeList("n600_100_r120.txt", "RGG");
                convertEdgeList("n700_200_r70.txt", "RGG");
                convertEdgeList("n700_200_r80.txt", "RGG");
                convertEdgeList("n700_200_r90.txt", "RGG");
                convertEdgeList("n700_200_r100.txt", "RGG");
                convertEdgeList("n700_200_r110.txt", "RGG");
                convertEdgeList("n700_200_r120.txt", "RGG");
                convertEdgeList("n1000_200_r100.txt", "RGG");
                convertEdgeList("n1000_200_r110.txt", "RGG");
                convertEdgeList("n1000_200_r120.txt", "RGG");
                convertEdgeList("n1000_200_r130.txt", "RGG");
                convertEdgeList("n1000_200_r140.txt", "RGG");
                convertEdgeList("n1000_200_r150.txt", "RGG");
                convertEdgeList("n1000_200_r160.txt", "RGG");
                convertEdgeList("n1500_250_r130.txt", "RGG");
                convertEdgeList("n1500_250_r140.txt", "RGG");
                convertEdgeList("n1500_250_r150.txt", "RGG");
                convertEdgeList("n1500_250_r160.txt", "RGG");
                convertEdgeList("n2000_300_r200.txt", "RGG");
                convertEdgeList("n2000_300_r210.txt", "RGG");
                convertEdgeList("n2000_300_r220.txt", "RGG");
                convertEdgeList("n2000_300_r230.txt", "RGG");
                convertEdgeList("n2500_350_r200.txt", "RGG");
                convertEdgeList("n2500_350_r210.txt", "RGG");
                convertEdgeList("n2500_350_r220.txt", "RGG");
                convertEdgeList("n2500_350_r230.txt", "RGG");
                convertEdgeList("n3000_400_r210.txt", "RGG");
                convertEdgeList("n3000_400_r220.txt", "RGG");
                convertEdgeList("n3000_400_r230.txt", "RGG");
                convertEdgeList("n3000_400_r240.txt", "RGG");

                convertEdgeList("sparse-n1000-np300.txt", "Sparse");
                convertEdgeList("sparse-n1000-np500.txt", "Sparse");
                convertEdgeList("sparse-n1000-np700.txt", "Sparse");
                convertEdgeList("sparse-n1500-np500.txt", "Sparse");
                convertEdgeList("sparse-n1500-np700.txt", "Sparse");
                convertEdgeList("sparse-n1500-np900.txt", "Sparse");
                convertEdgeList("sparse-n2000-np600.txt", "Sparse");
                convertEdgeList("sparse-n2000-np800.txt", "Sparse");
                convertEdgeList("sparse-n2000-np1000.txt", "Sparse");
                convertEdgeList("sparse-n2500-np800.txt", "Sparse");
                convertEdgeList("sparse-n2500-np1000.txt", "Sparse");
                convertEdgeList("sparse-n2500-np1200.txt", "Sparse");
                convertEdgeList("sparse-n3000-np1000.txt", "Sparse");
                convertEdgeList("sparse-n3000-np1300.txt", "Sparse");
                convertEdgeList("sparse-n3000-np1600.txt", "Sparse");
            }

            static void convertEdgeList(string oldPath, string dataset) {
                string[] lines = File.ReadAllLines(oldPath);
                string[] words = lines[0].Split(Checker.InlineDelimiters, StringSplitOptions.RemoveEmptyEntries);
                string nodeNum = words[0];
                string arcNum = words[1];
                StringBuilder sb = new StringBuilder();
                sb.Append(nodeNum).Append(' ').Append(arcNum).Append('\n');
                for (int l = 1; l < lines.Length; ++l) {
                    if (lines[l].Length <= 0) { continue; }
                    words = lines[l].Split(Checker.InlineDelimiters, StringSplitOptions.RemoveEmptyEntries);
                    sb.Append(int.Parse(words[0]) - 1).Append(' ').Append(int.Parse(words[1]) - 1).Append('\n');
                }

                StringBuilder fn = new StringBuilder();
                fn.Append(dataset).Append(".n").Append(nodeNum).Append('e').Append(arcNum).Append(".txt");
                File.WriteAllText(fn.ToString(), sb.ToString());
                Console.WriteLine(oldPath + ' ' + fn.ToString());
            }
        }

        public class PECCP {
            public static void convertAll() {
                Dictionary<int, string> bks = new Dictionary<int, string> {
                    { 1, "1.0000000000000000000000000000" },
                    { 2, "2.0000000000000000000000000000" },
                    { 3, "2.1547005383792515290182975610" },
                    { 4, "2.4142135623730950488016887242" },
                    { 5, "2.7013016167040798643630809941" },
                    { 6, "3.0000000000000000000000000000" },
                    { 7, "3.0000000000000000000000000000" },
                    { 8, "3.3047648709624865052411502235" },
                    { 9, "3.6131259297527530557132863469" },
                    { 10, "3.8130256313981243982516251560" },
                    { 11, "3.9238044001630872522327544134" },
                    { 12, "4.0296019301161834974827410413" },
                    { 13, "4.2360679774997896964091736687" },
                    { 14, "4.3284285548608366814039093675" },
                    { 15, "4.5213569647061642409073640084" },
                    { 16, "4.6154255948731939855392441620" },
                    { 17, "4.7920337483105791701491239533" },
                    { 18, "4.8637033051562731469989727989" },
                    { 19, "4.8637033051562731469989727989" },
                    { 20, "5.1223207369915283214476857922" },
                    { 21, "5.2523174750102422915460321681" },
                    { 22, "5.4397189590722134554857208822" },
                    { 23, "5.5452042225748582685233836876" },
                    { 24, "5.6516610917654367675791591314" },
                    { 25, "5.7528243308571153542293693448" },
                    { 26, "5.8281765369429493922062904714" },
                    { 27, "5.9063978473941545445249130172" },
                    { 28, "6.0149380973715121761530340729" },
                    { 29, "6.1385979039781456873652132661" },
                    { 30, "6.1977410708792260252433165923" },
                    { 31, "6.2915026221291811810032315073" },
                    { 32, "6.4294629709501135719882575974" },
                    { 33, "6.4867031235604333384960709573" },
                    { 34, "6.6109570900010028498504099889" },
                    { 35, "6.6971710917902442087214203194" },
                    { 36, "6.7467537934241754287626593644" },
                    { 37, "6.7587704831436335362164371093" },
                    { 38, "6.9618869652281487003392861751" },
                    { 39, "7.0578841626240042418445508455" },
                    { 40, "7.1238464359431248414143006495" },
                    { 41, "7.2600123286770460209996879266" },
                    { 42, "7.3467964069427686820835173680" },
                    { 43, "7.4199448563412109538502726317" },
                    { 44, "7.4980366829952483920611117219" },
                    { 45, "7.5729123263675215353211135129" },
                    { 46, "7.6501799146936677467655841557" },
                    { 47, "7.7241700525980185827076359726" },
                    { 48, "7.7912714305586675904508387393" },
                    { 49, "7.8868709588028646183617076263" },
                    { 50, "7.9475152747835107452038821496" },
                    { 51, "8.0275069524191890919129301062" },
                    { 52, "8.0847171906898829801310519896" },
                    { 53, "8.1795828268410683199877039635" },
                    { 54, "8.2039823834693371921171485911" },
                    { 55, "8.2111025509279785862384425349" },
                    { 56, "8.3835299225789572914412054485" },
                    { 57, "8.4471846534104282557549363563" },
                    { 58, "8.5245537701396069588854198515" },
                    { 59, "8.5924999593700717605905863678" },
                    { 60, "8.6462198454579193847041425608" },
                    { 61, "8.6612975755403886690909628653" },
                    { 62, "8.8297654089720517930686110274" },
                    { 63, "8.8923515375505536332378887958" },
                    { 64, "8.9619711084857383021613006266" },
                    { 65, "9.0173973232087350460475542013" },
                    { 66, "9.0962794269242756647629057074" },
                    { 67, "9.1689718817837957262351968737" },
                    { 68, "9.2297737467506672260419962729" },
                    { 69, "9.2697612666410936587426229785" },
                    { 70, "9.3456531940484826092717512253" },
                    { 71, "9.4157968968708229531192841998" },
                    { 72, "9.4738908567130273495618118642" },
                    { 73, "9.5403461521378991406456140615" },
                    { 74, "9.5892327643391725982091723484" },
                    { 75, "9.6720296319471139991260731919" },
                    { 76, "9.7295968021616445662321924714" },
                    { 77, "9.7989119245067863939174480968" },
                    { 78, "9.8577098998848208566592136212" },
                    { 79, "9.9050634676610127017578531932" },
                    { 80, "9.9681518131533798071034975412" },
                    { 81, "10.0108642412007307859864247798" },
                    { 82, "10.0508242234505218680194880060" },
                    { 83, "10.1168578751020959305277543283" },
                    { 84, "10.1495308672361587954446565027" },
                    { 85, "10.1631114658767610748858834751" },
                    { 86, "10.2987010531101919248398041752" },
                    { 87, "10.3632085050778998566306887322" },
                    { 88, "10.4323376927323602163663622221" },
                    { 89, "10.5004918145736102113826359314" },
                    { 90, "10.5460691779537198089458029723" },
                    { 91, "10.5667722335056261337227772794" },
                    { 92, "10.6846458479156644755133509374" },
                    { 93, "10.7333526002596364254447481500" },
                    { 94, "10.7780321602520198094572385731" },
                    { 95, "10.8402050215974201357665666039" },
                    { 96, "10.8832027597222846752088691255" },
                    { 97, "10.9385901100732978319039055308" },
                    { 98, "10.9793831282069775936470182421" },
                    { 99, "11.0331411514449735781402495310" },
                    { 100, "11.0821497243103699086257335782" },
                    { 101, "11.1469335754265382471061272191" },
                    { 102, "11.1968634727696600948764015011" },
                    { 103, "11.2651435658263984646467821994" },
                    { 104, "11.3176585663304875527944341841" },
                    { 105, "11.3626596129066573160796587562" },
                    { 106, "11.4218343660456461684025658586" },
                    { 107, "11.4720518374247524050529309749" },
                    { 108, "11.5240161340352466333679140104" },
                    { 109, "11.5621190710742770452156969698" },
                    { 110, "11.6168615502950367091649046633" },
                    { 111, "11.6628111844931478192386377737" },
                    { 112, "11.7052745264300809400741383847" },
                    { 113, "11.7475281224804981254839799158" },
                    { 114, "11.7951733640304600650106436911" },
                    { 115, "11.8394740093465938781728059434" },
                    { 116, "11.8967045003329706358261958380" },
                    { 117, "11.9434755362778876721716233023" },
                    { 118, "11.9855510463150610827982323538" },
                    { 119, "12.0423344438724781128302537706" },
                    { 120, "12.0852124599284667980680472996" },
                    { 121, "12.1248030819023424730975444392" },
                    { 122, "12.2043748116092193498124653150" },
                    { 123, "12.2763999090031473203286629900" },
                    { 124, "12.3217083152689478835464571738" },
                    { 125, "12.3682253207490942929251433460" },
                    { 126, "12.4171443917805119117063709701" },
                    { 127, "12.4615495148191989045474694885" },
                    { 128, "12.5023100709102814006803141105" },
                    { 129, "12.5537178187810239918347846675" },
                    { 130, "12.6003399701114242678867196832" },
                    { 131, "12.6496204614972823341873656230" },
                    { 132, "12.6874367905858006602768454626" },
                    { 133, "12.7352730885190409894259244830" },
                    { 134, "12.7714462397704341851621045399" },
                    { 135, "12.8142547714824447375210049106" },
                    { 136, "12.8657595506788380945789210298" },
                    { 137, "12.9144699002576862291979090665" },
                    { 138, "12.9613043108994870126762496160" },
                    { 139, "13.0089872407273277553253595350" },
                    { 140, "13.0597775057331085186544481710" },
                    { 141, "13.1072552949088365460563493543" },
                    { 142, "13.1464116255385592409290912748" },
                    { 143, "13.1974008249809039444391604958" },
                    { 144, "13.2477892250808460137655375892" },
                    { 145, "13.2766686298630886563838107226" },
                    { 146, "13.3312337680574898736878361734" },
                    { 147, "13.3571124946283477390153814267" },
                    { 148, "13.3869393551487226056863245151" },
                    { 149, "13.4355485182851237313297932008" },
                    { 150, "13.4608063709697900630016100898" },
                    { 151, "13.4761915083336893906389728645" },
                    { 152, "13.5317484870436019681015714457" },
                    { 153, "13.5915366870867321243280845076" },
                    { 154, "13.6368976706064949274792757265" },
                    { 155, "13.6737086404331516954698616270" },
                    { 156, "13.7163630489668480714323185470" },
                    { 157, "13.7729919003607813871100061173" },
                    { 158, "13.8232259166556617798603884043" },
                    { 159, "13.8641933675643960056667989009" },
                    { 160, "13.9196469418998501901931865519" },
                    { 161, "13.9694002696222885742122189033" },
                    { 162, "14.0101723316444130131772331455" },
                    { 163, "14.0652037110686895148409147452" },
                    { 164, "14.1096013733016879545083043916" },
                    { 165, "14.1443392923440231950706097233" },
                    { 166, "14.1856784202138940569209761848" },
                    { 167, "14.2190465877266622408736345659" },
                    { 168, "14.2577069152339919549322094719" },
                    { 169, "14.2953582279587828443077204535" },
                    { 170, "14.3311502744014509653586224506" },
                    { 171, "14.3676614475788151339516887407" },
                    { 172, "14.4167760836659018626587512868" },
                    { 173, "14.4513414361521701591482587807" },
                    { 174, "14.4928488387725855082159684362" },
                    { 175, "14.5364265080413278955989834125" },
                    { 176, "14.5741356548945390139077984623" },
                    { 177, "14.6145183915210320468996851148" },
                    { 178, "14.6588142238343864556255630608" },
                    { 179, "14.6998512520145806790959349327" },
                    { 180, "14.7390348789775911355004606392" },
                    { 181, "14.7808296565180690340818711248" },
                    { 182, "14.8189306311218648106197091087" },
                    { 183, "14.8656708374563219340090101863" },
                    { 184, "14.9054183044766775906142009154" },
                    { 185, "14.9381448354693417800672981997" },
                    { 186, "14.9611856849593948227393846685" },
                    { 187, "14.9891067504960502330823151103" },
                    { 188, "15.0287506891789476085167729008" },
                    { 189, "15.0690993725873700315044812103" },
                    { 190, "15.1036015044641347546509774428" },
                    { 191, "15.1423575111208273373178607797" },
                    { 192, "15.1694352123357453910484765858" },
                    { 193, "15.1993744953715701206736462660" },
                    { 194, "15.2414286570096097634780346313" },
                    { 195, "15.2871721678572280992440819574" },
                    { 196, "15.3229999968270922864553524906" },
                    { 197, "15.3674973823175215478248241415" },
                    { 198, "15.3899032013753373808518175954" },
                    { 199, "15.4022604152885652734276720590" },
                    { 200, "15.4632748785420938110388104429" },
                    { 201, "15.5202252928783596543164782444" },
                    { 202, "15.5680288369975571751706480510" },
                    { 203, "15.6112355175255907101923015973" },
                    { 204, "15.6424840847915931133367162355" },
                    { 205, "15.6909882687448969450810623795" },
                    { 206, "15.7345575112919712403610956488" },
                    { 207, "15.7701905692150945123135461445" },
                    { 208, "15.8117640861807465733913363753" },
                    { 209, "15.8398713581232796303756896272" },
                    { 210, "15.8787794237992720650577534603" },
                    { 211, "15.9025124865478752137832018924" },
                    { 212, "15.9353690989148198439139694000" },
                    { 213, "15.9698559410324953715443613693" },
                    { 214, "16.0179958151701771590155144937" },
                    { 215, "16.0493062100288447467174184439" },
                    { 216, "16.0851544989131357161671677554" },
                    { 217, "16.1166932961447405275777920035" },
                    { 218, "16.1495529378280203273406519054" },
                    { 219, "16.1691550981349604364503718083" },
                    { 220, "16.2240948246627448790290900350" },
                    { 221, "16.2588343455251273991584809410" },
                    { 222, "16.2987893466875673552837014101" },
                    { 223, "16.3371665983845199586055481870" },
                    { 224, "16.3688316850140736147914253855" },
                    { 225, "16.4036037815605299414074683744" },
                    { 226, "16.4483187175509875491623947907" },
                    { 227, "16.4883699220993278450742955515" },
                    { 228, "16.5249980710321232506193634558" },
                    { 229, "16.5626646047325600337319052078" },
                    { 230, "16.5925649892633700221577104950" },
                    { 231, "16.6290609333931952564866454479" },
                    { 232, "16.6647046452719209911230016299" },
                    { 233, "16.6948299423700160400567856120" },
                    { 234, "16.7063358831591239361001620372" },
                    { 235, "16.7125757859606098355610586350" },
                    { 236, "16.7743924883051285901870675820" },
                    { 237, "16.8015828598555444433308416833" },
                    { 238, "16.8269089946331820033619547887" },
                    { 239, "16.8635056215994110479564115089" },
                    { 240, "16.8944720757127196725837745005" },
                    { 241, "16.9150645837091892739725163902" },
                    { 242, "16.9607086593884323870043293906" },
                    { 243, "17.0002479552375666806669506754" },
                    { 244, "17.0348063778421421357905934808" },
                    { 245, "17.0746892054650702841474985398" },
                    { 246, "17.1079224802962059760579707339" },
                    { 247, "17.1320689451876882483482516695" },
                    { 248, "17.1821679273081930663749947977" },
                    { 249, "17.2190021241975091073202315427" },
                    { 250, "17.2619549069166399724674458495" },
                    { 251, "17.2953898979320920585299627468" },
                    { 252, "17.3251959001625737468828957916" },
                    { 253, "17.3459563226696384171750289274" },
                    { 254, "17.3937631870852272416867552056" },
                    { 255, "17.4440113501950621832231588501" },
                    { 256, "17.4862513044215434116560941964" },
                    { 257, "17.5179649183477340172173568970" },
                    { 258, "17.5382357767401891317467251052" },
                    { 259, "17.5739546547044501183889622666" },
                    { 260, "17.6008354838126732064910786484" },
                    { 261, "17.6272406721740352959267775894" },
                    { 262, "17.6579067866954889917690322427" },
                    { 263, "17.6886138363963228876870238555" },
                    { 264, "17.7013739262275838673381489368" },
                    { 265, "17.7412247628986603530561647299" },
                    { 266, "17.7760867684291997832061262765" },
                    { 267, "17.7973613039906757499024743622" },
                    { 268, "17.8320685079847132889880622687" },
                    { 269, "17.8625792679365094035830235717" },
                    { 270, "17.8872656677086791981316097681" },
                    { 271, "17.9296938825221520940238860774" },
                    { 272, "17.9591897216567972703538947411" },
                    { 273, "17.9952993468500123591289536129" },
                    { 274, "18.0336983646346192925890396821" },
                    { 275, "18.0626536889807627993450987394" },
                    { 276, "18.1034164739463259370639403918" },
                    { 277, "18.1360794644897753007726797610" },
                    { 278, "18.1805559751907234415621152632" },
                    { 279, "18.2175684474813896643639711354" },
                    { 280, "18.2459274734905471891373076055" },
                    { 281, "18.2804085757660627473273188840" },
                    { 282, "18.3087899448367376783214195635" },
                    { 283, "18.3402365752204520601281282792" },
                    { 284, "18.3594566407206223596050390789" },
                    { 285, "18.4026785950486345613963924160" },
                    { 286, "18.4292417626026806914550049532" },
                    { 287, "18.4682120772873587158495619508" },
                    { 288, "18.4943817280456846343046346020" },
                    { 289, "18.5116020899732017287755293981" },
                    { 290, "18.5487075867602665128184044655" },
                    { 291, "18.5673514541510359394045304942" },
                    { 292, "18.5945010895894833138219448013" },
                    { 293, "18.6233666998932200451240520027" },
                    { 294, "18.6448496966736234262731363747" },
                    { 295, "18.6552175221045694175036854011" },
                    { 296, "18.7027500058348009842313378193" },
                    { 297, "18.7297814988660642283744261494" },
                    { 298, "18.7593447494851384936698246877" },
                    { 299, "18.7854278118538095087727881976" },
                    { 300, "18.8131537062931831269590797897" },
                    { 301, "18.8434635069908858780859009160" },
                    { 302, "18.8917822550022562082180491229" },
                    { 303, "18.9297491525388083429270932632" },
                    { 304, "18.9644417508230520185586258663" },
                    { 305, "19.0017545651180791322037966311" },
                    { 306, "19.0303894072423338600255203012" },
                    { 307, "19.0601609214767925350406354244" },
                    { 308, "19.1049914367853007627721018146" },
                    { 309, "19.1425731648941607828586354586" },
                    { 310, "19.1789282653886124957679854388" },
                    { 311, "19.2113864051183677299875122219" },
                    { 312, "19.2335856527350863718482300496" },
                    { 313, "19.2571030143116321312672897778" },
                    { 314, "19.2861951409320677565957720670" },
                    { 315, "19.3022880676976142243201074794" },
                    { 316, "19.3340417543838142352433886059" },
                    { 317, "19.3675956723614500115986469430" },
                    { 318, "19.3915660907079258309661318770" },
                    { 319, "19.4242778298942753026185769892" },
                    { 320, "19.4562307640488204917027883836" },
                    { 321, "19.4929547404453992938108731847" },
                    { 322, "19.5291390715759513290428324896" },
                    { 323, "19.5540770033103744348878094858" },
                    { 324, "19.5774599683941350418140732011" },
                    { 325, "19.6018036203835543319803520815" },
                    { 326, "19.6279752963988715767619069236" },
                    { 327, "19.6491809910461082745478095884" },
                    { 328, "19.6769937090297968251341200219" },
                    { 329, "19.7054197856504547670567805680" },
                    { 330, "19.7332520642129983678882001124" },
                    { 331, "19.7612825354324218773993976408" },
                    { 332, "19.7942170978332407608548739015" },
                    { 333, "19.8229381817007463347451689087" },
                    { 334, "19.8555133073208322035652291601" },
                    { 335, "19.8851740610575929168709346100" },
                    { 336, "19.9196894206293640877898576912" },
                    { 337, "19.9504076162035542878726298219" },
                    { 338, "19.9911678904299167552155325893" },
                    { 339, "20.0292838595981300397346477881" },
                    { 340, "20.0593208057620819552731154196" },
                    { 341, "20.0909026277784269822509659275" },
                    { 342, "20.1070905160953347315522943082" },
                    { 343, "20.1132509977312713969368976985" },
                    { 344, "20.1618387580277347023305917351" },
                    { 345, "20.1749165684868106173991783548" },
                    { 346, "20.1964520618743200600676217826" },
                    { 347, "20.2225997296660057534550618248" },
                    { 348, "20.2352911473446586746975710968" },
                    { 349, "20.2414086671069312513131715878" },
                    { 350, "20.2991355651137556215620946983" },
                    { 351, "20.3311334119197955768620556100" },
                    { 352, "20.3655193762664828888416818080" },
                    { 353, "20.3921848857040216684227352552" },
                    { 354, "20.4150176841265648726868515282" },
                    { 355, "20.4417330070438665558602519397" },
                    { 356, "20.4782258193898727672077667210" },
                    { 357, "20.5026637050243934278367607422" },
                    { 358, "20.5345602085403057448351989375" },
                    { 359, "20.5639740493270528004995958185" },
                    { 360, "20.5845243209710978132570979508" },
                    { 361, "20.5965075825145461703152462749" },
                    { 362, "20.6440811608641409837586800420" },
                    { 363, "20.6709637674556402419198560233" },
                    { 364, "20.6982740697914560067558608171" },
                    { 365, "20.7273855789210661132854176125" },
                    { 366, "20.7563681487752331142722103316" },
                    { 367, "20.7852365379306580680166344731" },
                    { 368, "20.8335976488364014548222392644" },
                    { 369, "20.8676756671707914011144679889" },
                    { 370, "20.9032569593878826599255294424" },
                    { 371, "20.9316143896608329944226593283" },
                    { 372, "20.9472852617413311348078349437" },
                    { 373, "20.9775733010470554260261852486" },
                    { 374, "21.0012826261548865048299670184" },
                    { 375, "21.0247445137512881220932162395" },
                    { 376, "21.0385253096001994464757614734" },
                    { 377, "21.0670289309088295679041902264" },
                    { 378, "21.0902145443314033660534892375" },
                    { 379, "21.1163891682172762426920054250" },
                    { 380, "21.1391355467870843444616295672" },
                    { 381, "21.1685836455344088083326485767" },
                    { 382, "21.1894205630866113570728458993" },
                    { 383, "21.2240076660280257830457015620" },
                    { 384, "21.2600050943015092429732847977" },
                    { 385, "21.2976163430415969628539713631" },
                    { 386, "21.3278224966787551812328653702" },
                    { 387, "21.3478786782004838229783748903" },
                    { 388, "21.3678973504641584240229118876" },
                    { 389, "21.4007469091531059270332456687" },
                    { 390, "21.4284041621102092850815055746" },
                    { 391, "21.4540713807882005181886246566" },
                    { 392, "21.4799258678382607016101709989" },
                    { 393, "21.5059141895201835515022395027" },
                    { 394, "21.5293036536789282926671596931" },
                    { 395, "21.5620286535998312163455314685" },
                    { 396, "21.5887721376518933047984638128" },
                    { 397, "21.6188728795243861009033180656" },
                    { 398, "21.6378488016535885689276543073" },
                    { 399, "21.6630013903029577932855226852" },
                    { 400, "21.6895717950759766527496087556" },
                    { 401, "21.7142860750923558069121959028" },
                    { 402, "21.7330621576642508300544197803" },
                    { 403, "21.7696282636943714731942632323" },
                    { 404, "21.8058103449778205669062786201" },
                    { 405, "21.8235645111879431969837765127" },
                    { 406, "21.8421497022307833895303347318" },
                    { 407, "21.8616573858040995796034527544" },
                    { 408, "21.8752978581445374117047376123" },
                    { 409, "21.8883766357157953750126064308" },
                    { 410, "21.9218147642071114814343130306" },
                    { 411, "21.9429111789007870363371005371" },
                    { 412, "21.9601232064846365646318125465" },
                    { 413, "21.9872751342270252351171109628" },
                    { 414, "22.0076077150290002537135060589" },
                    { 415, "22.0289345485070862784796250921" },
                    { 416, "22.0574472989462889499824965531" },
                    { 417, "22.0803321403932793122302593980" },
                    { 418, "22.1020294169328499736862204818" },
                    { 419, "22.1186607123716768595331524009" },
                    { 420, "22.1354118426773607041079970928" },
                    { 421, "22.1396829834470871900626725022" },
                    { 422, "22.2166955359382217943694189674" },
                    { 423, "22.2539442896812166230411714091" },
                    { 424, "22.2889632778320428707061734114" },
                    { 425, "22.3134973516999938304439790731" },
                    { 426, "22.3447743253200443984082293379" },
                    { 427, "22.3725938872627234939181285613" },
                    { 428, "22.3987106777321134757377513623" },
                    { 429, "22.4289319441473997022891191131" },
                    { 430, "22.4578743179893473614049784171" },
                    { 431, "22.4931817125719313716532246930" },
                    { 432, "22.5224146581592372734598034130" },
                    { 433, "22.5376716074592670690400788176" },
                    { 434, "22.5832362831178408717244664428" },
                    { 435, "22.6068130922783261056989016592" },
                    { 436, "22.6276378309875144999530150571" },
                    { 437, "22.6582279991076262761675163347" },
                    { 438, "22.6747211136703101663003074921" },
                    { 439, "22.7025528425798917174469454882" },
                    { 440, "22.7217252645566763261651339124" },
                    { 441, "22.7476792635353753262181028645" },
                    { 442, "22.7736726826550084542753647677" },
                    { 443, "22.7944849970563401106622307566" },
                    { 444, "22.8135026402102665371579790093" },
                    { 445, "22.8391638775597088393225558784" },
                    { 446, "22.8608194955004497637371867625" },
                    { 447, "22.8851129603454994729565945110" },
                    { 448, "22.9121260621358594179466183880" },
                    { 449, "22.9297815953840248166488283638" },
                    { 450, "22.9507408091297583538675480049" },
                    { 451, "22.9735894083506566203191655262" },
                    { 452, "22.9929185546560016557835681122" },
                    { 453, "23.0043356715244343856991738873" },
                    { 454, "23.0524633672982246605457220236" },
                    { 455, "23.0744785310289332576179431978" },
                    { 456, "23.1027633236485147243934243320" },
                    { 457, "23.1292596893315491659465826202" },
                    { 458, "23.1493502414999063853525150179" },
                    { 459, "23.1717187992844168559109936224" },
                    { 460, "23.2143483733951364359193633380" },
                    { 461, "23.2418785961766895656171708726" },
                    { 462, "23.2704930441987215716505158044" },
                    { 463, "23.2988861853085193695050943080" },
                    { 464, "23.3221941862009842154076956295" },
                    { 465, "23.3431146116641422826791969285" },
                    { 466, "23.3679598686539477403480947399" },
                    { 467, "23.3886464562578249337399163541" },
                    { 468, "23.4194949980161406606191657503" },
                    { 469, "23.4472784655956000759856925706" },
                    { 470, "23.4697255197824528894293665930" },
                    { 471, "23.4945230473188359063824606456" },
                    { 472, "23.5124564634243264172370716012" },
                    { 473, "23.5459690663422524848013184867" },
                    { 474, "23.5655824807517469954386118968" },
                    { 475, "23.5752384349242049175097175806" },
                    { 476, "23.6008211943925307311698457134" },
                    { 477, "23.6093860038742202192008170011" },
                    { 478, "23.6222307193022452833862815561" },
                    { 479, "23.6335579623162838547322040025" },
                    { 480, "23.6420658520038198439271964369" },
                    { 481, "23.6456411954951575092334401966" },
                    { 482, "23.6986530781166988409811497192" },
                    { 483, "23.7213080851219457765613089416" },
                    { 484, "23.7409688210474042259494849484" },
                    { 485, "23.7647024288499260446009072982" },
                    { 486, "23.7834599134447208513690827190" },
                    { 487, "23.8065496086542018005549446549" },
                    { 488, "23.8356877880370879459253275588" },
                    { 489, "23.8485474695097388958153727600" },
                    { 490, "23.8692540970694492172585447016" },
                    { 491, "23.8969226171987950296106034661" },
                    { 492, "23.9237643408473873040179619093" },
                    { 493, "23.9422479268902459767984675288" },
                    { 494, "23.9668283597992158347427123468" },
                    { 495, "23.9899940511917298174811734665" },
                    { 496, "24.0126644003621981958977141031" },
                    { 497, "24.0488133387586027391096967781" },
                    { 498, "24.0599489391854506549913197957" },
                    { 499, "24.0625203627576969678460739416" },
                    { 500, "24.1329376239926108417344388443" },
                    { 501, "24.1807400654298023755705971991" },
                    { 502, "24.2165121488672104983892162353" },
                    { 503, "24.2398207443015577120047513869" },
                    { 504, "24.2711124340574203838332295125" },
                    { 505, "24.2933415273044007277778944509" },
                    { 506, "24.3252058117389450954336449239" },
                    { 507, "24.3570322874041805162511459299" },
                    { 508, "24.3823016449665412526656199489" },
                    { 509, "24.4106054973805802024694063779" },
                    { 510, "24.4365629292220817666529603617" },
                    { 511, "24.4539029278951548612518557420" },
                    { 512, "24.4697620294931243416864079018" },
                    { 513, "24.4944643498018747063237497170" },
                    { 514, "24.5189320232074248240415731044" },
                    { 515, "24.5352933430597846599864189531" },
                    { 516, "24.5558390873151238699223109098" },
                    { 517, "24.5806822590212222723713968189" },
                    { 518, "24.6013284675902076104093070447" },
                    { 519, "24.6245394790500953349522630867" },
                    { 520, "24.6609522830807871055495835464" },
                    { 521, "24.6810763560039066671453986763" },
                    { 522, "24.7012885845961014680455380807" },
                    { 523, "24.7194310850743853480060712451" },
                    { 524, "24.7352911011330708751615886788" },
                    { 525, "24.7498433822045172748232953464" },
                    { 526, "24.7624704643856224116414954567" },
                    { 527, "24.7822223416262900725886662109" },
                    { 528, "24.7937967284345832966421089874" },
                    { 529, "24.8300327048254044986217076623" },
                    { 530, "24.8482376878357396761711902476" },
                    { 531, "24.8709219627973013113771629005" },
                    { 532, "24.9009207176042486912432244769" },
                    { 533, "24.9255658760956514052122515605" },
                    { 534, "24.9449730884359521303901558520" },
                    { 535, "24.9719132434158578207531057357" },
                    { 536, "24.9966484684044296038824286457" },
                    { 537, "25.0180275827897994464292240505" },
                    { 538, "25.0385768914632580091158618829" },
                    { 539, "25.0666591244776773257059321577" },
                    { 540, "25.0884399378123240532356873454" },
                    { 541, "25.1177583473543259863376778785" },
                    { 542, "25.1445972357170063779442361267" },
                    { 543, "25.1632207529013896351103373246" },
                    { 544, "25.1916206619250060506471473382" },
                    { 545, "25.2258694641513494883982233724" },
                    { 546, "25.2441206124629800672996988615" },
                    { 547, "25.2486649453201275081930068970" },
                    { 548, "25.2948329974710914037663558773" },
                    { 549, "25.3183804731671505223171593530" },
                    { 550, "25.3384484708889589380813831888" },
                    { 551, "25.3683955697157560802179217371" },
                    { 552, "25.3828010180669254935089711279" },
                    { 553, "25.3943412017651602657501061851" },
                    { 554, "25.4090039972852424254193685963" },
                    { 555, "25.4201536150176136366010953082" },
                    { 556, "25.4451647443930777452864374396" },
                    { 557, "25.4701220261795010287506911613" },
                    { 558, "25.4921801882204151912929647083" },
                    { 559, "25.5012351522095316035318226822" },
                    { 560, "25.5167889934489480272489148284" },
                    { 561, "25.5306727703978426135673988907" },
                    { 562, "25.5435691432444520314541673235" },
                    { 563, "25.5664867850483447614008534661" },
                    { 564, "25.5734211782773178644280999157" },
                    { 565, "25.5758063104037468809478361522" },
                    { 566, "25.6192594550679299885987966945" },
                    { 567, "25.6503086587966476780441691551" },
                    { 568, "25.6768452576273807546826807119" },
                    { 569, "25.7134725983211149240200339411" },
                    { 570, "25.7224085765957983866062710256" },
                    { 571, "25.7430223439437905662747609185" },
                    { 572, "25.7801975440302437016826260948" },
                    { 573, "25.7945816754990929882053273050" },
                    { 574, "25.8103703921983955669970721747" },
                    { 575, "25.8291541052868682204399670978" },
                    { 576, "25.8746289610860337353494393627" },
                    { 577, "25.9029440025090509828642500431" },
                    { 578, "25.9195920293382167956362981713" },
                    { 579, "25.9385046904984748061020777189" },
                    { 580, "25.9623218515567230974952290473" },
                    { 581, "25.9980209649927703821077581933" },
                    { 582, "26.0156105291208186088716326596" },
                    { 583, "26.0243610543292019274659153579" },
                    { 584, "26.0818996705579915149111016955" },
                    { 585, "26.1026793646400947275530876693" },
                    { 586, "26.1495585383177514272357260722" },
                    { 587, "26.1636908777248805931898419424" },
                    { 588, "26.1828160121121344454490621354" },
                    { 589, "26.1990378164634794339084599702" },
                    { 590, "26.2105443770191322486989316465" },
                    { 591, "26.2324777614409669420662355677" },
                    { 592, "26.2500189238322543246352828485" },
                    { 593, "26.2754154991033243086111503742" },
                    { 594, "26.2943557992248926310121635325" },
                    { 595, "26.3131061328227168476290746618" },
                    { 596, "26.3247258232372285023160286849" },
                    { 597, "26.3542748537687084672744937123" },
                    { 598, "26.3836333563466547397672407489" },
                    { 599, "26.4046682888208569112039458791" },
                    { 600, "26.4274162693891433789707319884" },
                    { 601, "26.4525466204249586117857864490" },
                    { 602, "26.4743420415990358702709012017" },
                    { 603, "26.4920731812813439902951847192" },
                    { 604, "26.5098087014393538450193927138" },
                    { 605, "26.5365274698800015240927873479" },
                    { 606, "26.5607287980500893580628253625" },
                    { 607, "26.5803277298175576448104371556" },
                    { 608, "26.6012529572177251941292419868" },
                    { 609, "26.6219631079643402120495335261" },
                    { 610, "26.6310600018072019390685730295" },
                    { 611, "26.6521854818608615719003783903" },
                    { 612, "26.6756463667558536967977337872" },
                    { 613, "26.6970192350448996034108069907" },
                    { 614, "26.7165913826162385541553249779" },
                    { 615, "26.7352094392433978073812768130" },
                    { 616, "26.7645803494687920749373595580" },
                    { 617, "26.7884888700522518365938354959" },
                    { 618, "26.8135181841422275691798411862" },
                    { 619, "26.8362530691304034675424761778" },
                    { 620, "26.8618811252052567978340816951" },
                    { 621, "26.8841350218889171674701748916" },
                    { 622, "26.9021657622003828111990866723" },
                    { 623, "26.9257589653206946237198434549" },
                    { 624, "26.9498425666949751018043784562" },
                    { 625, "26.9715719130780543731973748597" },
                    { 626, "26.9917726201788099498796808967" },
                    { 627, "27.0005703520997943169126960616" },
                    { 628, "27.0142430864991893397307251021" },
                    { 629, "27.0298211002198180666362536045" },
                    { 630, "27.0340036487327463546231520055" },
                    { 631, "27.0362892833291336659684410261" },
                    { 632, "27.0814019044962451850715239043" },
                    { 633, "27.0942362046963062453704040852" },
                    { 634, "27.1112780249225398138142336535" },
                    { 635, "27.1413702235069851401307961467" },
                    { 636, "27.1548333114034253376167307360" },
                    { 637, "27.1591536811231914624995153183" },
                    { 638, "27.2070003382919577774228838297" },
                    { 639, "27.2132047341234682498893938093" },
                    { 640, "27.2419589705633876691783186357" },
                    { 641, "27.2674943591532834564227449793" },
                    { 642, "27.2844199577537943319225626404" },
                    { 643, "27.3064573906073232515571316601" },
                    { 644, "27.3376566064654684522441324090" },
                    { 645, "27.3593515694826226016140520262" },
                    { 646, "27.3736261171576086636697384831" },
                    { 647, "27.3863108008002295571538978984" },
                    { 648, "27.4042657909536718065835119609" },
                    { 649, "27.4064561509640759098413576516" },
                    { 650, "27.4458279070025562062991280264" },
                    { 651, "27.4652288765222072054491696760" },
                    { 652, "27.4982602405534169991357832398" },
                    { 653, "27.5258468955389835088129907716" },
                    { 654, "27.5470853402753789710821483594" },
                    { 655, "27.5692732044236055128266878337" },
                    { 656, "27.5774627266066415024563917053" },
                    { 657, "27.6039599115175259531365643767" },
                    { 658, "27.6312852127025316816693816115" },
                    { 659, "27.6486955860418691597718940163" },
                    { 660, "27.6680891671089174043910926845" },
                    { 661, "27.6905549869786971811170657526" },
                    { 662, "27.7234436040256602020735773875" },
                    { 663, "27.7480911922816179913882884273" },
                    { 664, "27.7737278990580628006533117097" },
                    { 665, "27.7941224889032508493538911826" },
                    { 666, "27.8222921562451196994670207972" },
                    { 667, "27.8574805251950056436417648395" },
                    { 668, "27.8710147221749476832931680670" },
                    { 669, "27.8866609961519115955958196582" },
                    { 670, "27.9068676438932940764302788916" },
                    { 671, "27.9201509487207631461744064174" },
                    { 672, "27.9417729524893817679033037461" },
                    { 673, "27.9596821228863146278995769855" },
                    { 674, "27.9853832196552225600092497078" },
                    { 675, "27.9978204160625900619144910518" },
                    { 676, "28.0076813624366630688451632476" },
                    { 677, "28.0279735047920469112705432657" },
                    { 678, "28.0545929508878908364959646138" },
                    { 679, "28.0768582951626392517590608633" },
                    { 680, "28.0951980574936288665781345916" },
                    { 681, "28.1133729031114499555079445394" },
                    { 682, "28.1365758386295095557164067179" },
                    { 683, "28.1488583027495775414531850736" },
                    { 684, "28.1533390043797220270651386750" },
                    { 685, "28.1789920563549965890552576282" },
                    { 686, "28.1877422416962426155315801399" },
                    { 687, "28.2049638371837759680657477096" },
                    { 688, "28.2249821330792821827675330123" },
                    { 689, "28.2378970838750700982250113137" },
                    { 690, "28.2458655421937462904586269795" },
                    { 691, "28.2975103000525227368280004975" },
                    { 692, "28.3200628126586850247523470225" },
                    { 693, "28.3416781163463646126579737697" },
                    { 694, "28.3586002830834043894157860678" },
                    { 695, "28.3822791346367664037278217346" },
                    { 696, "28.4043744075627237076019330668" },
                    { 697, "28.4244227062071936819724774262" },
                    { 698, "28.4545792917844568099323023356" },
                    { 699, "28.4764372016993282087837494235" },
                    { 700, "28.4958443163571755189230610352" },
                    { 701, "28.5192416150378299094458328529" },
                    { 702, "28.5431432056362962783521701349" },
                    { 703, "28.5662483943687031135908993347" },
                    { 704, "28.5856305325560953407870082519" },
                    { 705, "28.6082159067075582365360552976" },
                    { 706, "28.6278932536301223046574302550" },
                    { 707, "28.6520425877249518700148193893" },
                    { 708, "28.6725557146478445896801070205" },
                    { 709, "28.6891607648616296667006054882" },
                    { 710, "28.7110433153314685304376448176" },
                    { 711, "28.7315119242671941102382809235" },
                    { 712, "28.7492504971030835088915870676" },
                    { 713, "28.7590112287856035580460140423" },
                    { 714, "28.7762600654699639769817962978" },
                    { 715, "28.7863720804457314898935253457" },
                    { 716, "28.8056130183203704186147781116" },
                    { 717, "28.8168520335583012144222817077" },
                    { 718, "28.8270142272570056088361148917" },
                    { 719, "28.8492182452682612060976650479" },
                    { 720, "28.8599089373682472094539098542" },
                    { 721, "28.8681570386896766632821548665" },
                    { 722, "28.9023210926307842147433731217" },
                    { 723, "28.9086701010402448230105620811" },
                    { 724, "28.9251738759664986682647632105" },
                    { 725, "28.9558113728057473067060329657" },
                    { 726, "28.9662937285196738124515814152" },
                    { 727, "28.9624848399374774872859020126" },
                    { 728, "29.0121268585524392382029813781" },
                    { 729, "29.0285583848242625242680449778" },
                    { 730, "29.0380889370319866098252339585" },
                    { 731, "29.0603518172063188450851414531" },
                    { 732, "29.0851350010831955904449864445" },
                    { 733, "29.1097891627335645003830605760" },
                    { 734, "29.1250043241305469137242295584" },
                    { 735, "29.1380424521434181039794872466" },
                    { 736, "29.1675742215249639179338733834" },
                    { 737, "29.1885579872518040737950446537" },
                    { 738, "29.2040084780785347294693065827" },
                    { 739, "29.2258572648739216486369053509" },
                    { 740, "29.2501613747151442205487871809" },
                    { 741, "29.2673369350722693469491581552" },
                    { 742, "29.2823157363001067649527618558" },
                    { 743, "29.2989651470746445432591367263" },
                    { 744, "29.3158656214237890893822138089" },
                    { 745, "29.3165114429166209423802301283" },
                    { 746, "29.3724489515652994640925075622" },
                    { 747, "29.3968722735569571350288900970" },
                    { 748, "29.4222430273212917765702914449" },
                    { 749, "29.4424221375897529449206163810" },
                    { 750, "29.4806882503111679892052861385" },
                    { 751, "29.4964576789056784316816632321" },
                    { 752, "29.5245024038969654935712464434" },
                    { 753, "29.5441235520890116666805475023" },
                    { 754, "29.5657891671033538410694891220" },
                    { 755, "29.5864164492056276811777207235" },
                    { 756, "29.6027051081636464408019836395" },
                    { 757, "29.6128793133540721525279783724" },
                    { 758, "29.6326524988214221798512997315" },
                    { 759, "29.6457138234124885082729317900" },
                    { 760, "29.6611069657374067261766030699" },
                    { 761, "29.6841899581569478650102204420" },
                    { 762, "29.6989969223336180080903573808" },
                    { 763, "29.7207712331155431261926530389" },
                    { 764, "29.7349505494983801856589681803" },
                    { 765, "29.7550861244990579614353409137" },
                    { 766, "29.7694949528730671237563470491" },
                    { 767, "29.7933367339864517235064832454" },
                    { 768, "29.8085211574456187358887175495" },
                    { 769, "29.8292293662488393177974365984" },
                    { 770, "29.8480812040766549041108647305" },
                    { 771, "29.8609149052563033435676629936" },
                    { 772, "29.8794859425504894785777903120" },
                    { 773, "29.9017801561344785862563512870" },
                    { 774, "29.9175478793090515925868127640" },
                    { 775, "29.9354874346581121145148681662" },
                    { 776, "29.9566023830354543095011478305" },
                    { 777, "29.9740843589135799099765582932" },
                    { 778, "29.9893439763493033082941687521" },
                    { 779, "30.0018429601249077097740544698" },
                    { 780, "30.0188056551262564211790875876" },
                    { 781, "30.0278742023568650777760447048" },
                    { 782, "30.0477702989276466388342003724" },
                    { 783, "30.0491348987500699495686367703" },
                    { 784, "30.0919320289996014064345527781" },
                    { 785, "30.1042352916740245503772138294" },
                    { 786, "30.1225967809479350595827012110" },
                    { 787, "30.1467490155456104011733521489" },
                    { 788, "30.1667526330326772632775245646" },
                    { 789, "30.1837481364482378897167902980" },
                    { 790, "30.2195970061467653947327049043" },
                    { 791, "30.2362177421487998100428227126" },
                    { 792, "30.2662157277843033762577533883" },
                    { 793, "30.2813651037617110291807349230" },
                    { 794, "30.3026963886855032362758525294" },
                    { 795, "30.3277052163436264053603153412" },
                    { 796, "30.3480735600737945164663948976" },
                    { 797, "30.3755692236334159460164947847" },
                    { 798, "30.3907284430200917530818747157" },
                    { 799, "30.4091353795170896846709326604" },
                    { 800, "30.4212133790244663457995052330" },
                    { 801, "30.4461831645418668113218637169" },
                    { 802, "30.4601397202957138619957644068" },
                    { 803, "30.4931873665725104295142081947" },
                    { 804, "30.5017767784414145273034968041" },
                    { 805, "30.5049262713682813351761666677" },
                    { 806, "30.5444108844524030298185316617" },
                    { 807, "30.5524220036776543266220693909" },
                    { 808, "30.5669950558097350199123880220" },
                    { 809, "30.5905834474396826294999376146" },
                    { 810, "30.6017659657188219285933677272" },
                    { 811, "30.6090661326228963692633980805" },
                    { 812, "30.6358467947947455405237624768" },
                    { 813, "30.6455178246447284265481378763" },
                    { 814, "30.6594592877268064042770806701" },
                    { 815, "30.6849293150759415985834191517" },
                    { 816, "30.6938205445249170348992523393" },
                    { 817, "30.6926154325406871241443689967" },
                    { 818, "30.7297559207440365967357800742" },
                    { 819, "30.7378271776272576991186697747" },
                    { 820, "30.7666908825604079762913483686" },
                    { 821, "30.7873131713294392697539563468" },
                    { 822, "30.7916066698287244158250002136" },
                    { 823, "30.7951925937737593219907116679" },
                    { 824, "30.8292753643272257962635811509" },
                    { 825, "30.8530056898376862985781425959" },
                    { 826, "30.8680517143991548911681651023" },
                    { 827, "30.8886784782505851226576375926" },
                    { 828, "30.9026164658523736833738953680" },
                    { 829, "30.9212022728358641653268508534" },
                    { 830, "30.9418117602312143464218009272" },
                    { 831, "30.9517925548082668086731173836" },
                    { 832, "30.9765210495504041232647129144" },
                    { 833, "30.9905625418233911778247163601" },
                    { 834, "31.0104821173715360973445300433" },
                    { 835, "31.0296044634929077848756518663" },
                    { 836, "31.0558255275259170986647209784" },
                    { 837, "31.0717887731301899389530924688" },
                    { 838, "31.0872412118569451185637524791" },
                    { 839, "31.1101959516369744586059088996" },
                    { 840, "31.1208576444560318643573359245" },
                    { 841, "31.1410744923090578467915524408" },
                    { 842, "31.1572615437442153363484880073" },
                    { 843, "31.1769492236529264436833022104" },
                    { 844, "31.1925630669020028217658691736" },
                    { 845, "31.2080936232517002868898620434" },
                    { 846, "31.2210952934860488264312358439" },
                    { 847, "31.2211037209889741236748305076" },
                    { 848, "31.3120672717599189860140532181" },
                    { 849, "31.3275671751746272909199477857" },
                    { 850, "31.3552353388265156838386762268" },
                    { 851, "31.3720132077522508628666379749" },
                    { 852, "31.3852013444079243290290130004" },
                    { 853, "31.4040319195133340919253010450" },
                    { 854, "31.4141287822430719361100405727" },
                    { 855, "31.4308255912528343231568059389" },
                    { 856, "31.4404613223830537979704912527" },
                    { 857, "31.4617134169693767906289681961" },
                    { 858, "31.4783922437138844504125585197" },
                    { 859, "31.4958614949462995101822197171" },
                    { 860, "31.5114350782811263284422027535" },
                    { 861, "31.5340220771833295407362018307" },
                    { 862, "31.5563343468030628341317057740" },
                    { 863, "31.5731795034489153811687308971" },
                    { 864, "31.5879451061186977322037276147" },
                    { 865, "31.6107412073046813365539487293" },
                    { 866, "31.6308337857448657337453750779" },
                    { 867, "31.6435101274550983841800092721" },
                    { 868, "31.6572636119899135425475393636" },
                    { 869, "31.6694693971215179699680689395" },
                    { 870, "31.6807723725648392608489687084" },
                    { 871, "31.7006930730268304987498098814" },
                    { 872, "31.7129332495546915824153342271" },
                    { 873, "31.7297370844232446190844783765" },
                    { 874, "31.7523669701454039966173979890" },
                    { 875, "31.7733078986934137649248374782" },
                    { 876, "31.7902883672681908024053317344" },
                    { 877, "31.8055541387928158925024935585" },
                    { 878, "31.8245891828400819932554663535" },
                    { 879, "31.8366948955298094438621510301" },
                    { 880, "31.8536138755291003810088753121" },
                    { 881, "31.8739959816825768495599773284" },
                    { 882, "31.8888030786389261564851927707" },
                    { 883, "31.9093360007619518254033914986" },
                    { 884, "31.9359691760533474875592993487" },
                    { 885, "31.9518200778913515861573441371" },
                    { 886, "31.9710331442641989371645984743" },
                    { 887, "31.9933865665997394285830645795" },
                    { 888, "32.0091101898708936460257556336" },
                    { 889, "32.0304755579890607834659950170" },
                    { 890, "32.0482429713827293376938425615" },
                    { 891, "32.0686071271829608200065996924" },
                    { 892, "32.0884743995734040359427446281" },
                    { 893, "32.1096398200388593064035753544" },
                    { 894, "32.1328183897497531672035881909" },
                    { 895, "32.1491741691246028970054657172" },
                    { 896, "32.1661131216087961157269703466" },
                    { 897, "32.1841493303880540556602499368" },
                    { 898, "32.2064695638269673980288124915" },
                    { 899, "32.2281486009773466452449042119" },
                    { 900, "32.2330843545238362312448706558" },
                    { 901, "32.2344619984929240403474409448" },
                    { 902, "32.2635517825402415204213014971" },
                    { 903, "32.2727057448498440084432568391" },
                    { 904, "32.2892954316440709138814352001" },
                    { 905, "32.2999208057064488487582945085" },
                    { 906, "32.3065651824461800428109402544" },
                    { 907, "32.3191814763495696318966659734" },
                    { 908, "32.3409258592265298058301330214" },
                    { 909, "32.3587155436579031706629001283" },
                    { 910, "32.3661258161116988834633933371" },
                    { 911, "32.3720611667772066014359648887" },
                    { 912, "32.3814559638290902803369499497" },
                    { 913, "32.3827203133925270006783625506" },
                    { 914, "32.4730563805150412060161196686" },
                    { 915, "32.4859236415855603811887762343" },
                    { 916, "32.4958354686927490469064402225" },
                    { 917, "32.5051768529001927598657722142" },
                    { 918, "32.5205141154348780022840004300" },
                    { 919, "32.5389223186653623911268270410" },
                    { 920, "32.5489524356817055231863042917" },
                    { 921, "32.5655103229494305023000670803" },
                    { 922, "32.5797532503453581232924846447" },
                    { 923, "32.6092522620913172039537843702" },
                    { 924, "32.6279909131688188400609841091" },
                    { 925, "32.6294948248472913475887639464" },
                    { 926, "32.6557767513170541277629246037" },
                    { 927, "32.6663877136493449294418288316" },
                    { 928, "32.6774513387461675547331706798" },
                    { 929, "32.6880067398001722083311745739" },
                    { 930, "32.7013004786016335873379954392" },
                    { 931, "32.7079808896748682474754998971" },
                    { 932, "32.7484453460844760495116948268" },
                    { 933, "32.7646719335695341041998446591" },
                    { 934, "32.7840495983029925817189119001" },
                    { 935, "32.8070648809069951295073377762" },
                    { 936, "32.8333552420945667889682945436" },
                    { 937, "32.8591570358245405465193021754" },
                    { 938, "32.8825803503970115616689482294" },
                    { 939, "32.8940008406010478741104737834" },
                    { 940, "32.9143189847552516189913536564" },
                    { 941, "32.9434075198423933316431691429" },
                    { 942, "32.9685500215158850646183073916" },
                    { 943, "32.9782723248749523222247193040" },
                    { 944, "33.0210437064916647771640334111" },
                    { 945, "33.0363462137228055101921153915" },
                    { 946, "33.0554707114653956261123132959" },
                    { 947, "33.0817203239691405723586984833" },
                    { 948, "33.0936488841128970117873544603" },
                    { 949, "33.1075988331969524798019539806" },
                    { 950, "33.1232153862351685631701235750" },
                    { 951, "33.1372783987374740687550863136" },
                    { 952, "33.1522455006179701069259049001" },
                    { 953, "33.1723624657858283043045780886" },
                    { 954, "33.1820308809107659392802845276" },
                    { 955, "33.1768263546616463566509696089" },
                    { 956, "33.2140000040885562356597949424" },
                    { 957, "33.2285451999356157746301753346" },
                    { 958, "33.2488011423872461285855087641" },
                    { 959, "33.2532288847207167956791208555" },
                    { 960, "33.2729042511266574119958780507" },
                    { 961, "33.2970650432216334602023006117" },
                    { 962, "33.3148950267492844219118748531" },
                    { 963, "33.3297121721979918353516748234" },
                    { 964, "33.3393817194931839609151843292" },
                    { 965, "33.3574979691460613617069117985" },
                    { 966, "33.3737485259349588772838305517" },
                    { 967, "33.3981139950897586566998330076" },
                    { 968, "33.4099754989817472101904651191" },
                    { 969, "33.4216538781194497987642042606" },
                    { 970, "33.4357278370650287928688563897" },
                    { 971, "33.4520048869229623144876090576" },
                    { 972, "33.4677880694159116661676928896" },
                    { 973, "33.4882499612151154693897006323" },
                    { 974, "33.4956499551034340131167945596" },
                    { 975, "33.5045685602179398543574343174" },
                    { 976, "33.5245095352026505170480557137" },
                    { 977, "33.5337256997541446009656084640" },
                    { 978, "33.5554615593515628726229612682" },
                    { 979, "33.5729356105566128017355780498" },
                    { 980, "33.6049866471138647222395271233" },
                    { 981, "33.6211466227800530420680067608" },
                    { 982, "33.6407225942109193367552408238" },
                    { 983, "33.6537782734427556259481508532" },
                    { 984, "33.6690406655274307104209737740" },
                    { 985, "33.6782739498164634392059552993" },
                    { 986, "33.7058618623935155476937937914" },
                    { 987, "33.7247563877827611919706227593" },
                    { 988, "33.7425813961289669062547140887" },
                    { 989, "33.7649165837987229938461604146" },
                    { 990, "33.7831428325618425436419076708" },
                    { 991, "33.8017160525788135954620694464" },
                    { 992, "33.8138294111140311278169513545" },
                    { 993, "33.8358186987820745625070280979" },
                    { 994, "33.8542997162653579236055536317" },
                    { 995, "33.8765957592034299917415227093" },
                    { 996, "33.9029324548964350034443457512" },
                    { 997, "33.9294471082718647418733246601" },
                    { 998, "33.9354688011931550685373948706" },
                    { 999, "33.9481037189555655193855083762" },
                    { 1000, "33.9571409146834100529592411859" },
                    { 1001, "33.9795859825641079910187462988" },
                    { 1002, "33.9812105133812971446293926109" },
                    { 1003, "33.9817159723231625314539529686" },
                    { 1004, "34.0319918908423235638217021729" },
                    { 1005, "34.0443116705130450622393991890" },
                    { 1006, "34.0501168743681326473797805219" },
                    { 1007, "34.0549006938801426913374594096" },
                    { 1008, "34.0689598139933702173285431394" },
                    { 1009, "34.0775690183393323259767895036" },
                    { 1010, "34.1030840784383300312479597211" },
                    { 1011, "34.1137719453985245386664202242" },
                    { 1012, "34.1185430261761546286138397492" },
                    { 1013, "34.1365155682721246891647912628" },
                    { 1014, "34.1439823508836890807619287111" },
                    { 1015, "34.1398585411489350041166269682" },
                    { 1016, "34.2053811368709200003675088744" },
                    { 1017, "34.2108973942918523744651153067" },
                    { 1018, "34.2334212756640155519162695827" },
                    { 1019, "34.2415554897334235126472653820" },
                    { 1020, "34.2474383848550271598826065488" },
                    { 1021, "34.2610184862516839183273983936" },
                    { 1022, "34.2722386663553606302018670018" },
                    { 1023, "34.2916357842259218601090810648" },
                    { 1024, "34.3093489579418850189211626591" },
                    { 1025, "34.3188100720378620928836978720" },
                    { 1026, "34.3270205229675019493551499704" },
                    { 1027, "34.3287026276790249671738748397" },
                    { 1028, "34.4195729786490289432580093104" },
                    { 1029, "34.4328165436926456398112502951" },
                    { 1030, "34.4485831270713588194932886400" },
                    { 1031, "34.4680997189030529981646690969" },
                    { 1032, "34.4814563378460017752411423651" },
                    { 1033, "34.4974182509484565742539995284" },
                    { 1034, "34.5134756147194997031008581668" },
                    { 1035, "34.5215971616808752218249750148" },
                    { 1036, "34.5352512373865190618729974388" },
                    { 1037, "34.5422796239085884326921851054" },
                    { 1038, "34.5494578104345142675935333008" },
                    { 1039, "34.5523599446991287906879272036" },
                    { 1040, "34.5989489588798721412511848350" },
                    { 1041, "34.6181058576568186894774137368" },
                    { 1042, "34.6262055806694213474682614323" },
                    { 1043, "34.6363969862246373585944567820" },
                    { 1044, "34.6386575335623917131434687140" },
                    { 1045, "34.6386021441890345279297002145" },
                    { 1046, "34.7560459885160182420240531213" },
                    { 1047, "34.7609765853879688788135743102" },
                    { 1048, "34.7765627877062854624342179752" },
                    { 1049, "34.8010473294263842015861228143" },
                    { 1050, "34.8110693990113592635971882566" },
                    { 1051, "34.8189521162456099334226348696" },
                    { 1052, "34.8273259407833270012006390360" },
                    { 1053, "34.8356829744263333330261625715" },
                    { 1054, "34.8583677270890809702974532817" },
                    { 1055, "34.8709651103977027365128614439" },
                    { 1056, "34.8862191499508978308638553253" },
                    { 1057, "34.8982843073580183127253195478" },
                    { 1058, "34.9271004431190824162152049738" },
                    { 1059, "34.9372500624557296574389288942" },
                    { 1060, "34.9447864213092690753067745342" },
                    { 1061, "34.9532496175714853475476437315" },
                    { 1062, "34.9694387950923054525182073919" },
                    { 1063, "34.9863543873811993664681020124" },
                    { 1064, "35.0019909394718136575470383865" },
                    { 1065, "35.0200896130965059587344636975" },
                    { 1066, "35.0389344887155931660777507848" },
                    { 1067, "35.0590033259895147671910432549" },
                    { 1068, "35.0676613805278211472113424950" },
                    { 1069, "35.0818381589847300048160918639" },
                    { 1070, "35.1071687950238102518151753967" },
                    { 1071, "35.1344686339943495849719110271" },
                    { 1072, "35.1524225692134706219331323225" },
                    { 1073, "35.1619091288082743708521093744" },
                    { 1074, "35.1700017954071603177245645206" },
                    { 1075, "35.1772468934955818625060461738" },
                    { 1076, "35.1823117492187236859524275982" },
                    { 1077, "35.1860797827885527202252431255" },
                    { 1078, "35.1961021811084678837779996932" },
                    { 1079, "35.2199068634375865743922160196" },
                    { 1080, "35.2370063166879200696972593809" },
                    { 1081, "35.2577545181992626253073550264" },
                    { 1082, "35.2736890506433866587085616654" },
                    { 1083, "35.2923148697490140865966345948" },
                    { 1084, "35.3259793965730419449398262069" },
                    { 1085, "35.3281447753755891587464079599" },
                    { 1086, "35.3625143518438208092767640046" },
                    { 1087, "35.3752636791376522701095488605" },
                    { 1088, "35.3931997022050874705055285137" },
                    { 1089, "35.3993032208276390227642478865" },
                    { 1090, "35.3867090472512902912665995881" },
                    { 1091, "35.3935021969396100152805639963" },
                    { 1092, "35.4086420325674920958085342671" },
                    { 1093, "35.4505299678878032656811661985" },
                    { 1094, "35.4723666332015683552709336067" },
                    { 1095, "35.5023882468857678970765605707" },
                    { 1096, "35.5128075973303901366887424903" },
                    { 1097, "35.5337252107043195343963456213" },
                    { 1098, "35.5494382762749528954696449219" },
                    { 1099, "35.5737557400784222149330704504" },
                    { 1100, "35.6161932967842715234035677045" },
                    { 1101, "35.6130188574392963975220979108" },
                    { 1102, "35.6369703023006023544394241472" },
                    { 1103, "35.6495160569604923165182222498" },
                    { 1104, "35.6690060769902842223125427220" },
                    { 1105, "35.6887051811176339468423303584" },
                    { 1106, "35.6952515297464246692926988995" },
                    { 1107, "35.7034189786528788790955280570" },
                    { 1108, "35.7174841608823416360417753339" },
                    { 1109, "35.7307918648039021352161774647" },
                    { 1110, "35.7456562518067545815370962299" },
                    { 1111, "35.7162697616363707890520739512" },
                    { 1112, "35.7730662523323615457644284907" },
                    { 1113, "35.7895803616783252398708769508" },
                    { 1114, "35.8100179797402832763755949681" },
                    { 1115, "35.8177003205423402266405738037" },
                    { 1116, "35.8285870613315527470995472362" },
                    { 1117, "35.8383583820566360447650683231" },
                    { 1118, "35.8629960794566028876503990652" },
                    { 1119, "35.8640029804475100065906831401" },
                    { 1120, "35.8687168422259588653830918739" },
                    { 1121, "35.8754422665461972646639327723" },
                    { 1122, "35.8851241460609528649395326061" },
                    { 1123, "35.8949628570676399326716766604" },
                    { 1124, "35.9455737896928461796858355428" },
                    { 1125, "35.9536053424135848316496794093" },
                    { 1126, "35.9750461333018541247941235945" },
                    { 1127, "35.9806704634387155758648977168" },
                    { 1128, "35.9860911284722607899505127062" },
                    { 1129, "35.9970563838053953786601122521" },
                    { 1130, "36.0246919018264775904771099715" },
                    { 1131, "36.0348177821826753003068463541" },
                    { 1132, "36.0464126706870005751935842234" },
                    { 1133, "36.0589623928001534416662326262" },
                    { 1134, "36.0872789198730550520556430130" },
                    { 1135, "36.0526517390533896320099063949" },
                    { 1136, "36.1163298133146462435300095507" },
                    { 1137, "36.1308315341258610653996193688" },
                    { 1138, "36.1552813604829221303266120172" },
                    { 1139, "36.1696380915368212384511328204" },
                    { 1140, "36.1886471476564274707486941574" },
                    { 1141, "36.2138921552108471389148146448" },
                    { 1142, "36.2319810508749524284154632616" },
                    { 1143, "36.2522107937682077839738503134" },
                    { 1144, "36.2581262294714830257542573365" },
                    { 1145, "36.2893220791787794419303780292" },
                    { 1146, "36.2974711929223989229563402297" },
                    { 1147, "36.2920834775756050381336902080" },
                    { 1148, "36.3487784107095547357611509538" },
                    { 1149, "36.3723592606833041406767741403" },
                    { 1150, "36.3805112771936335036191137080" },
                    { 1151, "36.4038032941089945327512189037" },
                    { 1152, "36.4159984501093271536071356198" },
                    { 1153, "36.4280022235109339946928286313" },
                    { 1154, "36.4312525633769790199645084821" },
                    { 1155, "36.4661562212327863580856695239" },
                    { 1156, "36.4700181479833415114205684840" },
                    { 1157, "36.4925287353948548958107281736" },
                    { 1158, "36.5110509871621697605147639848" },
                    { 1159, "36.4831089327297404091087483041" },
                    { 1160, "36.5413071486303975140739988262" },
                    { 1161, "36.5506662431891128594119005815" },
                    { 1162, "36.5586886294909793547444050314" },
                    { 1163, "36.5685420854420818042144085231" },
                    { 1164, "36.5839543149274366425773403409" },
                    { 1165, "36.5818326538704299279343486190" },
                    { 1166, "36.5951493166248449725420434017" },
                    { 1167, "36.6302190054597785945565943860" },
                    { 1168, "36.6414445904631062779468715509" },
                    { 1169, "36.6503439258011656704735772876" },
                    { 1170, "36.6617841620607092119473717438" },
                    { 1171, "36.6624618478116508740959069148" },
                    { 1172, "36.6743363748260708087050315107" },
                    { 1173, "36.7121089125496957317826234434" },
                    { 1174, "36.7197270409748181599271362271" },
                    { 1175, "36.7402106298128692227343390216" },
                    { 1176, "36.7488725236134387137108462075" },
                    { 1177, "36.7637737669144116154766061826" },
                    { 1178, "36.7773820223038540490560898424" },
                    { 1179, "36.7894176372033129253286898277" },
                    { 1180, "36.8119557539023016430648440987" },
                    { 1181, "36.8386415515649860779552987065" },
                    { 1182, "36.8742913696715672813975115223" },
                    { 1183, "36.8820354514716363293946341957" },
                    { 1184, "36.9073791814571843768729233920" },
                    { 1185, "36.9130849467692153955952483987" },
                    { 1186, "36.9348374509032280720885284367" },
                    { 1187, "36.9451318165958690642150777768" },
                    { 1188, "36.9744908638245750063954441459" },
                    { 1189, "36.9890116289886968120336077863" },
                    { 1190, "36.9763157496825322605226446496" },
                    { 1191, "36.9885115969985314238052068234" },
                    { 1192, "37.0068367507159953535047741847" },
                    { 1193, "37.0143893885514555679808240921" },
                    { 1194, "37.0377725268612167903265499951" },
                    { 1195, "37.0521793845238033499912075576" },
                    { 1196, "37.0636311755169912937516509696" },
                    { 1197, "37.0674855425891381021254779498" },
                    { 1198, "37.0840337849929664025860912049" },
                    { 1199, "37.0961024328850154077597971734" },
                    { 1200, "37.1121608415615410605736453794" },
                    { 1201, "37.1298418902047128428672990620" },
                    { 1202, "37.1481586871914298621970420932" },
                    { 1203, "37.1636664663049327126262015441" },
                    { 1204, "37.1877723230943642700203419957" },
                    { 1205, "37.2018764338300341608708658404" },
                    { 1206, "37.2319525462657685557897947258" },
                    { 1207, "37.2435132798459982470085906648" },
                    { 1208, "37.2610389289290546582712510205" },
                    { 1209, "37.2796879796979255207831240699" },
                    { 1210, "37.2690535478743807697380017252" },
                    { 1211, "37.3139624195914864538582783994" },
                    { 1212, "37.3086262044071989703500862687" },
                    { 1213, "37.3453643197631545533267753750" },
                    { 1214, "37.3731774518144641838651718799" },
                    { 1215, "37.3792281352456093189918755086" },
                    { 1216, "37.3928563018678533975628324344" },
                    { 1217, "37.4033969116818574781460214829" },
                    { 1218, "37.4132742750649837762460233605" },
                    { 1219, "37.4391143490040120231154605243" },
                    { 1220, "37.4456517482995990316526141612" },
                    { 1221, "37.4596567124304239501575106215" },
                    { 1222, "37.4728821589935691499767587087" },
                    { 1223, "37.4813568823513862829316904461" },
                    { 1224, "37.4937487724665611113654296810" },
                    { 1225, "37.4602974786400358550489666869" },
                    { 1226, "37.5253841074720412270943345769" },
                    { 1227, "37.5520416700714391138515654381" },
                    { 1228, "37.5567923446142433898998424454" },
                    { 1229, "37.5735885941050404513111885877" },
                    { 1230, "37.5890669321437208740380248185" },
                    { 1231, "37.5963252239118913037555399239" },
                    { 1232, "37.6091887706189351181053404148" },
                    { 1233, "37.6205818701358392362972591553" },
                    { 1234, "37.6349534943048147456181688993" },
                    { 1235, "37.6535557830211521399791042732" },
                    { 1236, "37.6655164182173836906651589601" },
                    { 1237, "37.6331533822341011474705321177" },
                    { 1238, "37.6999012795582465889437719957" },
                    { 1239, "37.7123544629994352295095193433" },
                    { 1240, "37.7312540710419525193992121804" },
                    { 1241, "37.7515533423950557268792047027" },
                    { 1242, "37.7670168385799130077949576808" },
                    { 1243, "37.7718329774571493692676403855" },
                    { 1244, "37.7845018047856067181523660578" },
                    { 1245, "37.8195342341972334238722474261" },
                    { 1246, "37.8437336574525908382267491485" },
                    { 1247, "37.8510361397311855407038836921" },
                    { 1248, "37.8775839987306724474448788006" },
                    { 1249, "37.8353710241608820642267355404" },
                    { 1250, "37.9225339139921838543009044011" },
                    { 1251, "37.9361595876933739128809582007" },
                    { 1252, "37.9545724436990582480486135953" },
                    { 1253, "37.9599354758695928253101544899" },
                    { 1254, "37.9884234661810526733037443487" },
                    { 1255, "37.9566193696053648997617442126" },
                    { 1256, "38.0126167105977513487672153425" },
                    { 1257, "38.0233276736241413929045645331" },
                    { 1258, "38.0389763547067384982145253377" },
                    { 1259, "38.0561598533779042033872199965" },
                    { 1260, "38.0654227613341312213435408318" },
                    { 1261, "37.9814712653519044536333499421" },
                    { 1262, "38.0942403701802085386735579529" },
                    { 1263, "38.1008633939795130314998642719" },
                    { 1264, "38.1209327213494207874999954919" },
                    { 1265, "38.1269159347135385932911515475" },
                    { 1266, "38.1468196256981522032694687153" },
                    { 1267, "38.1635859970350083454651937784" },
                    { 1268, "38.1840491889494971746601893329" },
                    { 1269, "38.1967794549864955301399918564" },
                    { 1270, "38.2053226831178742363111744024" },
                    { 1271, "38.2139038278124529911856621930" },
                    { 1272, "38.2215133356371495706217568452" },
                    { 1273, "38.2234753940891031619220342476" },
                    { 1274, "38.2421810183940906076840329249" },
                    { 1275, "38.2727680683645522679162375381" },
                    { 1276, "38.2816372573538892550726689478" },
                    { 1277, "38.2918604932509966037978488594" },
                    { 1278, "38.3011687656143499825839207235" },
                    { 1279, "38.3190226246285742962126895048" },
                    { 1280, "38.3353922799071620519264291884" },
                    { 1281, "38.3432049303794638588816774210" },
                    { 1282, "38.3523068771580860033261609553" },
                    { 1283, "38.3772193208066325532393390253" },
                    { 1284, "38.4044429317220142389969576895" },
                    { 1285, "38.3758261701755482485063204732" },
                    { 1286, "38.4248020091612913022614276169" },
                    { 1287, "38.4391714547751531340337810561" },
                    { 1288, "38.4590691185414446402059138263" },
                    { 1289, "38.4679216334235198862851366247" },
                    { 1290, "38.4855706036953666364531575960" },
                    { 1291, "38.5067660479005805713625355867" },
                    { 1292, "38.5156677668367958413079936531" },
                    { 1293, "38.5266774851862483238781351361" },
                    { 1294, "38.5344754461316149188000819039" },
                    { 1295, "38.5677120957429096114283775702" },
                    { 1296, "38.5707557320732842479169260242" },
                    { 1297, "38.5843591052132882442152079797" },
                    { 1298, "38.5992877614807284866285290296" },
                    { 1299, "38.6187339984213757921171961556" },
                    { 1300, "38.6047666607721091357493814360" },
                    { 1301, "38.6369601102595217094921935324" },
                    { 1302, "38.6456494165579014648455075409" },
                    { 1303, "38.6491875827773183131907286497" },
                    { 1304, "38.6577345163436154245889710097" },
                    { 1305, "38.6708937138927867780735465950" },
                    { 1306, "38.6892242953606762318074448226" },
                    { 1307, "38.7016951119912548734308447539" },
                    { 1308, "38.7156323056647911365920201702" },
                    { 1309, "38.7376574223656831080106553678" },
                    { 1310, "38.7607981633345599953128066009" },
                    { 1311, "38.7663047598387233635753617906" },
                    { 1312, "38.7788732982541368437521011914" },
                    { 1313, "38.8018986348529580246041612529" },
                    { 1314, "38.8289485026830092406688392562" },
                    { 1315, "38.8377467829092579297801846753" },
                    { 1316, "38.8592985299764229639428636504" },
                    { 1317, "38.8677143958659557838842367886" },
                    { 1318, "38.8801079764769550532286429453" },
                    { 1319, "38.8896538694450889016578974695" },
                    { 1320, "38.9044839002360787306074732129" },
                    { 1321, "38.9143389853911501797674740793" },
                    { 1322, "38.9409696614784389073560119753" },
                    { 1323, "38.9406073797720641200993833869" },
                    { 1324, "38.9607931599000462174806091439" },
                    { 1325, "38.9903044229606791704663385337" },
                    { 1326, "39.0026918273974872258287550997" },
                    { 1327, "39.0212251207433350889303883271" },
                    { 1328, "39.0331812475369361911636577364" },
                    { 1329, "39.0445072508352304025127467795" },
                    { 1330, "39.0538065150101479694891463822" },
                    { 1331, "39.0705489824568783456101980154" },
                    { 1332, "39.0839796538004199182355709540" },
                    { 1333, "39.1049928999059606310572678009" },
                    { 1334, "39.1299734838250525249814646528" },
                    { 1335, "39.1410967942846880609200760656" },
                    { 1336, "39.1704511641320148837177507530" },
                    { 1337, "39.1678777747888238077774958923" },
                    { 1338, "39.1791441907984062625409432479" },
                    { 1339, "39.1810079571234966611010568821" },
                    { 1340, "39.2205905805562957587601403840" },
                    { 1341, "39.2251102833957616629982469128" },
                    { 1342, "39.2275806621664569503817212172" },
                    { 1343, "39.2373195648173117223244557159" },
                    { 1344, "39.2625771524818058444599487680" },
                    { 1345, "39.1957197342335528779821453837" },
                    { 1346, "39.3104355229351038732008519778" },
                    { 1347, "39.3198125857763939873105051879" },
                    { 1348, "39.3378566774619016290139138028" },
                    { 1349, "39.3449319061200627276155199063" },
                    { 1350, "39.3627333117197285451298240508" },
                    { 1351, "39.3850491545600367228445032884" },
                    { 1352, "39.3838397487421230446452899895" },
                    { 1353, "39.3846081753672472902744686461" },
                    { 1354, "39.4386586169313792099864952989" },
                    { 1355, "39.3945441608420837909132181167" },
                    { 1356, "39.4053751026822258314196236850" },
                    { 1357, "39.4153534867143426657053739619" },
                    { 1358, "39.4210480402302707618796471501" },
                    { 1359, "39.4455830317506019274802912434" },
                    { 1360, "39.4682961900063388119575773017" },
                    { 1361, "39.4833400443087149735797416226" },
                    { 1362, "39.4930291649029727994946889637" },
                    { 1363, "39.5169502061708419327816617135" },
                    { 1364, "39.5737899261110050152357606335" },
                    { 1365, "39.5907238944988483918678158180" },
                    { 1366, "39.5961210026814341636505843618" },
                    { 1367, "39.5665463763808805072547781066" },
                    { 1368, "39.5867479725083496143187722298" },
                    { 1369, "39.5380281575298227462520886583" },
                    { 1370, "39.6417643307607479357216765448" },
                    { 1371, "39.6512746907871106975017237140" },
                    { 1372, "39.6644726417809814860822178122" },
                    { 1373, "39.6897149033020164892615035703" },
                    { 1374, "39.7082552094960710774192837940" },
                    { 1375, "39.7077825098281952620721444146" },
                    { 1376, "39.7256574517897521646568913668" },
                    { 1377, "39.7398798873841768813533763743" },
                    { 1378, "39.7374541833964339957200251057" },
                    { 1379, "39.7456138543321262314226392114" },
                    { 1380, "39.7586897509224489923124194901" },
                    { 1381, "39.7478767162379275421656594065" },
                    { 1382, "39.8053649283443944225532488085" },
                    { 1383, "39.8245015511831549731820486588" },
                    { 1384, "39.8457701964159604438731592046" },
                    { 1385, "39.8639280561122504904946515463" },
                    { 1386, "39.8769002945790211238401735948" },
                    { 1387, "39.8947606792530960561403994094" },
                    { 1388, "39.9079279413702391045293768867" },
                    { 1389, "39.9122595183343921335203348333" },
                    { 1390, "39.9329741289192050451387647396" },
                    { 1391, "39.9506297986125071781963401427" },
                    { 1392, "39.9686692198310527252558358841" },
                    { 1393, "39.9511950992776390071405973291" },
                    { 1394, "39.9861011276338391593127087122" },
                    { 1395, "40.0094669984769767692120378817" },
                    { 1396, "40.0132007739256089768496132536" },
                    { 1397, "40.0373564854105729990775802128" },
                    { 1398, "40.0306143872304590103046598690" },
                    { 1399, "40.0482923029524236248724442524" },
                    { 1400, "40.0640658449571495557801889283" },
                    { 1401, "40.0716631288103957896824957473" },
                    { 1402, "40.0871115640845138693500338474" },
                    { 1403, "40.1050212259164001746550627972" },
                    { 1404, "40.1175518131317236355829663567" },
                    { 1405, "40.1293407771541941004139905286" },
                    { 1406, "40.1540855010769651561594098434" },
                    { 1407, "40.1731617562189614561745163260" },
                    { 1408, "40.1763734637958208461238964898" },
                    { 1409, "40.1912229271199966521154762415" },
                    { 1410, "40.2030988524081900739781756035" },
                    { 1411, "40.2018944448557836363608214863" },
                    { 1412, "40.2080106417840406300096073017" },
                    { 1413, "40.2423419419430405712287558714" },
                    { 1414, "40.2510738463147992722774107383" },
                    { 1415, "40.2585150157095450294491111019" },
                    { 1416, "40.2723245833636852968172208227" },
                    { 1417, "40.2840212651120348404205996766" },
                    { 1418, "40.2970229802367941253135591558" },
                    { 1419, "40.3052957158306160852850198359" },
                    { 1420, "40.3179857813871157593313272317" },
                    { 1421, "40.3440187787111373246670441970" },
                    { 1422, "40.3488447105797589276464821135" },
                    { 1423, "40.3607273044909418543479660610" },
                    { 1424, "40.3904129839250383658342940839" },
                    { 1425, "40.4067519743166689691910747402" },
                    { 1426, "40.4098399702750050153402318386" },
                    { 1427, "40.4215923183552964908798897024" },
                    { 1428, "40.4368333148352574823603980128" },
                    { 1429, "40.4456210392377714274325577465" },
                    { 1430, "40.4655158470798213007130734504" },
                    { 1431, "40.4818365564627916794930333219" },
                    { 1432, "40.4843967372436547986904876339" },
                    { 1433, "40.4989593421893238864999230821" },
                    { 1434, "40.5154599879326704414298647476" },
                    { 1435, "40.5317353458508899119613764629" },
                    { 1436, "40.5409930792763672314629027612" },
                    { 1437, "40.5634090057255290215801596326" },
                    { 1438, "40.5746944217837298824381865713" },
                    { 1439, "40.5895016553978722989814027571" },
                    { 1440, "40.6009334927336069424237306765" },
                    { 1441, "40.6026617878742322317884572269" },
                    { 1442, "40.6193014887414580398387677724" },
                    { 1443, "40.6298163638273620314458614205" },
                    { 1444, "40.6534530012014266459029882164" },
                    { 1445, "40.6660886570921629068412023057" },
                    { 1446, "40.6872265684175139857995044465" },
                    { 1447, "40.7073595864071521309455702095" },
                    { 1448, "40.7243194488349903125877310621" },
                    { 1449, "40.7421367334686316210456918353" },
                    { 1450, "40.7545860005939798880237424988" },
                    { 1451, "40.7648812859780093782270524506" },
                    { 1452, "40.7744323738676027801341363958" },
                    { 1453, "40.7921977053012281345008311994" },
                    { 1454, "40.8003205619530514640864362333" },
                    { 1455, "40.8362066793627162953835303282" },
                    { 1456, "40.8469664491748264809020406891" },
                    { 1457, "40.8725982112812002291381394112" },
                    { 1458, "40.8885736909573679559032903825" },
                    { 1459, "40.8975636568521836141231884334" },
                    { 1460, "40.9132670169526667653664038110" },
                    { 1461, "40.9077659719583138442770661995" },
                    { 1462, "40.9174384272575039648792136380" },
                    { 1463, "40.9332972497283547166586574202" },
                    { 1464, "40.9541054664063393574792938809" },
                    { 1465, "40.9502499898975913420181685935" },
                    { 1466, "40.9599423899665153096538845888" },
                    { 1467, "40.9729671816320173226307116115" },
                    { 1468, "40.9853993801928656754519040686" },
                    { 1469, "40.9924962798897564861179050076" },
                    { 1470, "40.9923643151619650281188585595" },
                    { 1471, "41.0311377648146112753915598885" },
                    { 1472, "41.0251113491601192752439279783" },
                    { 1473, "41.0406663595183346302199086671" },
                    { 1474, "41.0481359881132827022882185659" },
                    { 1475, "41.0543276160849750199919019546" },
                    { 1476, "41.0694025689773426032114651011" },
                    { 1477, "41.0709410917512487428858643299" },
                    { 1478, "41.0890359195565801948378378507" },
                    { 1479, "41.1046717411537084429224654679" },
                    { 1480, "41.1080433908812980069960055753" },
                    { 1481, "41.1332458899043204672293787966" },
                    { 1482, "41.1402649902448675732418426758" },
                    { 1483, "41.1426174435275655795271469263" },
                    { 1484, "41.1777692605148351392312033408" },
                    { 1485, "41.1931270792469557541835464052" },
                    { 1486, "41.2111443132739155966917304999" },
                    { 1487, "41.2200195989178157065130144760" },
                    { 1488, "41.2319542935610217676130595152" },
                    { 1489, "41.2436860293640082538262386541" },
                    { 1490, "41.2618891747925700769770099161" },
                    { 1491, "41.2702726400920228215681185937" },
                    { 1492, "41.2904057353272848770568524972" },
                    { 1493, "41.2976972457725806704751852185" },
                    { 1494, "41.3206081142477649205647664508" },
                    { 1495, "41.3170591263875378312072024547" },
                    { 1496, "41.3424401972047868157764845109" },
                    { 1497, "41.3710211209005364899026082374" },
                    { 1498, "41.3760028831266683443586556994" },
                    { 1499, "41.3892086588112012406816556670" },
                    { 1500, "41.4126836805494701799204483237" },
                    { 1501, "41.4369811261006312712032857156" },
                    { 1502, "41.4504671502517470302089689067" },
                    { 1503, "41.4512189562411296545749955818" },
                    { 1504, "41.4548714092894480926821101014" },
                    { 1505, "41.4747197828418422585409228134" },
                    { 1506, "41.4842129997392878542617480512" },
                    { 1507, "41.4897830518913387407373292413" },
                    { 1508, "41.5316405338588488175285134866" },
                    { 1509, "41.5542728627382848000767826074" },
                    { 1510, "41.5857043285429922576603301952" },
                    { 1511, "41.6107369111808351404735212406" },
                    { 1512, "41.6124690434229252168485442475" },
                    { 1513, "41.6368062782365493027846283997" },
                    { 1514, "41.6459884406491881520108679759" },
                    { 1515, "41.6505438945539567049649082239" },
                    { 1516, "41.6606098022991954572325186766" },
                    { 1517, "41.6646597080419006674734947671" },
                    { 1518, "41.6814254575903859606893641362" },
                    { 1519, "41.6888760997948808852187623757" },
                    { 1520, "41.7144040679663181815843513884" },
                    { 1521, "41.7317547644128953787607919539" },
                    { 1522, "41.7427900421466991200216168443" },
                    { 1523, "41.7542296296696381520547987809" },
                    { 1524, "41.7692482369913108641222296504" },
                    { 1525, "41.7837389299853957373635140482" },
                    { 1526, "41.8003832068042844563611709733" },
                    { 1527, "41.8045932552398686894650557583" },
                    { 1528, "41.8126662281987386450123465860" },
                    { 1529, "41.8214738708299172411708385056" },
                    { 1530, "41.8406451397953238867168200477" },
                    { 1531, "41.8484787479623378704197535518" },
                    { 1532, "41.8672549892164083015749960133" },
                    { 1533, "41.8722774115837748467243160226" },
                    { 1534, "41.8840529520457918997561042631" },
                    { 1535, "41.8960792303733061589530067940" },
                    { 1536, "41.9174048016062043885756440741" },
                    { 1537, "41.9286440875821147420880732652" },
                    { 1538, "41.9525693753738116282276723566" },
                    { 1539, "41.9493799915257481068756564475" },
                    { 1540, "41.9626563150099525117069087860" },
                    { 1541, "41.9826507186948933579417549426" },
                    { 1542, "42.0059446687543765575152769131" },
                    { 1543, "42.0194396770094817250441824205" },
                    { 1544, "42.0344079469966630842167459527" },
                    { 1545, "42.0461215103887256751137374662" },
                    { 1546, "42.0591933068497079501714755829" },
                    { 1547, "42.0760381508735117165428715342" },
                    { 1548, "42.0872916749841311267793668924" },
                    { 1549, "42.0925857431670118204241341452" },
                    { 1550, "42.1059779215907684815196779919" },
                    { 1551, "42.1181149558623842784400527966" },
                    { 1552, "42.1238719896869814717222375197" },
                    { 1553, "42.1335218581496096694254822790" },
                    { 1554, "42.1458422692482464763804603031" },
                    { 1555, "42.1603234154275641025138710876" },
                    { 1556, "42.1734354659201784285317034120" },
                    { 1557, "42.2032757816536297694078776254" },
                    { 1558, "42.2050381348339541142970757610" },
                    { 1559, "42.2266126223875108290899143310" },
                    { 1560, "42.2365893459611840100022057324" },
                    { 1561, "42.2404581732089711858661703103" },
                    { 1562, "42.2599245258086777200140587856" },
                    { 1563, "42.2670630996957431885404811772" },
                    { 1564, "42.2773904816549518122531356947" },
                    { 1565, "42.2847080902851555526318273259" },
                    { 1566, "42.3119558801030512351721261751" },
                    { 1567, "42.3276269254271810101579600396" },
                    { 1568, "42.3422276271295248969673193685" },
                    { 1569, "42.3445980386284261436725723126" },
                    { 1570, "42.3548057597639576351691955381" },
                    { 1571, "42.3762581204279160923260935562" },
                    { 1572, "42.3965517258173915673873879379" },
                    { 1573, "42.4289307514590632835632973109" },
                    { 1574, "42.4386856751381591640292316892" },
                    { 1575, "42.4529952245176359575379802650" },
                    { 1576, "42.4737481655795019098607863835" },
                    { 1577, "42.4886624485288010157785676828" },
                    { 1578, "42.5142688861944168985105939547" },
                    { 1579, "42.5094971445778195829649494489" },
                    { 1580, "42.5399827897492013781949579585" },
                    { 1581, "42.5400461689427922738243595274" },
                    { 1582, "42.5417237108932481740349703712" },
                    { 1583, "42.5613436504772984841082574033" },
                    { 1584, "42.5858714219403715217262734021" },
                    { 1585, "42.5963059660847817819084129010" },
                    { 1586, "42.6219467875837121218690707123" },
                    { 1587, "42.6304729557124800326005744139" },
                    { 1588, "42.6497336198454420967711812117" },
                    { 1589, "42.6156180402361285496956497318" },
                    { 1590, "42.6291307473924077730377682969" },
                    { 1591, "42.6351148839434520120593478747" },
                    { 1592, "42.6452981659327394721397699051" },
                    { 1593, "42.6535333554066751057294399883" },
                    { 1594, "42.6569429830607648826552814871" },
                    { 1595, "42.6685012974329830153908521097" },
                    { 1596, "42.6837853916123825244898938506" },
                    { 1597, "42.6704368187288409945422246455" },
                    { 1598, "42.6995438935835385517094670486" },
                    { 1599, "42.7116398441828504075414644407" },
                    { 1600, "42.7373701482957571366728444424" },
                    { 1601, "42.7458425708483149387606927640" },
                    { 1602, "42.7587777734461615563513684801" },
                    { 1603, "42.7832608263250616605461305325" },
                    { 1604, "42.8014625511676846499459741250" },
                    { 1605, "42.8050788316940927182620873944" },
                    { 1606, "42.8089270238143785614114671966" },
                    { 1607, "42.8237293281359155306984701520" },
                    { 1608, "42.8346729186807620443857197037" },
                    { 1609, "42.8623230656620807592209375840" },
                    { 1610, "42.8628599298494022655759865300" },
                    { 1611, "42.8671506682714067255628629277" },
                    { 1612, "42.8755879416583101724505675800" },
                    { 1613, "42.8872289923767745298816625846" },
                    { 1614, "42.8907135413626615285846018932" },
                    { 1615, "42.8979738213298838693716893486" },
                    { 1616, "42.9287463587618213613581904283" },
                    { 1617, "42.9358882111202875522810774849" },
                    { 1618, "42.9401231668262242884963507279" },
                    { 1619, "42.9597888506648605577552281628" },
                    { 1620, "42.9931084753463007208004199461" },
                    { 1621, "42.9947398647825766374764633090" },
                    { 1622, "43.0072056171580389616323229498" },
                    { 1623, "43.0155799704377580572391022151" },
                    { 1624, "43.0298360553940023604614351144" },
                    { 1625, "43.0465831659986154060264572234" },
                    { 1626, "43.0372459787635826316643832689" },
                    { 1627, "43.0426983961720991287090496296" },
                    { 1628, "43.1114115798111890313324092855" },
                    { 1629, "43.1125039205863330879858637557" },
                    { 1630, "43.1209879837647475839347639785" },
                    { 1631, "43.1358322623112006863692715176" },
                    { 1632, "43.1567148912317373407462857737" },
                    { 1633, "43.1609005294075999704966192870" },
                    { 1634, "43.1841594342001474245158899892" },
                    { 1635, "43.2027537796545783621071851600" },
                    { 1636, "43.2145211794559537151771362019" },
                    { 1637, "43.2296871100184708043606593345" },
                    { 1638, "43.2301854804156082546676125174" },
                    { 1639, "43.2262765919981467114409501321" },
                    { 1640, "43.2725997268742945770059791580" },
                    { 1641, "43.2912452367638274709319080336" },
                    { 1642, "43.3158023324923932765629202978" },
                    { 1643, "43.3298829234190309219446385552" },
                    { 1644, "43.3443433076648933487144331502" },
                    { 1645, "43.3619310874137137772512551481" },
                    { 1646, "43.3713924121968302497432805696" },
                    { 1647, "43.3953130590997446645348829410" },
                    { 1648, "43.4052797547255699712132726738" },
                    { 1649, "43.4182809284576267646668830784" },
                    { 1650, "43.4252808285405315289274220993" },
                    { 1651, "43.4317846636687280274651056396" },
                    { 1652, "43.4695696482342288553701091820" },
                    { 1653, "43.4756576029063177342674869513" },
                    { 1654, "43.4903689305790578765945520685" },
                    { 1655, "43.5080100164198053878862601053" },
                    { 1656, "43.5165171066727273922279564876" },
                    { 1657, "43.5330173982120765459990544958" },
                    { 1658, "43.5364508349947670083638734791" },
                    { 1659, "43.5436961601830608793821538813" },
                    { 1660, "43.5700309152192939007210017890" },
                    { 1661, "43.5841957667151944950834090325" },
                    { 1662, "43.5926081863747374013294628819" },
                    { 1663, "43.5926234732434033175478806364" },
                    { 1664, "43.5961517108497803211754634513" },
                    { 1665, "43.6193401316733714844668522429" },
                    { 1666, "43.6279858806530068874308258309" },
                    { 1667, "43.6306391337139027002326021865" },
                    { 1668, "43.6583583401818847638872207097" },
                    { 1669, "43.6647201286707306132115967834" },
                    { 1670, "43.6892586553567018323524490849" },
                    { 1671, "43.7071785436757842832538746541" },
                    { 1672, "43.7130372719885169696511305589" },
                    { 1673, "43.7276579205815548075257953584" },
                    { 1674, "43.7339837493721940281415252043" },
                    { 1675, "43.7415498887004821846291815696" },
                    { 1676, "43.7504025467751042944660258801" },
                    { 1677, "43.7640608708224168982737872458" },
                    { 1678, "43.7772511613353880345781837986" },
                    { 1679, "43.7987149566710158599628161687" },
                    { 1680, "43.8146248036782420450896406454" },
                    { 1681, "43.8180907732775057500899057671" },
                    { 1682, "43.8248593738078523289077119640" },
                    { 1683, "43.8519079456429771428710290718" },
                    { 1684, "43.8551358854700527497898218622" },
                    { 1685, "43.8864858983345239407422699687" },
                    { 1686, "43.8952826761826371951652896611" },
                    { 1687, "43.9001726102600888413795981892" },
                    { 1688, "43.9010632169120304436363674470" },
                    { 1689, "43.9013328340794139999538369552" },
                    { 1690, "43.9221701510825462803599324062" },
                    { 1691, "43.9324242570728905834776345141" },
                    { 1692, "43.9492822836768400853792777745" },
                    { 1693, "43.9695160836735997882441841296" },
                    { 1694, "43.9741181409587800748380751932" },
                    { 1695, "43.9993695200042535646939933088" },
                    { 1696, "44.0181912646786335417564837963" },
                    { 1697, "44.0235438799804624633944169555" },
                    { 1698, "44.0401734866242865475479121522" },
                    { 1699, "44.0476190016305880806459942427" },
                    { 1700, "44.0632216109875087860104387546" },
                    { 1701, "44.0754987364389636240981686283" },
                    { 1702, "44.0837031590731415033371120788" },
                    { 1703, "44.0860284850226132268051671965" },
                    { 1704, "44.1251022503219481401036152948" },
                    { 1705, "44.1263281049434263465832705133" },
                    { 1706, "44.1443614885192162805454952374" },
                    { 1707, "44.1577588745044898629965833304" },
                    { 1708, "44.1688417265810020067539249601" },
                    { 1709, "44.1888982826805401163533441802" },
                    { 1710, "44.2022491981818755572208619224" },
                    { 1711, "44.2034529710502187707812091606" },
                    { 1712, "44.2255802684491268119563804418" },
                    { 1713, "44.2407312919915775042895530659" },
                    { 1714, "44.2479652441929540222062169696" },
                    { 1715, "44.2701948457568568637000008761" },
                    { 1716, "44.2826288587526185812309438947" },
                    { 1717, "44.2882973170688912701742780064" },
                    { 1718, "44.3051337436437948441105649300" },
                    { 1719, "44.3093353325138189491420943714" },
                    { 1720, "44.3115021312514611757898486899" },
                    { 1721, "44.3126900519710653685619544110" },
                    { 1722, "44.3128364482441762852680174580" },
                    { 1723, "44.3133413221729355423388940292" },
                    { 1724, "44.3562297729786297342285683747" },
                    { 1725, "44.3849074484547600226150448084" },
                    { 1726, "44.3977921419733960967107380541" },
                    { 1727, "44.4066486359417130026797218033" },
                    { 1728, "44.4173570690421161844062665354" },
                    { 1729, "44.4232104664088812686788886640" },
                    { 1730, "44.4388158099463891166278978600" },
                    { 1731, "44.4518537401244227686298564501" },
                    { 1732, "44.4641115115864425785194709635" },
                    { 1733, "44.4673723569776730365764223434" },
                    { 1734, "44.4761533423900539119215573359" },
                    { 1735, "44.4663803611755387539721099431" },
                    { 1736, "44.4823792933729375322471376519" },
                    { 1737, "44.4900563834165729658038263896" },
                    { 1738, "44.4982161517061241998900480936" },
                    { 1739, "44.5045726793462286374044433750" },
                    { 1740, "44.5130364753052698949706128544" },
                    { 1741, "44.5223896946712234817648985528" },
                    { 1742, "44.5420952410787023647269332012" },
                    { 1743, "44.5521844851258431566645193438" },
                    { 1744, "44.5543392165024759294415290808" },
                    { 1745, "44.5618397791720858722238012656" },
                    { 1746, "44.5801474963985519644751447269" },
                    { 1747, "44.5907924759953530666686103555" },
                    { 1748, "44.5971528527642753940407013940" },
                    { 1749, "44.6070898790508918464326495570" },
                    { 1750, "44.6269409096060285499040284464" },
                    { 1751, "44.6392560253142778527526406560" },
                    { 1752, "44.6543612317762576156533593159" },
                    { 1753, "44.6700565782016065717002139454" },
                    { 1754, "44.6728196550706491597535680074" },
                    { 1755, "44.6857285143740584344949685775" },
                    { 1756, "44.7010800879517847975032252552" },
                    { 1757, "44.7171501884188044392056803202" },
                    { 1758, "44.7403234234215773879232233251" },
                    { 1759, "44.7256593668606640972608001113" },
                    { 1760, "44.7650534363251609207527749070" },
                    { 1761, "44.7879550098309567648025757683" },
                    { 1762, "44.7979018506854592628426220912" },
                    { 1763, "44.8040341639411928164783281865" },
                    { 1764, "44.8081049809870962450549028084" },
                    { 1765, "44.8108342310350945892269059437" },
                    { 1766, "44.8350447581872522606054764909" },
                    { 1767, "44.8581841933341194225046050073" },
                    { 1768, "44.8666008928567509388755682002" },
                    { 1769, "44.8749172567411197496043525021" },
                    { 1770, "44.8844510972598536707729734494" },
                    { 1771, "44.9165013011914600017201382336" },
                    { 1772, "44.9357626364944733971965890015" },
                    { 1773, "44.9809353756990184896197969845" },
                    { 1774, "44.9815833563675987426547364856" },
                    { 1775, "44.9914667153305661152813162834" },
                    { 1776, "44.9998304529705458628249217466" },
                    { 1777, "45.0049500558398783860463458604" },
                    { 1778, "45.0382879345366958099203020590" },
                    { 1779, "45.0667774536605139170825678959" },
                    { 1780, "45.0683098056119021323114775874" },
                    { 1781, "45.0868534368103846072764930312" },
                    { 1782, "45.1129801476330795467425917276" },
                    { 1783, "45.1146444688199560674084265057" },
                    { 1784, "45.1268536701455471660541308216" },
                    { 1785, "45.1315249939304849636122658966" },
                    { 1786, "45.1334206238978805231143806765" },
                    { 1787, "45.1354586895598599625452859998" },
                    { 1788, "45.1362223529988756978419724064" },
                    { 1789, "45.1355159728298055792778495125" },
                    { 1790, "45.1824174030606468449782247446" },
                    { 1791, "45.2061331510004351356816033954" },
                    { 1792, "45.2430340863789414087100273377" },
                    { 1793, "45.2623381691570262660731481400" },
                    { 1794, "45.2809538079273506073923647779" },
                    { 1795, "45.2828457031638197855222884917" },
                    { 1796, "45.2970456142780290864799701546" },
                    { 1797, "45.3087650471947734375184826942" },
                    { 1798, "45.3228910233297181784771144156" },
                    { 1799, "45.3340581876960131433949125448" },
                    { 1800, "45.3407921793657719304643212378" },
                    { 1801, "45.3583476067690404430696959245" },
                    { 1802, "45.3649320804695216203554443692" },
                    { 1803, "45.3731781953515840241519410763" },
                    { 1804, "45.3884968683809134455136753848" },
                    { 1805, "45.4001104862813336345048349372" },
                    { 1806, "45.4085758327851154230519570885" },
                    { 1807, "45.4157943036443661222247987829" },
                    { 1808, "45.4175553515150794810245654059" },
                    { 1809, "45.4388401911750707709686169507" },
                    { 1810, "45.4456323903907061250457149449" },
                    { 1811, "45.4569060699893747325858986787" },
                    { 1812, "45.4667637939440875566000643383" },
                    { 1813, "45.4826253404192697850369888129" },
                    { 1814, "45.4948893695765158746457998688" },
                    { 1815, "45.5013963245390525089788582115" },
                    { 1816, "45.5187574715563016264764229023" },
                    { 1817, "45.5276862466710109907889650034" },
                    { 1818, "45.5365405634940266189074377465" },
                    { 1819, "45.5554429653290219270326120554" },
                    { 1820, "45.5678337675945073081957331472" },
                    { 1821, "45.5803325396210487395895402492" },
                    { 1822, "45.5845744598653391936300042270" },
                    { 1823, "45.5980748946113370083874972052" },
                    { 1824, "45.6087760508699461070619099730" },
                    { 1825, "45.6173710024674612978557218973" },
                    { 1826, "45.6267509919262944988861884197" },
                    { 1827, "45.6493022715683998797667331119" },
                    { 1828, "45.6679632940603261071340596371" },
                    { 1829, "45.6724979071599003084592986301" },
                    { 1830, "45.6739381907688949038162743529" },
                    { 1831, "45.6861754386306089707009619677" },
                    { 1832, "45.6962573033773666222049364687" },
                    { 1833, "45.7028248381071260841023844591" },
                    { 1834, "45.7230167778755658509656807647" },
                    { 1835, "45.7247944227952191141431597750" },
                    { 1836, "45.7375436547420824101585736617" },
                    { 1837, "45.7510221614104950565911126482" },
                    { 1838, "45.7664288724261968682608659923" },
                    { 1839, "45.7777493864897811414966796672" },
                    { 1840, "45.7873558320585368244338651991" },
                    { 1841, "45.7936574798420593480776176133" },
                    { 1842, "45.8056353041680899791066511795" },
                    { 1843, "45.8117348932893086668117754025" },
                    { 1844, "45.8292427004797000656613807719" },
                    { 1845, "45.8680338548148877239303901358" },
                    { 1846, "45.8776370669228786935038467843" },
                    { 1847, "45.8816142331927102070978791400" },
                    { 1848, "45.9088972571199612613646243357" },
                    { 1849, "45.9305143300223714574683619088" },
                    { 1850, "45.9511554728248311387774186671" },
                    { 1851, "45.9638823915028630335563186863" },
                    { 1852, "45.9806597536332259389483472856" },
                    { 1853, "46.0017683649981643897696204528" },
                    { 1854, "46.0099562352448325494805010318" },
                    { 1855, "46.0360856009717094784164905629" },
                    { 1856, "46.0437330672192258913315191047" },
                    { 1857, "46.0484570077880341298322024094" },
                    { 1858, "46.0604228063433767430656554987" },
                    { 1859, "46.0729376531299902268292292365" },
                    { 1860, "46.0743124142878222089641350323" },
                    { 1861, "46.0795223662746797691991081718" },
                    { 1862, "46.0804622708797103621175052294" },
                    { 1863, "46.0854367724128641205921444345" },
                    { 1864, "46.0859107946564859120723948947" },
                    { 1865, "46.0864861041703400147600859791" },
                    { 1866, "46.0871973714704752149362595026" },
                    { 1867, "46.0777002219545426294207856811" },
                    { 1868, "46.1236405082049766755703511799" },
                    { 1869, "46.1275693676867122195070032163" },
                    { 1870, "46.1563103158126823813333105968" },
                    { 1871, "46.1667556624392602431222367013" },
                    { 1872, "46.1936484964799131550679938735" },
                    { 1873, "46.1946238174158869107949989641" },
                    { 1874, "46.1958148703300392243750676357" },
                    { 1875, "46.1977085334949954957460665081" },
                    { 1876, "46.2068937520145007664473594707" },
                    { 1877, "46.2219201237769344677071336528" },
                    { 1878, "46.2249653370429777934317146063" },
                    { 1879, "46.2268279656658824724577114545" },
                    { 1880, "46.2549451449781402865323414156" },
                    { 1881, "46.2705568004547626905287568517" },
                    { 1882, "46.2795961527507026516226797197" },
                    { 1883, "46.2852455946235423153218209952" },
                    { 1884, "46.3007410938671379638465806977" },
                    { 1885, "46.3019147544009315116462093390" },
                    { 1886, "46.3197765285278715243604069562" },
                    { 1887, "46.3426857264480227397094829104" },
                    { 1888, "46.3462288390951074872831162577" },
                    { 1889, "46.3465578456462016158559216152" },
                    { 1890, "46.3555992239564590365505177544" },
                    { 1891, "46.3435695901345739177474702363" },
                    { 1892, "46.4017027964056932706181096939" },
                    { 1893, "46.4376035618829834129040281115" },
                    { 1894, "46.4404460550060380280129294419" },
                    { 1895, "46.4515437253893394015089262017" },
                    { 1896, "46.4562460963308386473458233776" },
                    { 1897, "46.4724691759976092723862481884" },
                    { 1898, "46.4856866059629508362803535538" },
                    { 1899, "46.5021326399452780712827787383" },
                    { 1900, "46.5193610730846373744233757390" },
                    { 1901, "46.5228102220998096181205648325" },
                    { 1902, "46.5441796103241517228372691603" },
                    { 1903, "46.5466044166098431598982749902" },
                    { 1904, "46.5575317883734318848585904659" },
                    { 1905, "46.5661400334165547141740462970" },
                    { 1906, "46.5724647537300681148713884801" },
                    { 1907, "46.5855254963354632966098936505" },
                    { 1908, "46.6016424274628447610130502058" },
                    { 1909, "46.6036569493378402633398395208" },
                    { 1910, "46.6054473981221727790406886363" },
                    { 1911, "46.6115886446854851016321078492" },
                    { 1912, "46.6220010240946301699313249830" },
                    { 1913, "46.6324063310007912264057422459" },
                    { 1914, "46.6447897382194376809863798864" },
                    { 1915, "46.6504736576621034206033893240" },
                    { 1916, "46.7030668718587641279187129842" },
                    { 1917, "46.7139522000946948078973308296" },
                    { 1918, "46.7329213184649342651220347504" },
                    { 1919, "46.7441532096900530683138412805" },
                    { 1920, "46.7481824890858789350394961423" },
                    { 1921, "46.7538050267433897380286087324" },
                    { 1922, "46.7842419000016046292023781045" },
                    { 1923, "46.7990618482868600587583438646" },
                    { 1924, "46.8172675037140928080845580951" },
                    { 1925, "46.8360691906970230117479656629" },
                    { 1926, "46.8471054588142962370853880166" },
                    { 1927, "46.8699039348099245835862951244" },
                    { 1928, "46.8712724920752722108057294992" },
                    { 1929, "46.8873257807129800326183170726" },
                    { 1930, "46.9197575142243740811767851508" },
                    { 1931, "46.9179728908447866665046551446" },
                    { 1932, "46.9321678907392650815186126570" },
                    { 1933, "46.9353644559228592964021237982" },
                    { 1934, "46.9456418621722479037829871571" },
                    { 1935, "46.9575617485745746998792292747" },
                    { 1936, "46.9772854718152426278981700814" },
                    { 1937, "46.9863075735515876568466006561" },
                    { 1938, "47.0003051710216035153307636614" },
                    { 1939, "47.0137080348428970971802476989" },
                    { 1940, "47.0182654831354140962322955152" },
                    { 1941, "47.0260713108569642331548268192" },
                    { 1942, "47.0307354937834917416008755449" },
                    { 1943, "47.0553450719610453028650509843" },
                    { 1944, "47.0626333231005789678219112445" },
                    { 1945, "47.0918020523046709985992958446" },
                    { 1946, "47.1008169180649428788176784749" },
                    { 1947, "47.1050051202775357656338272754" },
                    { 1948, "47.1149006635080483631199562888" },
                    { 1949, "47.1280576042588443005748761964" },
                    { 1950, "47.1370035155920115415023011726" },
                    { 1951, "47.1532170499147539616782535143" },
                    { 1952, "47.1730341665113472767032132383" },
                    { 1953, "47.1810720797725940766623570033" },
                    { 1954, "47.1886906015363036633719565006" },
                    { 1955, "47.2266787720664414580819875734" },
                    { 1956, "47.2282315783194193740052287591" },
                    { 1957, "47.2363776145926688044077663560" },
                    { 1958, "47.2444303419814686820408707717" },
                    { 1959, "47.2533070083510624999637228910" },
                    { 1960, "47.2648390941468483762751704017" },
                    { 1961, "47.2841007173824245893337451073" },
                    { 1962, "47.2870307630846408559683536463" },
                    { 1963, "47.2971313315219551707334179603" },
                    { 1964, "47.3119480228228801459353614258" },
                    { 1965, "47.3150629361460532970118244296" },
                    { 1966, "47.3322312262089966947700369530" },
                    { 1967, "47.3582643428706829356790698982" },
                    { 1968, "47.3617725960936967400334869207" },
                    { 1969, "47.3712370702043021215680946244" },
                    { 1970, "47.3754334069686000457044874402" },
                    { 1971, "47.3925155644287217555007824461" },
                    { 1972, "47.4036825762972877036788902311" },
                    { 1973, "47.4180339595134449143595718212" },
                    { 1974, "47.4204131859884227501113610157" },
                    { 1975, "47.4377715774825195353091176341" },
                    { 1976, "47.4477846070943073402630438099" },
                    { 1977, "47.4648980427447738839999512659" },
                    { 1978, "47.4735840892522121904576792052" },
                    { 1979, "47.4908915724665261804395107998" },
                    { 1980, "47.4956421787080436713243628724" },
                    { 1981, "47.5154966542526067518041213169" },
                    { 1982, "47.5268316019839985654250477094" },
                    { 1983, "47.5456467636148912689152955410" },
                    { 1984, "47.5509615695275696028012595570" },
                    { 1985, "47.5628976360056171522179910310" },
                    { 1986, "47.5791182425677083480928495750" },
                    { 1987, "47.5859112725482095915547972075" },
                    { 1988, "47.5980540737243355720525705030" },
                    { 1989, "47.6119622861403511996266621611" },
                    { 1990, "47.6178487269725972029175208467" },
                    { 1991, "47.6320739162837929555086072093" },
                    { 1992, "47.6438455163765411507551494264" },
                    { 1993, "47.6588596891883342049177332437" },
                    { 1994, "47.6773587611865619757661821500" },
                    { 1995, "47.7000696561133732181787461134" },
                    { 1996, "47.7118190616702887878202856949" },
                    { 1997, "47.7185110022676392965629152767" },
                    { 1998, "47.7324336440707084154570816863" },
                    { 1999, "47.7385835773343375971480987322" },
                    { 2000, "47.7439933579944840856804601500" },
                    { 2001, "47.7634358181837796900806920088" },
                    { 2002, "47.7774852987849978876015025461" },
                    { 2003, "47.7923798700591064619293803695" },
                    { 2004, "47.8072509720423132306944473147" },
                    { 2005, "47.8142338098886734835947037065" },
                    { 2006, "47.8218096955354576194587249766" },
                    { 2007, "47.8264071936909319505295606331" },
                    { 2008, "47.8394348907824408843028658055" },
                    { 2009, "47.8405738055529411822354635807" },
                    { 2010, "47.8423856753448103409290638053" },
                    { 2011, "47.8554434563109111317878867706" },
                    { 2012, "47.8557922826109271866037613044" },
                    { 2013, "47.8678817799672112908529790567" },
                    { 2014, "47.8706962035612095629331699258" },
                    { 2015, "47.8723047798366249524388424519" },
                    { 2016, "47.8754309029289887865519034015" },
                    { 2017, "47.8805265727432398111274382960" },
                    { 2018, "47.9014504114307869942760393465" },
                    { 2019, "47.9110651362678174270959189891" },
                    { 2020, "47.9207901151116788513760157343" },
                    { 2021, "47.9255334501908194137524010673" },
                    { 2022, "47.9330922836329388876251201312" },
                    { 2023, "47.9492301640513453110863554191" },
                    { 2024, "47.9609409093377515069972755818" },
                    { 2025, "47.9736749570970040669420920258" },
                    { 2026, "47.9754462697462646261137404717" },
                    { 2027, "47.9756582770129399978521333753" },
                    { 2028, "47.9764541363623845846925368738" },
                    { 2029, "47.9769235633459068838461607367" },
                    { 2030, "48.0241756767096439411287568060" },
                    { 2031, "48.0407934856350339802736777351" },
                    { 2032, "48.0721973559388118235685551592" },
                    { 2033, "48.0834178175844072749414909249" },
                    { 2034, "48.1004053948565688776416912082" },
                    { 2035, "48.1096949894284230356189222390" },
                    { 2036, "48.1251342087368092936119680998" },
                    { 2037, "48.1544540734277515760589574903" },
                    { 2038, "48.1674778349448349661513548181" },
                    { 2039, "48.1736293538130840013656430491" },
                    { 2040, "48.1820469510978156969035321093" },
                    { 2041, "48.1968825846308915021148011148" },
                    { 2042, "48.2107329502426214083702609825" },
                    { 2043, "48.2245062421840290432158035636" },
                    { 2044, "48.2397920025326159028110364727" },
                    { 2045, "48.2432807946661933874951831769" },
                    { 2046, "48.2438332907941074054189641477" },
                    { 2047, "48.2493967806421083585594294221" },
                    { 2048, "48.2555608051817225962533705416" },
                    { 2049, "48.2676318834073939474000093505" },
                    { 2050, "48.2715787720940281866984378453" },
                    { 2051, "48.2742189400532864182618673847" },
                    { 2052, "48.2759063975600354924509222463" },
                    { 2053, "48.2771293542937184254825338141" },
                    { 2054, "48.3203924390592951625271806341" },
                    { 2055, "48.3337399034365126215055479138" },
                    { 2056, "48.3528482170997075155389901376" },
                    { 2057, "48.3697013727863103118335351049" },
                    { 2058, "48.3979283560038060528191903129" },
                    { 2059, "48.4039829372427518575466368370" },
                    { 2060, "48.4168589769027568947527172695" },
                    { 2061, "48.4320162634701612914178196358" },
                    { 2062, "48.4508579300906101008186741542" },
                    { 2063, "48.4528192504646460375636817345" },
                    { 2064, "48.4626577251981269248872781819" },
                    { 2065, "48.4698685815491825588922541951" },
                    { 2066, "48.4810224203567266728213416450" },
                    { 2067, "48.4930112880171281169816226098" },
                    { 2068, "48.5061489171470026172436826465" },
                    { 2069, "48.5188547355153080856726795108" },
                    { 2070, "48.5396668981043216892870392660" },
                    { 2071, "48.5444855227152012300067523199" },
                    { 2072, "48.5590080121773897054643846095" },
                    { 2073, "48.5638514828829255853124043573" },
                    { 2074, "48.5662420268560944703728647097" },
                    { 2075, "48.5696732845353476389155323495" },
                    { 2076, "48.5817602651066314601698099707" },
                    { 2077, "48.5765206786500107699099427104" },
                    { 2078, "48.6228978399375813673557905286" },
                    { 2079, "48.6269596088506796083005710553" },
                    { 2080, "48.6634613798591091236741494939" },
                    { 2081, "48.6835331166717211867176744826" },
                    { 2082, "48.6856831149045035109939954727" },
                    { 2083, "48.7021114364725095476077718187" },
                    { 2084, "48.7071092228621488234767614028" },
                    { 2085, "48.7196276953603500963705127907" },
                    { 2086, "48.7288905156122973166147415099" },
                    { 2087, "48.7417054250965533045864482001" },
                    { 2088, "48.7478349716575421504469587488" },
                    { 2089, "48.7569958245932288738708210313" },
                    { 2090, "48.7668914169889867156265680938" },
                    { 2091, "48.7783493489944175203443628879" },
                    { 2092, "48.7817656260635003212685992554" },
                    { 2093, "48.7892383914991666874072672407" },
                    { 2094, "48.7996688497471119114055571836" },
                    { 2095, "48.8259343822460429897863334248" },
                    { 2096, "48.8271215294305035371611336983" },
                    { 2097, "48.8451379444059811068650343282" },
                    { 2098, "48.8578256001103175789146975694" },
                    { 2099, "48.8790294029341717137736495354" },
                    { 2100, "48.8897944079330700698113907910" },
                    { 2101, "48.8941213067869684844494421186" },
                    { 2102, "48.9029535233270870012604070708" },
                    { 2103, "48.9243565796036413019584915841" },
                    { 2104, "48.9361312457596142563273457100" },
                    { 2105, "48.9464782054617467728942013372" },
                    { 2106, "48.9502302862970815146727548703" },
                    { 2107, "48.9673972237428481766747629039" },
                    { 2108, "48.9790102718458428354144177034" },
                    { 2109, "48.9858320332575061384287997875" },
                    { 2110, "49.0094506316932998050781743512" },
                    { 2111, "49.0047241658651356692639058813" },
                    { 2112, "49.0099962878900644430931323297" },
                    { 2113, "49.0344671525619216054713668623" },
                    { 2114, "49.0449652580024638874814844037" },
                    { 2115, "49.0496810598314294188229471222" },
                    { 2116, "49.0503881930710428345770632306" },
                    { 2117, "49.0752090221414212524486433127" },
                    { 2118, "49.0865301767052101998491683806" },
                    { 2119, "49.0962173501581295874617658904" },
                    { 2120, "49.1005778421255324368914037224" },
                    { 2121, "49.1038783380892185671024311924" },
                    { 2122, "49.1200285422987958089881606626" },
                    { 2123, "49.1311466308158516687360035242" },
                    { 2124, "49.1354642697427725108258599986" },
                    { 2125, "49.1413201181369336428807054323" },
                    { 2126, "49.1603657354447086948360376895" },
                    { 2127, "49.1630810478916464283530087385" },
                    { 2128, "49.1793961799464394225351982138" },
                    { 2129, "49.1907569324256203467393442388" },
                    { 2130, "49.1962993214325732246251884526" },
                    { 2131, "49.2189233490645710207456477542" },
                    { 2132, "49.2391894944704012451428935502" },
                    { 2133, "49.2481827689053799592585421533" },
                    { 2134, "49.2555377659821914892627610609" },
                    { 2135, "49.2674574050404576311412683583" },
                    { 2136, "49.2787865151149501977484481546" },
                    { 2137, "49.2868651690740913207416851063" },
                    { 2138, "49.3113066726487213004758071693" },
                    { 2139, "49.3214396344485271450397317596" },
                    { 2140, "49.3276342516148905239969768628" },
                    { 2141, "49.3295319044843059482720586562" },
                    { 2142, "49.3520734765492652953320406461" },
                    { 2143, "49.3864553243343121264309625535" },
                    { 2144, "49.3994488601827830148932062514" },
                    { 2145, "49.4038674217350320922360164716" },
                    { 2146, "49.4126855120746577578636518034" },
                    { 2147, "49.4363533022693937619728562918" },
                    { 2148, "49.4507044167833581516885939566" },
                    { 2149, "49.4550765168302511161123471458" },
                    { 2150, "49.4665109438287173704912387148" },
                    { 2151, "49.4670999021750746831626073830" },
                    { 2152, "49.4858349413736507038336914641" },
                    { 2153, "49.5171427259520050565965721914" },
                    { 2154, "49.5262561244627560391227454228" },
                    { 2155, "49.5292570562194314709310643262" },
                    { 2156, "49.5389588881306473266797650392" },
                    { 2157, "49.5536126441174222769819975597" },
                    { 2158, "49.5589480002588713416617635277" },
                    { 2159, "49.5601003179238192531111467381" },
                    { 2160, "49.5622269720465027752361817514" },
                    { 2161, "49.5380436799287901640804003315" },
                    { 2162, "49.5954034435010320968451424305" },
                    { 2163, "49.6026587836302789427081553642" },
                    { 2164, "49.6334021895183087590051580143" },
                    { 2165, "49.6182384586253802847398569905" },
                    { 2166, "49.6287208291919080020304123388" },
                    { 2167, "49.6326348005699336827966522105" },
                    { 2168, "49.6447481616083396642648995856" },
                    { 2169, "49.6567727014908328743667108792" },
                    { 2170, "49.6705659890119769536599769546" },
                    { 2171, "49.6788065923729936996495433200" },
                    { 2172, "49.6827478747489704618616912472" },
                    { 2173, "49.6947248856929148727838794095" },
                    { 2174, "49.7061952167660068342496488699" },
                    { 2175, "49.7231860107352010547308867794" },
                    { 2176, "49.7413864943108363683580028350" },
                    { 2177, "49.7420029030717082353145253774" },
                    { 2178, "49.7466712258283545996787669320" },
                    { 2179, "49.7574218715892061728037253484" },
                    { 2180, "49.7620875760778533902861830865" },
                    { 2181, "49.7659112370496534978011672860" },
                    { 2182, "49.7737316432662902272616606115" },
                    { 2183, "49.7868758517932521173419751800" },
                    { 2184, "49.7966814307571949218352989319" },
                    { 2185, "49.7968617799427216538295217823" },
                    { 2186, "49.8091205874218983292980055659" },
                    { 2187, "49.8171498347597927307125291160" },
                    { 2188, "49.8351885141378286543706650329" },
                    { 2189, "49.8403894461309300862687370820" },
                    { 2190, "49.8537885434656168795111725145" },
                    { 2191, "49.8816281140265824558451897221" },
                    { 2192, "49.8949115851804785188296951467" },
                    { 2193, "49.8953203417538285680315839746" },
                    { 2194, "49.8990404301067704861866852162" },
                    { 2195, "49.9217191609118674991807009633" },
                    { 2196, "49.9285159776623955182611840717" },
                    { 2197, "49.9314772862439335349430723435" },
                    { 2198, "49.9541203710293488828009144652" },
                    { 2199, "49.9734161676354442092343069306" },
                    { 2200, "49.9835288543341660351123716001" },
                    { 2201, "49.9970977215331537728717387090" },
                    { 2202, "50.0185870506183368918415982149" },
                    { 2203, "50.0299774875768786204261008839" },
                    { 2204, "50.0320800131353249175221222106" },
                    { 2205, "50.0421785208172661531348779129" },
                    { 2206, "50.0459002427265513579158767228" },
                    { 2207, "50.0469557109351239157414892591" },
                    { 2208, "50.0571062187234424233350094872" },
                    { 2209, "50.0527967734093451535313340393" },
                    { 2210, "50.0961950916280866837134038126" },
                    { 2211, "50.1079817997980380738297428497" },
                    { 2212, "50.1310903076524571008046755158" },
                    { 2213, "50.1412888542222155055098607476" },
                    { 2214, "50.1508093688019030391263475102" },
                    { 2215, "50.1620859781602328199156030665" },
                    { 2216, "50.1996671075604168293673632326" },
                    { 2217, "50.2059896699208660596052957967" },
                    { 2218, "50.2261437193139770123463020747" },
                    { 2219, "50.2263530022474340774267542027" },
                    { 2220, "50.2321448796501887280099868704" },
                    { 2221, "50.2369215747256931060736702583" },
                    { 2222, "50.2655414620553319357548921046" },
                    { 2223, "50.2805717898316059241045964661" },
                    { 2224, "50.2895156581342724178047911430" },
                    { 2225, "50.3074050475740390620098974677" },
                    { 2226, "50.3224304807709672795527916322" },
                    { 2227, "50.3274507407699542476297514270" },
                    { 2228, "50.3466204784091327357818234972" },
                    { 2229, "50.3633489895845003163921118297" },
                    { 2230, "50.3761369819229230978817652816" },
                    { 2231, "50.3919585549626003959012617923" },
                    { 2232, "50.3928795020284152515476247114" },
                    { 2233, "50.3821759096381853997984866654" },
                    { 2234, "50.4258548951731950681305439080" },
                    { 2235, "50.4548007032533064544512015840" },
                    { 2236, "50.4658504505618521220375123440" },
                    { 2237, "50.4790329653458833029223636077" },
                    { 2238, "50.4815508062943091843564959774" },
                    { 2239, "50.4880564507159137012703374121" },
                    { 2240, "50.4885243521153897323467215542" },
                    { 2241, "50.5048443441047112612730309968" },
                    { 2242, "50.5054039511160117165199186836" },
                    { 2243, "50.5102191271284124361614369624" },
                    { 2244, "50.5146475560024316064662346728" },
                    { 2245, "50.5214487203265991372584031571" },
                    { 2246, "50.5508327620536419461205841764" },
                    { 2247, "50.5569128285034940651803010781" },
                    { 2248, "50.5680245083003090759685234495" },
                    { 2249, "50.5698287789531025507920784781" },
                    { 2250, "50.5811876504064855741437977344" },
                    { 2251, "50.5863397013914343778964598282" },
                    { 2252, "50.6001061320966832187084960052" },
                    { 2253, "50.6202100186040426997698386466" },
                    { 2254, "50.6438870250376494384408135997" },
                    { 2255, "50.6514214450269702937240765334" },
                    { 2256, "50.6539661980574728342609153160" },
                    { 2257, "50.6801280788455957118416541992" },
                    { 2258, "50.6883349654612284973371657779" },
                    { 2259, "50.6926432177173443743534015539" },
                    { 2260, "50.7190378432971285542367901666" },
                    { 2261, "50.7245581894063570630991380229" },
                    { 2262, "50.7301729651728249058369192420" },
                    { 2263, "50.7361793607552261203350528386" },
                    { 2264, "50.7368050885247716057544818215" },
                    { 2265, "50.7487942896631482302327851016" },
                    { 2266, "50.7519023195528858557872518761" },
                    { 2267, "50.7665169833788531001070246174" },
                    { 2268, "50.7889552199385128406922439000" },
                    { 2269, "50.8135484177400855126176091802" },
                    { 2270, "50.8176084437277363752207972377" },
                    { 2271, "50.8259082880871531686501118267" },
                    { 2272, "50.8384824563492582904514176448" },
                    { 2273, "50.8563565298138968682164646420" },
                    { 2274, "50.8615275321872177433850743415" },
                    { 2275, "50.8860765352200154275521403911" },
                    { 2276, "50.8972917734544279618183329561" },
                    { 2277, "50.9348094050462059312049949957" },
                    { 2278, "50.9390461092801243902102491343" },
                    { 2279, "51.0384843730848357348841006056" },
                    { 2280, "50.9534507657398143853650103014" },
                    { 2281, "50.9532820785294932925632451515" },
                    { 2282, "50.9610105595038076513338675578" },
                    { 2283, "50.9698628386834049789897922611" },
                    { 2284, "50.9914717238860187376675493358" },
                    { 2285, "51.0666703169508348119308457511" },
                    { 2286, "51.0719459620383981570845950635" },
                    { 2287, "51.0218106489291610592048824945" },
                    { 2288, "51.0370786626558869521313113912" },
                    { 2289, "51.1041820659395784904736180983" },
                    { 2290, "51.1131797634349860105823353761" },
                    { 2291, "51.1275997459431597963963787036" },
                    { 2292, "51.0582169397572711188761022543" },
                    { 2293, "51.0756759991455886977922230818" },
                    { 2294, "51.0956494628672663806165980786" },
                    { 2295, "51.1076336508495564389929099259" },
                    { 2296, "51.1192447518119839768033780724" },
                    { 2297, "51.1347870636820526811997933890" },
                    { 2298, "51.1377651835469425862763943288" },
                    { 2299, "51.1541420611389999565292235884" },
                    { 2300, "51.1667380567094690680127513161" },
                    { 2301, "51.1981957334317949799891753010" },
                    { 2302, "51.2166658746550499479913294807" },
                    { 2303, "51.1960421560942028170380531547" },
                    { 2304, "51.2149983286017604497795531645" },
                    { 2305, "51.2262218358506507335051697000" },
                    { 2306, "51.2373356707192468455452808122" },
                    { 2307, "51.2489140520457598154456951074" },
                    { 2308, "51.2984046345197210402395975030" },
                    { 2309, "51.3119507798198861414528865822" },
                    { 2310, "51.3283467997253110986435434456" },
                    { 2311, "51.3457362720062359867423313007" },
                    { 2312, "51.3544712897143270354618295574" },
                    { 2313, "51.3490742652119314080813263531" },
                    { 2314, "51.3245953373843798505726278529" },
                    { 2315, "51.3340877978453028424638771206" },
                    { 2316, "51.3403160082736565664149244495" },
                    { 2317, "51.3620602608991461450047543235" },
                    { 2318, "51.3710562595838324185139720128" },
                    { 2319, "51.3789486912722655078569924876" },
                    { 2320, "51.3852218346405867983670653547" },
                    { 2321, "51.4008594162278401959509774845" },
                    { 2322, "51.3953637065978507871489290236" },
                    { 2323, "51.4015599499043460526210120158" },
                    { 2324, "51.4053552225092110127801069880" },
                    { 2325, "51.4267026967792028234533907749" },
                    { 2326, "51.4341337042762698985631496062" },
                    { 2327, "51.4399105291646140267832194572" },
                    { 2328, "51.4369623255817769218252553838" },
                    { 2329, "51.4372617194541064252052375393" },
                    { 2330, "51.4451459124343636969229586894" },
                    { 2331, "51.4381280003573495100928678014" },
                    { 2332, "51.4394186678511498116495375544" },
                    { 2333, "51.4480712347457068392612778004" },
                    { 2334, "51.4405859173778689695072507500" },
                    { 2335, "51.4393165296335400029169759945" },
                    { 2336, "51.4832486417737249180370458329" },
                    { 2337, "51.5143721472538356267185699578" },
                    { 2338, "51.5255745600564025739726733523" },
                    { 2339, "51.5369138492017487220669363799" },
                    { 2340, "51.5586297765463688261504016011" },
                    { 2341, "51.5657502428414322824997870976" },
                    { 2342, "51.5849191226379398301761939937" },
                    { 2343, "51.5967066329261557747912246734" },
                    { 2344, "51.6064544076324985855230383504" },
                    { 2345, "51.6095796161070868464130119234" },
                    { 2346, "51.6107030217459849048479632431" },
                    { 2347, "51.6124396766817658658601317390" },
                    { 2348, "51.6274229316424406343916451090" },
                    { 2349, "51.6280878518378313766908007204" },
                    { 2350, "51.6533868852743077410880538208" },
                    { 2351, "51.6798969064143015529191684863" },
                    { 2352, "51.6864667835242109466566956872" },
                    { 2353, "51.7114383243430811122812595915" },
                    { 2354, "51.7286221321438135626330450995" },
                    { 2355, "51.7051781168503854287958529760" },
                    { 2356, "51.7184716596426729534108819307" },
                    { 2357, "51.7291002363781875955010249496" },
                    { 2358, "51.7741780465558669611258430063" },
                    { 2359, "51.7780532962811412098410377943" },
                    { 2360, "51.7883211193620362803607406747" },
                    { 2361, "51.7946456580684522179738280465" },
                    { 2362, "51.7965360076726238634623549463" },
                    { 2363, "51.8010624721137713897876869635" },
                    { 2364, "51.7973739153417789525018843099" },
                    { 2365, "51.8243579684595822039010503978" },
                    { 2366, "51.8267791992072079657357032618" },
                    { 2367, "51.8285768214343515923731092644" },
                    { 2368, "51.8475105450679201579165638395" },
                    { 2369, "51.8552980047665271359426847723" },
                    { 2370, "51.8621572533465143509565312676" },
                    { 2371, "51.8629643933649588563954029576" },
                    { 2372, "51.8801152133969948006851380404" },
                    { 2373, "51.8902890495801863280782834767" },
                    { 2374, "51.9055259276451427323659025535" },
                    { 2375, "51.9155335872957528269115555159" },
                    { 2376, "51.9168440900741701547980442618" },
                    { 2377, "51.9274020926431412733822977248" },
                    { 2378, "51.9376152996016679325507444284" },
                    { 2379, "51.9473801134696007627153444249" },
                    { 2380, "51.9606300695170686782462394021" },
                    { 2381, "51.9626552488245599354180924206" },
                    { 2382, "51.9641282109684691311123377832" },
                    { 2383, "51.9649437441367685478222565522" },
                    { 2384, "52.0103278831370496541039663753" },
                    { 2385, "52.0324357247419853425823005522" },
                    { 2386, "52.0618681748365345386114998799" },
                    { 2387, "52.0910346513422360035690674700" },
                    { 2388, "52.1099375211249343044051468253" },
                    { 2389, "52.1199057334541338375692329036" },
                    { 2390, "52.1322302252316187427172988988" },
                    { 2391, "52.1699718870071363519221000443" },
                    { 2392, "52.1637973145701136802706031787" },
                    { 2393, "52.1619993683189458053197547684" },
                    { 2394, "52.1903399334929152482970154577" },
                    { 2395, "52.1904659719617602426886927158" },
                    { 2396, "52.2066692009966312415668275830" },
                    { 2397, "52.2296547513049061212068125055" },
                    { 2398, "52.2345771371086511243473607819" },
                    { 2399, "52.2398874373074430895361232380" },
                    { 2400, "52.2549902600530408129377126604" },
                    { 2401, "52.2622004958003998675454951296" },
                    { 2402, "52.2726834393039799887515905008" },
                    { 2403, "52.2789946664612222830569454760" },
                    { 2404, "52.2785476859934155437066101648" },
                    { 2405, "52.2870099100732359495895633568" },
                    { 2406, "52.2969394254454425843575620919" },
                    { 2407, "52.2845142700181313871734295653" },
                    { 2408, "52.3300103364158369615741097808" },
                    { 2409, "52.3542404775595753984552030892" },
                    { 2410, "52.3719392372498957035681631582" },
                    { 2411, "52.3799171012585723343304179643" },
                    { 2412, "52.3425770106791386959910222530" },
                    { 2413, "52.4164107407912897150460029434" },
                    { 2414, "52.4402379729892924689280705529" },
                    { 2415, "52.3975597483364118119895977933" },
                    { 2416, "52.4612505769769276807479744031" },
                    { 2417, "52.4675575740811578080660364252" },
                    { 2418, "52.4173249792586456267560420417" },
                    { 2419, "52.4854067868939310186768137075" },
                    { 2420, "52.5302986926263516656940840416" },
                    { 2421, "52.4542930097015751688069229604" },
                    { 2422, "52.4650438237144034715968720121" },
                    { 2423, "52.5521926167819719507582414937" },
                    { 2424, "52.5727705414521537974135739201" },
                    { 2425, "52.5804193748953071190672380628" },
                    { 2426, "52.5169431951224916060086864827" },
                    { 2427, "52.5943900372790588408977487400" },
                    { 2428, "52.6074163714332575752868787326" },
                    { 2429, "52.6232839251736373691411449778" },
                    { 2430, "52.6353842875969473223348460936" },
                    { 2431, "52.6237173466805813560349315649" },
                    { 2432, "52.6078914433169036813966882477" },
                    { 2433, "52.6800987512054769550915690814" },
                    { 2434, "52.7138740533150518132312004814" },
                    { 2435, "52.7220871582378657120335926451" },
                    { 2436, "52.7314522040507700679101825837" },
                    { 2437, "52.6872008793318774558779365038" },
                    { 2438, "52.7671815273026184715921678420" },
                    { 2439, "52.7640071468587132697175387857" },
                    { 2440, "52.6782778282732421021372350009" },
                    { 2441, "52.8005462977821518155038944191" },
                    { 2442, "52.8053434553816612367147694302" },
                    { 2443, "52.8119056244984616727501146220" },
                    { 2444, "52.7998261490759320824286828310" },
                    { 2445, "52.8227375697245724613321771530" },
                    { 2446, "52.8314917684042862652525381121" },
                    { 2447, "52.8282112267708249823397103255" },
                    { 2448, "52.8291664989136411410768374104" },
                    { 2449, "52.7984781098771521487743838159" },
                    { 2450, "52.8440268226981635970856065684" },
                    { 2451, "52.8627816349971228372649278196" },
                    { 2452, "52.8522872191937883758672209404" },
                    { 2453, "52.8581600531077375190784050903" },
                    { 2454, "52.8542731449345493267570080184" },
                    { 2455, "52.8622023619636567640592701109" },
                    { 2456, "52.8977616114090320030997567707" },
                    { 2457, "52.8753427275393727362374897209" },
                    { 2458, "52.8883242955297074441380820769" },
                    { 2459, "52.9066796836321264676522072389" },
                    { 2460, "52.9166796145791851511560512531" },
                    { 2461, "52.9187482213080095850899571871" },
                    { 2462, "52.9171107001332591235126400812" },
                    { 2463, "52.9296777880385498425376420310" },
                    { 2464, "52.9524103719816405155890518137" },
                    { 2465, "52.9635773347522324985632499700" },
                    { 2466, "52.9679202983550184547529626073" },
                    { 2467, "52.9693060538851927053684968502" },
                    { 2468, "52.9746736214229439970925056272" },
                    { 2469, "52.9998961363990594845955309798" },
                    { 2470, "53.0082660555543989568509267152" },
                    { 2471, "53.0105524052160549671823336319" },
                    { 2472, "53.0205672714616224198772484077" },
                    { 2473, "53.0207078041216609899130999222" },
                    { 2474, "53.0185459492116944246388438955" },
                    { 2475, "53.0304540464122729479135775405" },
                    { 2476, "53.0349373795028851022465495331" },
                    { 2477, "53.0366252871749272417200296965" },
                    { 2478, "53.0373703891337354034202026629" },
                    { 2479, "53.0227441333950547093303133268" },
                    { 2480, "53.0676734070792101867635188022" },
                    { 2481, "53.0986553775804587422931157365" },
                    { 2482, "53.0980550043815834420906990585" },
                    { 2483, "53.1033172904658206887395155369" },
                    { 2484, "53.1221104277457267126781587822" },
                    { 2485, "53.1227613597922007021766791122" },
                    { 2486, "53.1365177604222373089144580784" },
                    { 2487, "53.1414239992080686918062213013" },
                    { 2488, "53.1432944599304095272413427775" },
                    { 2489, "53.1464138943028372713173313663" },
                    { 2490, "53.1464674083820330045797228790" },
                    { 2491, "53.1458726039968391976316517738" },
                    { 2492, "53.1724776723564798904777618169" },
                    { 2493, "53.1810685925004385546256136735" },
                    { 2494, "53.2019538350257129376414168621" },
                    { 2495, "53.2167915607082905153956636714" },
                    { 2496, "53.2196436384030926449715914061" },
                    { 2497, "53.2261875416867799689948721896" },
                    { 2498, "53.2271250790571642707744241680" },
                    { 2499, "53.2289151271604530570041966659" },
                    { 2500, "53.2318152006335493142551439545" },
                    { 2501, "53.2321687249402556790427164370" },
                    { 2502, "53.2343224343619961069575503171" },
                    { 2503, "53.2346508140998591207587602918" },
                    { 2504, "53.2737134245190428572027379602" },
                    { 2505, "53.2856904905376715467734161186" },
                    { 2506, "53.3277060740499758296690484774" },
                    { 2507, "53.3327120162194318246037355009" },
                    { 2508, "53.3361244209294610277550090084" },
                    { 2509, "53.3555195415842080089743260986" },
                    { 2510, "53.3613718183941213655194499330" },
                    { 2511, "53.3766137404288610533788014657" },
                    { 2512, "53.4062662857662467041568143561" },
                    { 2513, "53.4045541110122441281239948688" },
                    { 2514, "53.4132058751871129686022892579" },
                    { 2515, "53.4055050986090107841674759850" },
                    { 2516, "53.4187559544858807571930819892" },
                    { 2517, "53.4281154606663958941697613686" },
                    { 2518, "53.4388323091886520073702760481" },
                    { 2519, "53.4465781332926952385602638365" },
                    { 2520, "53.4610812090246219428015869794" },
                    { 2521, "53.4612893405330693305618986812" },
                    { 2522, "53.4739528912304944904483926774" },
                    { 2523, "53.4764855616829898828070273245" },
                    { 2524, "53.4877909873278423788662974192" },
                    { 2525, "53.4888163440876138855536651664" },
                    { 2526, "53.5006704608141475196882961832" },
                    { 2527, "53.4911748661382705318523646333" },
                    { 2528, "53.5282974754761968764968641037" },
                    { 2529, "53.5930702744992177696988691527" },
                    { 2530, "53.5911451473762899399741534821" },
                    { 2531, "53.6316872735167149970657548837" },
                    { 2532, "53.6385929100686084968702068258" },
                    { 2533, "53.6588179507478139788160407168" },
                    { 2534, "53.6720585316433411676712923660" },
                    { 2535, "53.6749019720587480267526579606" },
                    { 2536, "53.6697230089939898453056011408" },
                    { 2537, "53.6927246560737619201444111525" },
                    { 2538, "53.7032186831166545250925631030" },
                    { 2539, "53.7129615870502653770969922009" },
                    { 2540, "53.7175778012661280811177887397" },
                    { 2541, "53.7226022389728207768437403453" },
                    { 2542, "53.7176968770096084527641156669" },
                    { 2543, "53.7416656529898674114855084248" },
                    { 2544, "53.7443046397326069244145884841" },
                    { 2545, "53.7443654544958674840429099042" },
                    { 2546, "53.7667548284124585554202955217" },
                    { 2547, "53.7787127347730504204590036539" },
                    { 2548, "53.7957790799289956391315126625" },
                    { 2549, "53.7907841538496299967013943164" },
                    { 2550, "53.8025591935806380258284741164" },
                    { 2551, "53.8094440573072274009207625038" },
                    { 2552, "53.8346784192173821640740720713" },
                    { 2553, "53.8575534012739738127341190445" },
                    { 2554, "53.8625762715415571734362713149" },
                    { 2555, "53.8685077736414896170015443801" },
                    { 2556, "53.8784322504944267968836736428" },
                    { 2557, "53.8844115005212016847006917014" },
                    { 2558, "53.8982941911536963828171499482" },
                    { 2559, "53.8949575536263661714625305680" },
                    { 2560, "53.9233484086938800995594378720" },
                    { 2561, "53.9235083987898525906555118477" },
                    { 2562, "53.9282800647637247493306891435" },
                    { 2563, "53.9356478970029775792916010118" },
                    { 2564, "53.9592516406375661271659025488" },
                    { 2565, "53.9779911769519377135382768037" },
                    { 2566, "53.9910711167914864849097330988" },
                    { 2567, "54.0226906608042890110508029591" },
                    { 2568, "54.0482306481621762233626675461" },
                    { 2569, "54.0558042545172150886202839065" },
                    { 2570, "54.0692917765766730849901521890" },
                    { 2571, "54.1025786863994141635700062714" },
                    { 2572, "54.1163042381895280528270775182" },
                    { 2573, "54.1194278047423949087949899530" },
                    { 2574, "54.1415858342173622405285658924" },
                    { 2575, "54.1193733464738039304872699720" },
                    { 2576, "54.1609650455764390497996030483" },
                    { 2577, "54.1904813700946694102206860455" },
                    { 2578, "54.1933431738744236218858573301" },
                    { 2579, "54.1907837260470504272621708997" },
                    { 2580, "54.2083053001211895462691857047" },
                    { 2581, "54.2127968422379558017834618760" },
                    { 2582, "54.2085803424301369706237045823" },
                    { 2583, "54.2185075351744707384049886319" },
                    { 2584, "54.2249612095429727430284236622" },
                    { 2585, "54.2369874062005395468214055356" },
                    { 2586, "54.2411165782380010297937529397" },
                    { 2587, "54.2452398601386795960581810908" },
                    { 2588, "54.2601630738024215890814655717" },
                    { 2589, "54.2864523628984310619731656167" },
                    { 2590, "54.3260922387696963526897118200" },
                    { 2591, "54.3257868186664294612000283214" },
                    { 2592, "54.3580271278410765353230154353" },
                    { 2593, "54.3902874765864510324306861737" },
                    { 2594, "54.3611237509454975173819106150" },
                    { 2595, "54.4192831114307561590294013863" },
                    { 2596, "54.3886640313451425067649719223" },
                    { 2597, "54.3980450017879686960650501299" },
                    { 2598, "54.4135111919993140251167582219" },
                    { 2599, "54.4258653244973573702538099284" },
                    { 2600, "54.5074157778554356289528464947" },
                    { 2611, "54.5207413682757693852392113525" },
                    { 2623, "54.6179797167200718479997054214" },
                    { 2635, "54.6985939242156433775672449788" },
                    { 2647, "54.7838414383624935385058614522" },
                    { 2653, "54.7939756964600140919170590752" },
                    { 2665, "54.9501960000749381255574094061" },
                    { 2677, "55.0318492410936920804469016028" },
                    { 2689, "55.1435522434324292596258184378" },
                    { 2713, "55.4166889353596146216325380716" },
                    { 2725, "55.6442836221762677526151249636" },
                    { 2737, "55.7327432551392074758205720158" },
                    { 2749, "55.9035909243632558692943253349" },
                    { 2773, "56.2073510196711045673275171010" },
                    { 2779, "56.2272281528993082121653232589" },
                    { 2791, "56.2906321525655505858856194751" },
                    { 2803, "56.4294997250505509601677809750" },
                    { 2815, "56.4802985918147569407123077829" },
                    { 2839, "56.6665449686158079594800781089" },
                    { 2857, "56.8791365165998814973037239432" },
                    { 2869, "56.9845157521568251207034826854" },
                    { 2893, "57.2167586448541382933608346401" },
                    { 2905, "57.3868790482294833534498206381" },
                    { 2917, "57.5352068111280692964201044828" },
                    { 2929, "57.6856982538293596937668916196" },
                    { 2941, "57.8639716579257382425932763483" },
                    { 2965, "58.0125874043809604153325785323" },
                    { 2989, "58.2019972520585731103632522231" },
                    { 3000, "58.2551206109639611446466425894" },
                    { 3001, "58.2351887554945423639084579256" },
                    { 3013, "58.4079117190109049716763795421" },
                    { 3025, "58.4968472137225366672802423895" },
                    { 3037, "58.5758055336159892799900867244" },
                    { 3049, "58.7499624924638544492341486420" },
                    { 3055, "58.7686144007434717614636318528" },
                    { 3067, "58.9250646046142173676583329109" },
                    { 3079, "59.0344713737635926687127606379" },
                    { 3091, "59.1315482455709023583751045991" },
                    { 3103, "59.3527297477820136944764512289" },
                    { 3115, "59.4269378776850994939380600342" },
                    { 3121, "59.5107718124568728849486261841" },
                    { 3145, "59.7377316938569225157927950300" },
                    { 3169, "59.9102146484889493200802261581" },
                    { 3181, "59.9748315328693610853435592689" },
                    { 3193, "60.0505217489591721350836640155" },
                    { 3205, "60.1545735800907452259421697795" },
                    { 3217, "60.2968039329937325145391377245" },
                    { 3241, "60.5432605378031808311770941232" },
                    { 3253, "60.6319837562995066808416390612" },
                    { 3259, "60.6624379770589659712478483104" },
                    { 3283, "60.9861187215992192446069438587" },
                    { 3295, "61.0684386748707796055970964103" },
                    { 3307, "61.2936648397585162458295981744" },
                    { 3319, "61.3734896364555281251664185267" },
                    { 3331, "61.4330943528640770699758366139" },
                    { 3343, "61.5780955672987894781031211579" },
                    { 3355, "61.6646911269992200911878254163" },
                    { 3367, "61.7174566947834089704955588761" },
                    { 3403, "61.9522323033551576232453460700" },
                    { 3415, "62.0867158369586129428825823263" },
                    { 3427, "62.1763943629367841034933594432" },
                    { 3439, "62.3262154351111165467568636868" },
                    { 3463, "62.5737092407912518906285330974" },
                    { 3481, "62.8022085607463559396565890967" },
                    { 3493, "62.9306842941474675199167335750" },
                    { 3505, "63.0357623442688057701982069068" },
                    { 3511, "63.0519466190870573985954710024" },
                    { 3535, "63.3211351069767002282774851757" },
                    { 3547, "63.3604849321778137518889418416" },
                    { 3559, "63.3850178250819007914952281402" },
                    { 3571, "63.4883364937202947065462885046" },
                    { 3595, "63.7250811254008032316248155987" },
                    { 3607, "63.8028501557638183533732286310" },
                    { 3619, "63.8927918925611909084704191804" },
                    { 3631, "64.0363233766662218166774110929" },
                    { 3643, "64.0990574961046001180173299071" },
                    { 3655, "64.2935977940859434510707946414" },
                    { 3667, "64.3701856898453160506133011085" },
                    { 3679, "64.5277386638594027673415371035" },
                    { 3691, "64.6637738914535880711783457188" },
                    { 3697, "64.6864270605494698319771008648" },
                    { 3721, "64.9143821149893547504714970873" },
                    { 3745, "65.0454966623419297478150458072" },
                    { 3757, "65.1378188054526886081329375463" },
                    { 3781, "65.3274603326337471925330043856" },
                    { 3793, "65.3646035586252062389235593621" },
                    { 3805, "65.5662490844444345980636372288" },
                    { 3817, "65.6550683799212893978710737066" },
                    { 3829, "65.7183244202739152655012221517" },
                    { 3853, "65.9392518141871323356769545294" },
                    { 3865, "66.0458514634616225859971663162" },
                    { 3877, "66.1865365501334361154544940118" },
                    { 3889, "66.2957240153611560903780202541" },
                    { 3901, "66.4790253829509326521578333718" },
                    { 3919, "66.6063081797261186715902775251" },
                    { 3931, "66.7023742821090169158644510398" },
                    { 3943, "66.8236545936866935762716598220" },
                    { 3949, "66.8205193586814746585679573200" },
                    { 3973, "66.9488696643170493962994178885" },
                    { 3985, "67.0183852547403242950970922639" },
                    { 4000, "67.1802524874904122812080003785" },
                    { 4009, "67.2114250939643225101803600140" },
                    { 4021, "67.3161328366920804052080967710" },
                    { 4033, "67.4431262095666796961500698921" },
                    { 4045, "67.5979271295688653903889945059" },
                    { 4057, "67.6921519481733344430226700483" },
                    { 4069, "67.7777638662386835402839999677" },
                    { 4081, "67.8855503040025907466215826518" },
                    { 4093, "68.0200019507492706428665462689" },
                    { 4105, "68.0943624408674712093189213389" },
                    { 4117, "68.2336335096233472534051347055" },
                    { 4141, "68.4849148429885446836277639541" },
                    { 4153, "68.5099392497664121404189411011" },
                    { 4177, "68.6292972121900190560325132847" },
                    { 4189, "68.7488168250449256600086201947" },
                    { 4195, "68.7562644558692859846241691691" },
                    { 4219, "68.9743613560810983557218349104" },
                    { 4231, "69.0271726925885962432412078755" },
                    { 4243, "69.1016100317258046371742497711" },
                    { 4255, "69.2510246493434638392591262721" },
                    { 4267, "69.3382702284971098123560571874" },
                    { 4303, "69.7210284588283435912382640827" },
                    { 4315, "69.8151362687987840948421239997" },
                    { 4339, "70.0329328913477079135265700031" },
                    { 4345, "70.0612349492516376185197257329" },
                    { 4357, "70.1499849651572935449933983468" },
                    { 4381, "70.2923490333340243504921528377" },
                    { 4405, "70.4711103891702741496972107754" },
                    { 4417, "70.5197292903446632657032404282" },
                    { 4429, "70.6204059084202476213175489152" },
                    { 4447, "70.8067134488447343389289125048" },
                    { 4459, "70.8759219446374501516552531598" },
                    { 4471, "70.9841875538697908937447342419" },
                    { 4483, "71.0282766674149106170781367947" },
                    { 4495, "71.1842197986180331561526382762" },
                    { 5000, "75.0563585417160563539572351900" },
                };

                for (int n = 50; n < 250; ++n) { File.WriteAllText($"n{n}.txt", $"{n} {bks[n]} 1e-15"); }
                for (int n = 950; n < 1050; ++n) { File.WriteAllText($"n{n}.txt", $"{n} {bks[n]} 1e-15"); }
                for (int n = 1950; n < 2050; ++n) { File.WriteAllText($"n{n}.txt", $"{n} {bks[n]} 1e-15"); }
            }
        }

        public class DARP2d {
            public static void convertAll() {
                convertPR("DARP/Instance/pr01");
                convertPR("DARP/Instance/pr02");
                convertPR("DARP/Instance/pr03");
                convertPR("DARP/Instance/pr04");
                convertPR("DARP/Instance/pr05");
                convertPR("DARP/Instance/pr06");
                convertPR("DARP/Instance/pr07");
                convertPR("DARP/Instance/pr08");
                convertPR("DARP/Instance/pr09");
                convertPR("DARP/Instance/pr10");
                convertPR("DARP/Instance/pr11");
                convertPR("DARP/Instance/pr12");
                convertPR("DARP/Instance/pr13");
                convertPR("DARP/Instance/pr14");
                convertPR("DARP/Instance/pr15");
                convertPR("DARP/Instance/pr16");
                convertPR("DARP/Instance/pr17");
                convertPR("DARP/Instance/pr18");
                convertPR("DARP/Instance/pr19");
                convertPR("DARP/Instance/pr20");
                convertAB("DARP/Instance/a2-16");
                convertAB("DARP/Instance/a2-20");
                convertAB("DARP/Instance/a2-24");
                convertAB("DARP/Instance/a3-18");
                convertAB("DARP/Instance/a3-24");
                convertAB("DARP/Instance/a3-30");
                convertAB("DARP/Instance/a3-36");
                convertAB("DARP/Instance/a4-16");
                convertAB("DARP/Instance/a4-24");
                convertAB("DARP/Instance/a4-32");
                convertAB("DARP/Instance/a4-40");
                convertAB("DARP/Instance/a4-48");
                convertAB("DARP/Instance/a5-40");
                convertAB("DARP/Instance/a5-50");
                convertAB("DARP/Instance/a5-60");
                convertAB("DARP/Instance/a6-48");
                convertAB("DARP/Instance/a6-60");
                convertAB("DARP/Instance/a6-72");
                convertAB("DARP/Instance/a7-56");
                convertAB("DARP/Instance/a7-70");
                convertAB("DARP/Instance/a7-84");
                convertAB("DARP/Instance/a8-64");
                convertAB("DARP/Instance/a8-80");
                convertAB("DARP/Instance/a8-96");
                convertAB("DARP/Instance/b2-16");
                convertAB("DARP/Instance/b2-20");
                convertAB("DARP/Instance/b2-24");
                convertAB("DARP/Instance/b3-18");
                convertAB("DARP/Instance/b3-24");
                convertAB("DARP/Instance/b3-30");
                convertAB("DARP/Instance/b3-36");
                convertAB("DARP/Instance/b4-16");
                convertAB("DARP/Instance/b4-24");
                convertAB("DARP/Instance/b4-32");
                convertAB("DARP/Instance/b4-40");
                convertAB("DARP/Instance/b4-48");
                convertAB("DARP/Instance/b5-40");
                convertAB("DARP/Instance/b5-50");
                convertAB("DARP/Instance/b5-60");
                convertAB("DARP/Instance/b6-48");
                convertAB("DARP/Instance/b6-60");
                convertAB("DARP/Instance/b6-72");
                convertAB("DARP/Instance/b7-56");
                convertAB("DARP/Instance/b7-70");
                convertAB("DARP/Instance/b7-84");
                convertAB("DARP/Instance/b8-64");
                convertAB("DARP/Instance/b8-80");
                convertAB("DARP/Instance/b8-96");
            }

            static void convertPR(string oldPath) {
                string[] lines = File.ReadAllLines(oldPath + ".txt");

                string[] words = lines[0].Split(Checker.InlineDelimiters, StringSplitOptions.RemoveEmptyEntries);
                int requestNum = int.Parse(words[1]) / 2;
                string vehicleNum = words[0];
                string vehicleCapacity = words[3];
                string maxTravelTime = words[2];
                string maxRideTime = words[4];

                StringBuilder sb = new StringBuilder();
                for (int l = 1; l < lines.Length; ++l) {
                    words = lines[l].Split(Checker.InlineDelimiters, StringSplitOptions.RemoveEmptyEntries);
                    if (words.Length < 7) { continue; }
                    sb.Append(words[1]).Append('\t').Append(words[2]).Append('\t')
                        .Append(words[4]).Append('\t').Append(words[3]).Append('\t')
                        .Append(words[5]).Append('\t').AppendLine(words[6]);
                }

                StringBuilder header = new StringBuilder();
                header.Append(requestNum).Append(' ').Append(vehicleNum).Append(' ').Append(vehicleCapacity).Append(' ').Append(maxTravelTime).Append(' ').AppendLine(maxRideTime);
                StringBuilder fn = new StringBuilder();
                fn.Append(".r").Append(requestNum).Append('v').Append(vehicleNum).Append('c').Append(vehicleCapacity).Append('t').Append(maxTravelTime).Append('l').Append(maxRideTime).Append(".txt");
                File.WriteAllText(oldPath + fn.ToString(), header.ToString() + sb.ToString());
            }
            static void convertAB(string oldPath) {
                string[] lines = File.ReadAllLines(oldPath + ".txt");

                string[] words = lines[0].Split(Checker.InlineDelimiters, StringSplitOptions.RemoveEmptyEntries);
                int requestNum = int.Parse(words[1]);
                string vehicleNum = words[0];
                string vehicleCapacity = words[3];
                string maxTravelTime = words[2];
                string maxRideTime = words[4];

                StringBuilder sb = new StringBuilder();
                for (int l = 1, i = 2 * requestNum + 1; (l < lines.Length) && (i-- > 0); ++l) {
                    words = lines[l].Split(Checker.InlineDelimiters, StringSplitOptions.RemoveEmptyEntries);
                    if (words.Length < 7) { continue; }
                    sb.Append(words[1]).Append('\t').Append(words[2]).Append('\t')
                        .Append(words[4]).Append('\t').Append(words[3]).Append('\t')
                        .Append(words[5]).Append('\t').AppendLine(words[6]);
                }
                string[] depot0 = lines[1].Split(Checker.InlineDelimiters, StringSplitOptions.RemoveEmptyEntries);
                string[] depot1 = lines[lines.Length - 1].Split(Checker.InlineDelimiters, StringSplitOptions.RemoveEmptyEntries);
                for (int i = 1; i < depot0.Length; ++i) {
                    if (depot0[i] != depot1[i]) { Console.WriteLine("two depots not matched"); break; }
                }

                StringBuilder header = new StringBuilder();
                header.Append(requestNum).Append(' ').Append(vehicleNum).Append(' ').Append(vehicleCapacity).Append(' ').Append(maxTravelTime).Append(' ').AppendLine(maxRideTime);
                StringBuilder fn = new StringBuilder();
                fn.Append(".r").Append(requestNum).Append('v').Append(vehicleNum).Append('c').Append(vehicleCapacity).Append('t').Append(maxTravelTime).Append('l').Append(maxRideTime).Append(".txt");
                File.WriteAllText(oldPath + fn.ToString(), header.ToString() + sb.ToString());
            }
        }
    }
}
