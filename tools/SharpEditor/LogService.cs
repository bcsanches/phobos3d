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
        private static StreamWriter m_Writer = new StreamWriter("editor.log", false);
        private static LogControl m_LogControl;

        public static bool Running
        {
            get { return m_Writer != null; }
        }

        public static void Start(LogControl logControl)
        {
            if (logControl == null)
                throw new ArgumentNullException();

            m_LogControl = logControl;
        }

        public static void Stop()
        {
            m_Writer.Close();
            m_Writer.Dispose();
            m_Writer = null;
        }

        public static void Log(string msg)
        {            
            m_Writer.WriteLine(msg);
            m_Writer.Flush();

            StatusBarService.Status = msg;

            m_LogControl.Log(msg);
        }
    }
}
