using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Net;
using System.Net.Mail;
using System.Text;
using SevenZipExtractor;
using MimeKit;
using MailKit;
using MailKit.Search;
using MailKit.Net.Imap;


namespace AutoBenchmark {
    public class EmailFetcher {
        public static bool fetch() {
            try {
                return checkUnseenMails();
            } catch (Exception e) {
                Util.log("[error] fetch fail due to " + e.Message);
                return false;
            }
        }


        static bool checkUnseenMails() {
            Util.log("[info] query unseen emails");
            using (ImapClient client = new ImapClient()) {
                client.Connect(EmailCfg.ImapAddr, EmailCfg.ImapSslPort, true);
                client.Authenticate(EmailCfg.Username, EmailCfg.Password);
                client.Inbox.Open(FolderAccess.ReadWrite);

                bool updated = false;
                IList<UniqueId> uids = client.Inbox.Search(SearchQuery.NotSeen);
                foreach (var uid in uids) {
                    MimeMessage msg = client.Inbox.GetMessage(uid);
                    if (msg.Subject.StartsWith(EmailCfg.SubjectFilter)) {
                        Util.log("[info] handle " + msg.Subject);
                        updated |= handleMessage(msg);
                        client.Inbox.Store(uid, new StoreFlagsRequest(StoreAction.Add, MessageFlags.Seen) { Silent = true });
                    } else {
                        Util.log("[info] ignore " + msg.Subject);
                    }
                }

                client.Disconnect(true);
                return updated;
            }
        }

        static bool handleMessage(MimeMessage msg) {
            DateTime now = DateTime.Now;

            Submission s = new Submission();
            int problemIndexEnd = msg.Subject.IndexOf(EmailCfg.SubjectDelim);
            if (problemIndexEnd < 0) {
                Util.log("[error] unable to extract problem due to no subject delimiter found");
                return false;
            }
            s.problem = msg.Subject.subStr(EmailCfg.ProblemIndexBegin, problemIndexEnd);
            if (!BenchmarkCfg.rank.problems.ContainsKey(s.problem) || !BenchmarkCfg.Checkers.ContainsKey(s.problem)) {
                Util.log("[error] problem not available");
                return false;
            }

            s.author = msg.Subject.Substring(msg.Subject.IndexOf(EmailCfg.SubjectDelim) + 1);
            s.email = msg.Sender?.Address ?? msg.From.Mailboxes.First().Address;
            s.date = Util.friendlyDateTime(now);

            string dirPath = Path.Combine(s.problem, CommonCfg.SolverSubDir, s.author + Util.compactDateTime(now));
            Func<string, string> detectExe = (string fileName) => {
                string filePath = Path.Combine(dirPath, fileName);
                if (!fileName.EndsWith(".exe")) { return filePath; }
                if (s.exePath != null) { Util.log("[warning] multiple executable detected"); }
                return s.exePath = filePath;
            };
            try {
                Directory.CreateDirectory(dirPath);
                foreach (var file in msg.Attachments) {
                    string fileName = file.ContentDisposition?.FileName;
                    if (fileName == null) { continue; }
                    string ext = Path.GetExtension(fileName);
                    if (CommonCfg.ZipFileExts.Contains(ext)) {
                        using (MemoryStream ms = ((MimePart)file).toStream())
                        using (ArchiveFile archiveFile = new ArchiveFile(ms)) {
                            foreach (Entry entry in archiveFile.Entries) {
                                if (entry.Size > EmailCfg.MaxFileByteSize) { Util.log($"[warning] skip file larger than {EmailCfg.MaxFileByteSize}B"); continue; }
                                if (entry.FileName.Contains("__MACOSX") || entry.FileName.Contains(".DS_Store")) { continue; }
                                entry.Extract(detectExe(entry.FileName));
                            }
                        }
                    } else {
                        using (FileStream fs = File.Create(detectExe(fileName))) { ((MimePart)file).Content.DecodeTo(fs); }
                    }
                }
            } catch (Exception e) {
                Util.log("[error] save attachment fail due to " + e.Message);
                return false;
            }

            if (s.exePath != null) {
                Benchmark.push(s);
                //StringBuilder sb = new StringBuilder();
                //sb.AppendLine("Your submission has been received.")
                //    .Append("There are ").Append(Benchmark.queueSize).AppendLine(" submissions in queue.");
                //StdSmtp.send(s.email, "Re: " + msg.Subject, sb.ToString());
                return true;
            }
            Util.log("[error] no executable found");
            StdSmtp.send(s.email, "Re: " + msg.Subject, "No executable found in your submission.");
            return false;
        }
    }


    public class StdSmtp {
        public static void send(string toAddress, string subject, string body) {
            using (SmtpClient client = new SmtpClient(EmailCfg.SmtpAddr)) {
                //client.Port = EmailCfg.SmtpSslPort;
                //client.EnableSsl = true;
                //client.UseDefaultCredentials = false;
                client.Credentials = new NetworkCredential(EmailCfg.Username, EmailCfg.Password);

                using (MailMessage msg = new MailMessage()) {
                    msg.From = new MailAddress(EmailCfg.MyAddress);
                    msg.To.Add(toAddress);
                    //msg.CC.Add(EmailCfg.CcAddress);
                    //msg.SubjectEncoding = Encoding.UTF8;
                    msg.Subject = subject;
                    //msg.BodyEncoding = Encoding.UTF8;
                    msg.IsBodyHtml = true;
                    msg.Body = body;

                    try { client.Send(msg); } catch (Exception e) { Util.log(e.ToString()); }
                }
            }
        }
    }
}
