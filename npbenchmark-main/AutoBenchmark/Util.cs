using MimeKit;
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Net.Mail;
using System.Runtime.InteropServices;
using System.Runtime.Serialization.Json;
using System.Text;
using System.Threading;


namespace AutoBenchmark {
    public static class Util {
        public static void log(string msg) { // https://docs.microsoft.com/en-us/dotnet/csharp/language-reference/tokens/interpolated
            Console.WriteLine($"{friendlyDateTime()} {msg}");
        }

        #region String
        public static string toString(this MemoryStream ms) {
            ms.Position = 0;
            using (StreamReader sr = new StreamReader(ms)) { return sr.ReadToEnd(); }
        }

        public static string removeSuffix(this string str, string suffix) {
            return (str.EndsWith(suffix) ? str.Substring(0, str.Length - suffix.Length) : str);
        }

        public static string quote(this string str) { return "\"" + str + "\""; }

        public static string subStr(this string str, int beginIndex, int endIndex) {
            return str.Substring(beginIndex, endIndex - beginIndex);
        }
        public static string subStr(this string str, int beginIndex, char delim) {
            int endIndex = str.IndexOf(delim);
            return str.subStr(beginIndex, (endIndex > beginIndex) ? endIndex : str.Length);
        }


        public static void save(this Attachment attachment, string filePath) {
            using (FileStream fs = File.Create(filePath)) {
                attachment.ContentStream.CopyTo(fs);
            }
        }
        public static string toString(this Attachment attachment) {
            using (MemoryStream ms = new MemoryStream()) {
                attachment.ContentStream.CopyTo(ms);
                return ms.toString();
            }
        }

        public static MemoryStream toStream(this MimePart attachment) {
            MemoryStream ms = new MemoryStream();
            attachment.Content.DecodeTo(ms);
            ms.Position = 0;
            return ms;
        }

        public static string toHtmlTable(this string s) {
            StringBuilder sb = new StringBuilder("<table border=1>");
            bool emptyLine = true;
            foreach (var c in s) {
                if ((c == '\r') || (c == '\n')) {
                    if (!emptyLine) {
                        sb.Append("</td></tr>");
                        emptyLine = true;
                    }
                    continue;
                }
                if (emptyLine) { sb.Append("<tr><td>"); }
                if (c == '\t') {
                    sb.Append("</td><td>");
                    continue;
                }
                emptyLine = false;
                sb.Append(c);
            }
            if (!emptyLine) { sb.Append("</td></tr>"); }
            sb.Append("</table>");
            return sb.ToString();
        }

        public static string toSafeCsvStr(this string s) {
            if (s.Length <= 0) { return "?"; }
            return s.Replace(",", "").Replace("\"", "").Replace("\t", "").Replace("\n", "").Replace("\r", "");
        }
        #endregion String

        #region Serialization
        public static Encoding getEncoding(int codePage = 936) {
            Encoding.RegisterProvider(CodePagesEncodingProvider.Instance);
            return Encoding.GetEncoding(codePage);
        }

        public static string readText(string path) {
            if (!File.Exists(path)) { return ""; }
            return File.ReadAllText(path, CommonCfg.DefaultEncoding);
        }
        public static string[] readLines(string path) {
            if (!File.Exists(path)) { return new string[0]; }
            return File.ReadAllLines(path, CommonCfg.DefaultEncoding);
        }
        public static void appendText(string path, string contents) {
            File.AppendAllText(path, contents, CommonCfg.DefaultEncoding);
        }
        public static void appendLine(string path, string contents) {
            File.AppendAllText(path, contents + Environment.NewLine, CommonCfg.DefaultEncoding);
        }
        public static void appendLines(string path, IEnumerable<string> contents) {
            File.AppendAllLines(path, contents, CommonCfg.DefaultEncoding);
        }
        public static void writeText(string path, string contents) {
            File.WriteAllText(path, contents, CommonCfg.DefaultEncoding);
        }
        public static void writeLine(string path, string contents) {
            File.WriteAllText(path, contents + Environment.NewLine, CommonCfg.DefaultEncoding);
        }
        public static void writeLines(string path, IEnumerable<string> contents) {
            File.WriteAllLines(path, contents, CommonCfg.DefaultEncoding);
        }

