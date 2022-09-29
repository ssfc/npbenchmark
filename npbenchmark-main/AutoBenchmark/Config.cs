using System;
using System.Collections.Generic;
using System.Text;


namespace AutoBenchmark {
    public class CommonCfg {
        public const string SolverSubDir = "Solver";
        public const string InstanceSubDir = "Instance";
        public const string SolutionSubDir = "Solution";

        public const string RankPath = "rank.json";
        public const string ReadMePath = "ReadMe.md";
        public const string QueuePath = "Queue.md";
        public const string RankPagePath = "index.html";
        public const string StatisticsPath = "Statistics.csv";

        public static string rankMarkdownPath(string problemName) { return $"{problemName}/ReadMe.md"; }
        public static string rankCsvPath(string problemName) { return $"{problemName}/Rank.csv"; }
        public static string bestCsvPath(string problemName) { return $"{problemName}/Best.csv"; }
        public static string logPath(string problemName, string year) { return $"{problemName}/Log{year}.tsv"; }

        public enum QueueState { Pending, Running, Finished }

        public const int MaxResultsCountPerInstance = 10;

        public const int PollIntervalInMillisecond = 15 * 60 * 1000; // 15 minutes.

        public static readonly HashSet<string> ZipFileExts = new HashSet<string> { ".7z", ".gz", ".tar", ".rar", ".zip", ".bz2", ".iso", ".xz" };

        public static readonly Encoding DefaultEncoding = Util.getEncoding(936);
    }

    public class EmailCfg {
        public const int Pop3Port = 110;
        public const int ImapPort = 143;
        public const int SmtpPort = 25;
        public const int SmtpPort2 = 2525;

        public const int Pop3SslPort = 995;
        public const int ImapSslPort = 993;
        public const int SmtpSslPort = 465;
        public const int SmtpSslPort2 = 587;

        public class HustMail {
            public const string Pop3 = "mail.hust.edu.cn";
            public const string Imap = "mail.hust.edu.cn";
            public const string Smtp = "mail.hust.edu.cn";
        }
        public class QqMail {
            public const string Pop3 = "pop3.qq.com";
            public const string Imap = "imap.qq.com";
            public const string Smtp = "smtp.qq.com";
        }
        public class ExqqMail {
            public const string Pop3 = "pop3.exmail.qq.com";
            public const string Imap = "imap.exmail.qq.com";
            public const string Smtp = "smtp.exmail.qq.com";
        }

        public const string Pop3Addr = ExqqMail.Pop3;
        public const string ImapAddr = ExqqMail.Imap;
        public const string SmtpAddr = ExqqMail.Smtp;

        // https://docs.microsoft.com/zh-cn/dotnet/api/system.text.regularexpressions.regex
        public const string SubjectRegex = @"^Challenge20.*-.*$";
        public const string SubjectFilter = "Challenge20";
        public const int ProblemIndexBegin = 13; // "Challenge2020".Length;
        public const char SubjectDelim = '-';

        public const string MyAddress = "szx@duhe.tech";
        //public const string ToAddress = "";
        //public const string CcAddress = "";

        public const string Username = MyAddress;
        public const string Password = PrivateCfg.EmailPassword;

        public const ulong MaxFileByteSize = 8 * 1024 * 1024;
    }

    public enum ProblemName {
        GCP,
        PCP,
        FJSP,
        RWA,
        RPP,
        VRPTW2d,
        OARSMT,
        DFVSP,
        MCDSP,
        PECCP,
        DARP2d,
    }

    public class BenchmarkCfg {
        public const int MsCheckInterval = 1000;
        public const int MsReadInputTime = 8000;
        public const int MsSaveOutputTime = 2000;
        public const long ByteMemoryLimit = 16L * 1024 * 1024 * 1024;

        public static readonly int ParallelBenchmarkNum = Math.Min(16, Environment.ProcessorCount);

        public const string UserName = PrivateCfg.UserName;
        public const string Password = PrivateCfg.Password;
        public const int RandSeedInc = PrivateCfg.RandSeedInc;
        public const int RandSeedMul = PrivateCfg.RandSeedMul;

        public const string LogDelim = "\t";
        public const string LogBasicHeader = "Instance" + LogDelim + "Obj" + LogDelim + "Duration";
        public const string LogCommonHeader = "Solver" + LogDelim + "Seed" + LogDelim + LogBasicHeader;
        public static readonly Dictionary<string, string> LogHeaders = new Dictionary<string, string> {
            { ProblemName.GCP.ToString(), LogCommonHeader + LogDelim + "Conflict" },
            { ProblemName.PCP.ToString(), LogCommonHeader + LogDelim + "Center" + LogDelim + "UncoveredNode" },
            { ProblemName.FJSP.ToString(), LogCommonHeader + LogDelim + "RestJob" },
            { ProblemName.RWA.ToString(), LogCommonHeader + LogDelim + "BrokenPath" + LogDelim + "Conflict" },
            { ProblemName.RPP.ToString(), LogCommonHeader + LogDelim + "RestRect" + LogDelim + "Conflict" },
            { ProblemName.VRPTW2d.ToString(), LogCommonHeader + LogDelim + "Vehicle" + LogDelim + "UncoveredNode" + LogDelim + "Conflict" + LogDelim + "Overload" + LogDelim + "Delay" },
            { ProblemName.OARSMT.ToString(), LogCommonHeader + LogDelim + "Invasion" + LogDelim + "SubTree" },
            { ProblemName.DFVSP.ToString(), LogCommonHeader + LogDelim + "UnsortNode" },
            { ProblemName.MCDSP.ToString(), LogCommonHeader + LogDelim + "UncoveredNode" + LogDelim + "SubGraph" },
            { ProblemName.PECCP.ToString(), LogCommonHeader + LogDelim + "RestCircle" + LogDelim + "Conflict" },
            { ProblemName.DARP2d.ToString(), LogCommonHeader + LogDelim + "Vehicle" + LogDelim + "UncoveredNode" + LogDelim + "Conflict" + LogDelim + "Overload" + LogDelim + "Delay" },
        };

        public const string LeaderboardDelim = ",";
        public const int LeaderboardHeaderRowNum = 3;

        public static readonly Dictionary<string, Check> Checkers = new Dictionary<string, Check> {
            { ProblemName.GCP.ToString(), Checker.coloring },
            { ProblemName.PCP.ToString(), Checker.pCenter },
            { ProblemName.FJSP.ToString(), Checker.jobshop },
            { ProblemName.RWA.ToString(), Checker.rwa },
            { ProblemName.RPP.ToString(), Checker.rectPacking },
            { ProblemName.VRPTW2d.ToString(), Checker.vrptw2d },
            { ProblemName.OARSMT.ToString(), Checker.oarsmt }, // alternative: `Checker.oarsmtEfficientRepresentation`.
            { ProblemName.DFVSP.ToString(), Checker.dfvsp },
            { ProblemName.MCDSP.ToString(), Checker.mcdsp },
            { ProblemName.PECCP.ToString(), Checker.peccp },
            { ProblemName.DARP2d.ToString(), Checker.darp2d },
        };

        public static Rank rank;
        public static Dictionary<string, Leaderboard> leaderboards = new Dictionary<string, Leaderboard>();
    }
}
