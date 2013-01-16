using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SharpEditor
{
    public static class LogService
    {
        private static StreamWriter mWriter = new StreamWriter("editor.log", false);
        private static LogControl mLogControl;

        public static void Start(LogControl logControl)
        {
            if (logControl == null)
                throw new ArgumentNullException();

            mLogControl = logControl;
        }

        public static void Stop()
        {
            mWriter.Close();
            mWriter.Dispose();
            mWriter = null;
        }

        public static void Log(string msg)
        {            
            mWriter.WriteLine(msg);
            mWriter.Flush();

            StatusBarService.Status = msg;

            mLogControl.Log(msg);
        }
    }
}