        public static async void appendAll(this StringBuilder sb, StreamReader sr, int msTimeout, int bufSize = 4096) { // non-blocking version.
            for (char[] buf = new char[bufSize]; ;) {
                var n = sr.ReadAsync(buf, 0, buf.Length);
                if (!n.Wait(msTimeout)) { break; }
                sb.Append(new Span<char>(buf, 0, await n));
            }
            //ThreadPool.QueueUserWorkItem(o => {
            //    try {
            //        for (int c = 0; (c = sr.Read()) != -1; sb.Append((char)c)) { }
            //    } catch (Exception) { }
            //});
        }
        public static void appendAll(this StringBuilder sb, StreamReader sr) { // blocking version.
            try {
                for (int c = 0; (c = sr.Read()) != -1; sb.Append((char)c)) { }
            } catch (Exception) { }
        }

        public static void saveCsv(string filename, string[,] table, string delim) {
            StringBuilder sb = new StringBuilder();
            for (int i = 0; i < table.GetLength(0); ++i) {
                sb.Append($"{table[i, 0]}");
                for (int j = 1; j < table.GetLength(1); ++j) {
                    sb.Append($"{delim}{table[i, j]}");
                }
                sb.AppendLine();
            }
            writeText(filename, sb.ToString());
        }
        public static string[][] loadCsv(string filename, string delim) {
            if (!File.Exists(filename)) { return null; }
            string[] lines = readLines(filename);
            string[][] table = new string[lines.Length][];
            for (int i = 0; i < lines.Length; ++i) { table[i] = lines[i].Split(delim); }
            return table;
        }

        public static class Json {
            static readonly DataContractJsonSerializerSettings JsonSettings = new DataContractJsonSerializerSettings {
                UseSimpleDictionaryFormat = true
            };

            public static void save<T>(string path, T obj) {
                using (FileStream fs = File.Open(path,
                    FileMode.Create, FileAccess.Write, FileShare.Read)) {
                    serialize<T>(fs, obj);
                }
            }

            public static T load<T>(string path) where T : new() {
                if (!File.Exists(path)) { return new T(); }
                using (FileStream fs = File.Open(path,
                    FileMode.Open, FileAccess.Read, FileShare.Read)) {
                    return deserialize<T>(fs);
                }
            }

            public static void serialize<T>(Stream stream, T obj) {
                DataContractJsonSerializer js = new DataContractJsonSerializer(typeof(T), JsonSettings);
                js.WriteObject(stream, obj);
            }

            public static T deserialize<T>(Stream stream) {
                DataContractJsonSerializer js = new DataContractJsonSerializer(typeof(T), JsonSettings);
                return (T)js.ReadObject(stream);
            }

            public static string toJsonString<T>(T obj) {
                using (MemoryStream ms = new MemoryStream()) {
                    serialize(ms, obj);
                    return ms.toString();
                }
            }

            public static T fromJsonString<T>(string json) {
                using (MemoryStream ms = new MemoryStream(Encoding.UTF8.GetBytes(json))) {
                    return deserialize<T>(ms);
                }
            }
        }
        #endregion Serialization

        #region FileSystem
        public delegate void ManipulateFile(FileInfo file);
        public delegate bool ManipulateDir(DirectoryInfo dir); // return false if there is no need to look into it.


        public static void traverseDirectory(string dirPath, ManipulateFile fileOp, ManipulateDir dirOp) {
            DirectoryInfo root = new DirectoryInfo(dirPath);
            traverseDirectory(root, fileOp, dirOp);
        }

        public static void traverseDirectory(DirectoryInfo root, ManipulateFile fileOp, ManipulateDir dirOp) {
            foreach (var dir in root.GetDirectories()) {
                if (dirOp(dir)) { traverseDirectory(dir.FullName, fileOp, dirOp); }
            }
            foreach (var file in root.GetFiles()) { fileOp(file); }
        }

        public static List<string> listFiles(string dir, bool recursive = false) {
            List<string> fileList = new List<string>();
            traverseDirectory(dir, (f) => {
                fileList.Add(f.Name);
            }, (d) => { return recursive; });
            return fileList;
        }

        public static void moveFile(string sourceFileName, string destFileName) {
            if (!File.Exists(sourceFileName)) { return; }
            if (File.Exists(destFileName)) { File.Delete(destFileName); }
            File.Move(sourceFileName, destFileName);
        }
        #endregion FileSystem

