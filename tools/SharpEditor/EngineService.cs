using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace SharpEditor
{
    public static class EngineService
    {
        private static EngineProcess mProcess;

        struct ProcessInfo
        {
            public string mFileName;
            public string mWorkingDirectory;
            public string mSystemPath;
            public bool mExternalProcess;
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

                info.mExternalProcess = Properties.Settings.Default.EngineExternalStart;

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

                info.mFileName = exe;
                info.mWorkingDirectory = workingDir;

                info.mSystemPath = Properties.Settings.Default.EnginePathVariable;

                return true;
            }
        }

        public static void Start(System.Windows.Forms.Panel enginePanel)
        {
            if (mProcess != null)
            {
                throw new InvalidOperationException("Engine process already running");
            }

            ProcessInfo info;

            if (!PrepareProcessInfo(out info))
                throw new Exception("User aborted");

            if (!info.mExternalProcess)
            {
                StatusBarService.Status = "Starting engine";
                mProcess = new EngineProcess(info.mFileName, info.mWorkingDirectory, info.mSystemPath, enginePanel);
                mProcess.Start();
                StatusBarService.Status = "Engine started";
            }

            EngineNetworkService.Start();
        }

        public static void Stop()
        {
            if(mProcess != null)
                mProcess.Stop();
        }
    }
}
