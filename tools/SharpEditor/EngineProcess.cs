using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Windows.Forms;

namespace SharpEditor
{
    class EngineProcess
    {
        private Process mProcess = new Process();
        private Panel mPanel;
        private Timer mCaptureMainWindowTimer;

        [DllImport("user32.dll", SetLastError = true)]
        private static extern bool MoveWindow(IntPtr hwnd, int x, int y, int cx, int cy, bool repaint);

        public EngineProcess(string exeFile, string workingDirectory, string systemPath, Panel panel)
        {
            mPanel = panel;

            mProcess.StartInfo.Arguments = "\"set dvParentWindow 0x" + panel.Handle.ToString("X") + "\"" + 

#if DEBUG
                " \"loadPlugin PH_Editor_d\"";
#else
                " \"loadPlugin PH_Editor\"";
#endif

            mProcess.StartInfo.UseShellExecute = false;
            mProcess.StartInfo.FileName = exeFile;
            mProcess.StartInfo.WorkingDirectory = workingDirectory;

            if (!string.IsNullOrEmpty(systemPath))
            {
                if (mProcess.StartInfo.EnvironmentVariables.ContainsKey("PATH"))
                {
                    string path = mProcess.StartInfo.EnvironmentVariables["PATH"];
                    if (!path.EndsWith(";"))
                        path += ";";

                    path += systemPath;
                    mProcess.StartInfo.EnvironmentVariables["PATH"] = path;
                }
                else
                {
                    mProcess.StartInfo.EnvironmentVariables.Add("PATH", systemPath);
                }
            }

            mProcess.EnableRaisingEvents = true;
            mProcess.Exited += new EventHandler(mProcess_Exited);

            mCaptureMainWindowTimer = new Timer();
            mCaptureMainWindowTimer.Interval = 100;
            mCaptureMainWindowTimer.Tick += new EventHandler(mCaptureMainWindowTimer_Tick);
            mCaptureMainWindowTimer.Start();

            StatusBarService.Status = "Waiting for main window capture";
        }

        void mCaptureMainWindowTimer_Tick(object sender, EventArgs e)
        {
            mProcess.Refresh();

            if (mProcess.HasExited)
            {
                this.StopCaptureMainWindowTimer();
                return;
            }

            if (mProcess.MainWindowHandle.Equals(new IntPtr(0)))
                return;

            StatusBarService.Status = "Main window capture complete";
            this.StopCaptureMainWindowTimer();

            mPanel.SizeChanged += new EventHandler(mPanel_SizeChanged);

            this.HandleResize();
        }

        void mPanel_SizeChanged(object sender, EventArgs e)
        {
            this.HandleResize();
        }

        void StopCaptureMainWindowTimer()
        {
            if (mCaptureMainWindowTimer == null)
                return;

            mCaptureMainWindowTimer.Stop();
            mCaptureMainWindowTimer.Dispose();
            mCaptureMainWindowTimer = null;
        }

        void mProcess_Exited(object sender, EventArgs e)
        {
            StatusBarService.Status = "Engine exited";

            mPanel.SizeChanged -= mPanel_SizeChanged;
        }

        public void Start()
        {
            mProcess.Start();
        }

        public void Stop()
        {
            if(!mProcess.HasExited)
                mProcess.Kill();
        }

        private void HandleResize()
        {
            var size = mPanel.Size;
            MoveWindow(mProcess.MainWindowHandle, 0, 0, size.Width, size.Height, true);
        }
    }
}
