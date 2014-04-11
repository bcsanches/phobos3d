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
        private Process m_Process = new Process();
        private Panel m_pnlPanel;
        private Timer m_CaptureMainWindowTimer;
        private IntPtr m_ptrEngineWindow;

        [DllImport("user32.dll", SetLastError = true)]
        private static extern bool MoveWindow(IntPtr hwnd, int x, int y, int cx, int cy, bool repaint);

        [DllImport("user32.dll", SetLastError = true)]
        private static extern long GetWindowThreadProcessId(IntPtr hwnd, out uint processId);

        public EngineProcess(IntPtr engineWindow, Panel panel)
        {
            uint processId;

            GetWindowThreadProcessId(engineWindow, out processId);

            m_Process = Process.GetProcessById((int)processId);

            m_pnlPanel = panel;
            m_ptrEngineWindow = engineWindow;

            m_Process.Refresh();

            this.StartResizeHandler();
        }

        public EngineProcess(string exeFile, string workingDirectory, string systemPath, Panel panel)
        {
            m_pnlPanel = panel;

            m_Process.StartInfo.Arguments = "\"set dvParentWindow 0x" + panel.Handle.ToString("X") + "\"" + 

#if DEBUG
                " \"loadPlugin PH_Editor_d\"";
#else
                " \"loadPlugin PH_Editor\"";
#endif            
            m_Process.StartInfo.UseShellExecute = false;
            m_Process.StartInfo.FileName = exeFile;
            m_Process.StartInfo.WorkingDirectory = workingDirectory;

            if (!string.IsNullOrEmpty(systemPath))
            {
                if (m_Process.StartInfo.EnvironmentVariables.ContainsKey("PATH"))
                {
                    string path = m_Process.StartInfo.EnvironmentVariables["PATH"];
                    if (!path.EndsWith(";"))
                        path += ";";

                    path += systemPath;
                    m_Process.StartInfo.EnvironmentVariables["PATH"] = path;
                }
                else
                {
                    m_Process.StartInfo.EnvironmentVariables.Add("PATH", systemPath);
                }
            }

            m_Process.EnableRaisingEvents = true;
            m_Process.Exited += new EventHandler(mProcess_Exited);

            m_CaptureMainWindowTimer = new Timer();
            m_CaptureMainWindowTimer.Interval = 100;
            m_CaptureMainWindowTimer.Tick += new EventHandler(mCaptureMainWindowTimer_Tick);
            m_CaptureMainWindowTimer.Start();

            m_Process.Start();

            LogService.Log("Waiting for main window capture");
        }

        void StartResizeHandler()
        {
            m_pnlPanel.SizeChanged += new EventHandler(mPanel_SizeChanged);

            this.HandleResize();
        }

        void mCaptureMainWindowTimer_Tick(object sender, EventArgs e)
        {
            m_Process.Refresh();

            if (m_Process.HasExited)
            {
                this.StopCaptureMainWindowTimer();
                return;
            }

            if (m_Process.MainWindowHandle.Equals(new IntPtr(0)))
                return;

            m_ptrEngineWindow = m_Process.MainWindowHandle;

            LogService.Log("Main window capture complete");
            this.StopCaptureMainWindowTimer();

            this.StartResizeHandler();
        }

        void mPanel_SizeChanged(object sender, EventArgs e)
        {
            this.HandleResize();
        }

        void StopCaptureMainWindowTimer()
        {
            if (m_CaptureMainWindowTimer == null)
                return;

            m_CaptureMainWindowTimer.Stop();
            m_CaptureMainWindowTimer.Dispose();
            m_CaptureMainWindowTimer = null;
        }

        void mProcess_Exited(object sender, EventArgs e)
        {
            LogService.Log("Engine exited");

            m_pnlPanel.SizeChanged -= mPanel_SizeChanged;
        }

        public void Stop()
        {
            if (!m_Process.HasExited)
            {
                m_Process.CloseMainWindow();
            }               
        }

        public void Kill()
        {            
            m_Process.Kill();
        }

        private void HandleResize()
        {
            var size = m_pnlPanel.Size;
            MoveWindow(m_ptrEngineWindow, 0, 0, size.Width, size.Height, true);
        }

        public bool HasExited
        {
            get { return m_Process == null || m_Process.HasExited; }
        }
    }
}