        #region Process
        public static class OS {
            [Flags]
            public enum ErrorModes : uint {
                SYSTEM_DEFAULT = 0x0,
                SEM_FAILCRITICALERRORS = 0x0001,
                SEM_NOALIGNMENTFAULTEXCEPT = 0x0004,
                SEM_NOGPFAULTERRORBOX = 0x0002,
                SEM_NOOPENFILEERRORBOX = 0x8000,

                NoWerDialog = SEM_NOGPFAULTERRORBOX | SEM_FAILCRITICALERRORS | SEM_NOOPENFILEERRORBOX,
            }

            [DllImport("kernel32.dll")]
            public static extern ErrorModes GetErrorMode();
            [DllImport("kernel32.dll")]
            public static extern ErrorModes SetErrorMode(ErrorModes mode);

            public static void disableWindowsErrorReportingDialog() {
                SetErrorMode(GetErrorMode() | ErrorModes.NoWerDialog);
            }
        }

        // [Blocking][NoWindow][InterceptOutput]
        public static string runRead(string fileName, string arguments = "") {
            ProcessStartInfo psi = new ProcessStartInfo(fileName, arguments);
            psi.WindowStyle = ProcessWindowStyle.Hidden;
            psi.UseShellExecute = false;
            psi.RedirectStandardOutput = true;
            using (Process p = Process.Start(psi)) {
                p.WaitForExit();
                return p.StandardOutput.ReadToEnd();
            }
        }
        // [NonBlocking][NoWindow]
        public static Process runAsync(string fileName, string arguments = "") {
            ProcessStartInfo psi = new ProcessStartInfo(fileName, arguments);
            psi.WindowStyle = ProcessWindowStyle.Hidden;
            psi.UseShellExecute = false;
            return Process.Start(psi);
        }
        // [Blocking][NoWindow][GetExitCode]
        public static int run(string fileName, string arguments = "") {
            using (Process p = runAsync(fileName, arguments)) {
                p.WaitForExit();
                return p.ExitCode;
            }
        }
        // [Blocking][ShowWindow]
        public static Process runUI(string fileName, string arguments = "") {
            Process p = Process.Start(fileName, arguments);
            p.WaitForExit();
            return p;
        }
        // [NonBlocking][ShowWindow]
        public static Process runUIAsync(string fileName, string arguments = "") {
            return Process.Start(fileName, arguments);
        }

        public static class Signal {
            public enum Type : uint {
                Interrupt = CtrlTypes.CTRL_C_EVENT,
                Break = CtrlTypes.CTRL_BREAK_EVENT,
            }

            public static bool send(Process p, int msTimeout = 0) {
                SetConsoleCtrlHandler(null, true);
                run("SendSignal", $"{p.Id}");
                bool r = p.WaitForExit(msTimeout);
                SetConsoleCtrlHandler(null, false);
                return r;
            }
            public static bool send(Process p, int msTimeout, Type signal) {
                SetConsoleCtrlHandler(null, true);
                run("SendSignal", $"{p.Id} {(uint)signal}");
                bool r = p.WaitForExit(msTimeout);
                SetConsoleCtrlHandler(null, false);
                return r;
            }


            //[Obsolete("NotWorking", true)]
            //public static void send(Process proc, int msTimeout = 0, Type signal = Type.Interrupt) {
            //    FreeConsole();
            //    if (AttachConsole((uint)proc.Id)) { // this does not require the console window to be visible.
            //        SetConsoleCtrlHandler(null, true); // disable Ctrl-C handling for ourselves.

            //        GenerateConsoleCtrlEvent((uint)signal, 0);
            //        FreeConsole(); // avoid terminating ourselves if `proc` is killed by others.
            //        proc.WaitForExit(msTimeout); // avoid terminating ourselves.

            //        SetConsoleCtrlHandler(null, false); // re-enable Ctrl-C handling or any subsequently started programs will inherit the disabled state.
            //    }
            //}


            private enum CtrlTypes : uint {
                CTRL_C_EVENT = 0,
                CTRL_BREAK_EVENT,
                CTRL_CLOSE_EVENT,
                CTRL_LOGOFF_EVENT = 5,
                CTRL_SHUTDOWN_EVENT,
            }

