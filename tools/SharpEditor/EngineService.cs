using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace SharpEditor
{
    public static class EngineService
    {
        private static EngineProcess m_Process;        

        struct ProcessInfo
        {
            public string m_strFileName;
            public string m_strWorkingDirectory;
            public string m_strSystemPath;
            public bool m_fExternalProcess;
        }

        public static bool IsExternalProcess()
        {
            return m_Process == null;
        }

        public static bool ShowEngineConfigDialog()
        {
            using (EngineConfigForm form = new EngineConfigForm())
            {
                if (form.ShowDialog() == DialogResult.Cancel)
                {
                    return false;
                }

                return true;
            }
        }

        private static bool PrepareProcessInfo(out ProcessInfo info)
        {
            info = new ProcessInfo();

            for (; ; )
            {
                info.m_fExternalProcess = Properties.Settings.Default.EngineExternalStart;
                if (info.m_fExternalProcess)
                    return true;

                string exe = Properties.Settings.Default.EnginePath;
#if DEBUG
                exe = System.IO.Path.Combine(exe, "PH_EngineMain_d.exe");
#else
                exe = System.IO.Path.Combine(exe, "PH_EngineMain.exe");
#endif
                
                if (!System.IO.File.Exists(exe))
                {
                    if (!ShowEngineConfigDialog())
                        return false;

                    continue;
                }                

                string workingDir = Properties.Settings.Default.EngineWorkingDirectory;
                if (string.IsNullOrEmpty(workingDir))
                {
                    workingDir = System.IO.Path.GetFullPath(exe);
                }
                else if(!System.IO.Directory.Exists(workingDir))
                {
                    if (!ShowEngineConfigDialog())
                        return false;

                    continue;
                }

                info.m_strFileName = exe;
                info.m_strWorkingDirectory = workingDir;

                info.m_strSystemPath = Properties.Settings.Default.EnginePathVariable;

                return true;
            }
        }

        private static void CheckIfStopped()
        {
            if (m_Process != null)
            {
                throw new InvalidOperationException("Engine process already running");
            }
        }

        public static void Start(System.Windows.Forms.Panel enginePanel)
        {
            CheckIfStopped();

            ProcessInfo info;

            if (!PrepareProcessInfo(out info))
                throw new Exception("User aborted");

            if (!info.m_fExternalProcess)
            {
                LogService.Log("Starting engine");
                m_Process = new EngineProcess(info.m_strFileName, info.m_strWorkingDirectory, info.m_strSystemPath, enginePanel);                
                LogService.Log("Engine started");
            }

            RemoteProcedureService.Start();
        }

        public static void CaptureByWindow(IntPtr engineWindow, System.Windows.Forms.Panel enginePanel)
        {
            CheckIfStopped();

            m_Process = new EngineProcess(engineWindow, enginePanel);
        }
             
        public static void Stop()
        {
            if (m_Process == null)
                return;

            RemoteProcedureService.Call("Quit", null, null);

            RemoteProcedureService.Stop();

            //m_Process.Stop();
        }

        public static void Kill()
        {
            if (m_Process != null && !m_Process.HasExited)
                m_Process.Kill();
        }

        public static bool HasEngineProcessExited
        {
            get { return m_Process == null || m_Process.HasExited; }
        }
    }
}