            private delegate bool ConsoleCtrlDelegate(uint CtrlType);

            //[DllImport("kernel32.dll", SetLastError = true)]
            //private static extern bool AttachConsole(uint dwProcessId);

            //[DllImport("kernel32.dll", SetLastError = true, ExactSpelling = true)]
            //private static extern bool FreeConsole();

            [DllImport("kernel32.dll")]
            private static extern bool SetConsoleCtrlHandler(ConsoleCtrlDelegate HandlerRoutine, bool Add);

            //[DllImport("kernel32.dll")]
            //[return: MarshalAs(UnmanagedType.Bool)]
            //private static extern bool GenerateConsoleCtrlEvent(uint dwCtrlEvent, uint dwProcessGroupId);
        }
        #endregion Process

        #region Thread
        public delegate bool IsTaskTaken();
        public delegate void UserTask(IsTaskTaken isTaskTaken);

        /// <summary> workers scramble for tasks. </summary>
        /// <remarks>call `isTaskTaken` in `userTask` to avoid repeating.</remarks>
        /// <example>
        /// HashSet<int> s = new HashSet<int> { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
        /// Util.TaskTaker.run(4, (isTaskTaken) => {
        ///     foreach (var i in s) {
        ///         if (isTaskTaken()) { continue; }
        ///         Console.WriteLine(i);
        ///         Thread.Sleep(2000);
        ///     }
        /// });
        /// </example>
        public static void fightForTasks(int workerNum, UserTask userTask) {
            Thread[] workers = new Thread[workerNum];
            int gt = 0; // global task index.
            for (int w = 0; w < workerNum; ++w) {
                workers[w] = new Thread(() => {
                    int t = 0; // local task index.
                    userTask(() => {
                        if (t < gt) { ++t; return true; }
                        int ogt = gt; // old global task index.
                        return ogt != Interlocked.CompareExchange(ref gt, ogt + 1, t++);
                    });
                });
                workers[w].Start();
            }
            for (int w = 0; w < workerNum; ++w) { workers[w].Join(); }
        }
        #endregion Thread

        #region DateTime
        // https://docs.microsoft.com/en-us/dotnet/standard/base-types/custom-date-and-time-format-strings
        public static string compactDateTime(DateTime dt) { return dt.ToString("yyyyMMddHHmmss"); }
        public static string compactDateTime() { return compactDateTime(DateTime.Now); }
        public static string friendlyDateTime(DateTime dt) { return dt.ToString("yyyy-MM-dd HH:mm:ss.fff"); }
        public static string friendlyDateTime() { return friendlyDateTime(DateTime.Now); }
        #endregion DateTime

        #region Container
        public static void pop<T>(this List<T> l) { l.RemoveAt(l.Count - 1); }

        public static void tryInc<TKey>(IDictionary<TKey, int> dictionary, TKey key) {
            dictionary.TryAdd(key, 0);
            ++dictionary[key];
        }
        public static void tryDec<TKey>(IDictionary<TKey, int> dictionary, TKey key) {
            if (--dictionary[key] <= 0) { dictionary.Remove(key); }
        }
        public static T tryAdd<TKey, T>(IDictionary<TKey, T> dictionary, TKey key) where T : new() {
            dictionary.TryAdd(key, new T());
            return dictionary[key];
        }

        public static bool tryUpdateMin<TKey, TValue>(IDictionary<TKey, TValue> dictionary, TKey key, TValue value) where TValue : IComparable<TValue> {
            if (!dictionary.ContainsKey(key)) { dictionary.Add(key, value); return true; }
            if (value.CompareTo(dictionary[key]) < 0) { dictionary[key] = value; return true; }
            return false;
        }

        public static bool lexLess<T>(T[] l, T[] r) where T : IComparable<T> {
            int len = Math.Min(l.Length, r.Length);
            for (int i = 0; i < len; ++i) {
                int diff = l[i].CompareTo(r[i]);
                if (diff != 0) { return diff < 0; }
            }
            return l.Length < r.Length;
        }

        public static int[] orderedKeys<T>(Dictionary<int, T> mapping) {
            int[] keys = mapping.Keys.ToArray();
            Array.Sort(keys);
            return keys;
        }
        public static int[] mapBack(Dictionary<int, int> mapping) {
            int[] keys = orderedKeys(mapping);
            for (int i = 0; i < keys.Length; ++i) { mapping[keys[i]] = i; }
            return keys;
        }
        #endregion Container

        #region Math
        public static int count(params bool[] binaries) { return binaries.Count(t => t); }

        public static bool updateMin(ref int minValue, int newValue) {
            if (newValue < minValue) { minValue = newValue; return true; }
            return false;
        }
        public static bool updateMax(ref int maxValue, int newValue) {
            if (newValue > maxValue) { maxValue = newValue; return true; }
            return false;
        }
        public static bool updateMin(ref double minValue, double newValue) {
            if (newValue < minValue) { minValue = newValue; return true; }
            return false;
        }
        public static bool updateMax(ref double maxValue, double newValue) {
            if (newValue > maxValue) { maxValue = newValue; return true; }
            return false;
        }
        public static bool updateMin(ref string minValue, string newValue) {
            if (newValue.CompareTo(minValue) < 0) { minValue = newValue; return true; }
            return false;
        }
        public static bool updateMax(ref string maxValue, string newValue) {
            if (newValue.CompareTo(maxValue) > 0) { maxValue = newValue; return true; }
            return false;
        }

        public static int lowerBound(int[] arr, int value) {
            int i = Array.BinarySearch(arr, value);
            return (i < 0) ? ~i : i;
        }

        public static double powerSum(double a, double b) {
            return a * a + b * b;
        }

        // "forward" means `(src0 <= dst0) && (src1 <= dst1)`.
        public static class RectilinearForward {
            public static bool between(int value, int lb, int ub) { return (lb <= value) && (value <= ub); }

            public static bool within(int[] pos, int[] corner0, int[] corner1) {
                for (int i = 0; i < pos.Length; ++i) {
                    if (!between(pos[i], corner0[i], corner1[i])) { return false; }
                }
                return true;
            }
            public static bool within(int[] pos, int[] box) {
                return within(pos, new int[2] { box[0], box[1] }, new int[2] { box[2], box[3] });
            }

            public static bool segmentsInterfering(int src0, int dst0, int src1, int dst1) { return (src0 <= dst1) && (src1 <= dst0); }

            /// <summary>
            /// check if two rectilinear forward segments are intersected or overlapped.
            /// </summary>
            public static bool segmentsInterfering(int[] src0, int[] dst0, int[] src1, int[] dst1) {
                int dx0 = dst0[0] - src0[0];
                int dx1 = dst1[0] - src1[0];
                if ((dx0 == 0) != (dx1 == 0)) { // orthogonal.
                    if (dx0 == 0) { // segment 0 is vertical and segment 1 is horizontal.
                        return between(src0[0], src1[0], dst1[0]) && between(src1[1], src0[1], dst0[1]);
                    } else { // segment 0 is horizontal and segment 1 is vertical.
                        return between(src1[0], src0[0], dst0[0]) && between(src0[1], src1[1], dst1[1]);
                    }
                } else { // parallel.
                    if (dx0 == 0) { // both are vertical.
                        return (src0[0] == src1[0]) && segmentsInterfering(src0[1], dst0[1], src1[1], dst1[1]);
                    } else { // both are horizontal.
                        return (src0[1] == src1[1]) && segmentsInterfering(src0[0], dst0[0], src1[0], dst1[0]);
                    }
                }
            }
            public static bool segmentsInterfering(int[][] segment0, int[][] segment1) {
                return segmentsInterfering(segment0[0], segment0[1], segment1[0], segment1[1]);
            }

            /// <summary>
            /// check if a rectilinear segments and a box (hollow rectangle) is intersected or overlapped.
            /// </summary>
            public static bool segmentBoxInterfering(int[][] segment, int[] box) {
                int dx0 = segment[0][1] - segment[0][0];
                if (dx0 == 0) { // vertical.
                    return between(segment[0][0], box[0], box[2]) && segmentsInterfering(segment[0][1], segment[1][1], box[1], box[3]);
                } else { // horizontal.
                    return between(segment[0][1], box[1], box[3]) && segmentsInterfering(segment[0][0], segment[1][0], box[0], box[2]);
                }
            }

            /// <summary>
            /// check if a rectilinear segments and a solid rectangle is intersected or overlapped.
            /// </summary>
            public static bool segmentRectInterfering(int[][] segment, int[] rect) {
                return segmentBoxInterfering(segment, rect)
                    || (within(segment[0], rect) && within(segment[1], rect));
            }
        }

        public static class Rectilinear {
            public static bool between(int value, int bound0, int bound1) {
                //return ((value - bound0) * (value - bound1)) <= 0; // may overflow.
                return ((value == bound0) || (value == bound1)) || ((value < bound0) != (value < bound1));
                //return (value <= bound0) ? (value >= bound1) : (value <= bound1);
            }

            public static bool within(int[] pos, int[] corner0, int[] corner1) {
                for (int i = 0; i < pos.Length; ++i) {
                    if (!between(pos[i], corner0[i], corner1[i])) { return false; }
                }
                return true;
            }
            public static bool within(int[] pos, int[] box) {
                return within(pos, new int[2] { box[0], box[1] }, new int[2] { box[2], box[3] });
            }

            public static bool segmentsInterfering(int src0, int dst0, int src1, int dst1) {
                int r = count(src0 <= src1, src0 < dst1, dst0 < src1, dst0 <= dst1);
                return (0 < r) && (r < 4);
            }

            /// <summary>
            /// check if two rectilinear segments are intersected or overlapped.
            /// </summary>
            public static bool segmentsInterfering(int[] src0, int[] dst0, int[] src1, int[] dst1) {
                int dx0 = dst0[0] - src0[0];
                int dx1 = dst1[0] - src1[0];
                if ((dx0 == 0) != (dx1 == 0)) { // orthogonal.
                    if (dx0 == 0) { // segment 0 is vertical and segment 1 is horizontal.
                        return between(src0[0], src1[0], dst1[0]) && between(src1[1], src0[1], dst0[1]);
                    } else { // segment 0 is horizontal and segment 1 is vertical.
                        return between(src1[0], src0[0], dst0[0]) && between(src0[1], src1[1], dst1[1]);
                    }
                } else { // parallel.
                    if (dx0 == 0) { // both are vertical.
                        if (src0[0] != src1[0]) { return false; }
                        return segmentsInterfering(src0[1], dst0[1], src1[1], dst1[1]);
                    } else { // both are horizontal.
                        if (src0[1] != src1[1]) { return false; }
                        return segmentsInterfering(src0[0], dst0[0], src1[0], dst1[0]);
                    }
                }
            }
            public static bool segmentsInterfering(int[][] segment0, int[][] segment1) {
                return segmentsInterfering(segment0[0], segment0[1], segment1[0], segment1[1]);
            }

            /// <summary>
            /// check if a rectilinear segments and a box (hollow rectangle) is intersected or overlapped.
            /// </summary>
            public static bool segmentBoxInterfering(int[][] segment, int[] box) {
                int dx0 = segment[0][1] - segment[0][0];
                int[] n00 = new int[2] { box[0], box[1] };
                int[] n01 = new int[2] { box[0], box[3] };
                int[] n10 = new int[2] { box[2], box[1] };
                int[] n11 = new int[2] { box[2], box[3] };
                if (dx0 == 0) { // vertical.
                    return segmentsInterfering(segment, new int[2][] { n00, n10 })
                        || segmentsInterfering(segment, new int[2][] { n01, n11 });
                } else { // horizontal.
                    return segmentsInterfering(segment, new int[2][] { n00, n01 })
                        || segmentsInterfering(segment, new int[2][] { n10, n11 });
                }
            }

            /// <summary>
            /// check if a rectilinear segments and a solid rectangle is intersected or overlapped.
            /// </summary>
            public static bool segmentRectInterfering(int[][] segment, int[] rect) {
                return segmentBoxInterfering(segment, rect)
                    || (within(segment[0], rect) && within(segment[1], rect));
            }
        }
        #endregion Math

        public class OppositeComparer<T> : IComparer<T> {
            public static OppositeComparer<T> Default { get { return oppositeComparer; } }

            public int Compare(T x, T y) { return -Comparer<T>.Default.Compare(x, y); }

            protected static OppositeComparer<T> oppositeComparer = new OppositeComparer<T>();
        }

        public static void swap<T>(ref T l, ref T r) {
            T tmp = l;
            l = r;
            r = tmp;
        }
    }
}
