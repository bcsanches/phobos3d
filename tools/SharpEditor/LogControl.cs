using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace SharpEditor
{
    public partial class LogControl : UserControl
    {
        public LogControl()
        {
            InitializeComponent();
        }

        private void SafeLog(String msg)
        {
            tbLog.AppendText(msg + Environment.NewLine);                        
        }

        public void Log(String msg)
        {
            if (tbLog.InvokeRequired)
            {
                this.Invoke(new MethodInvoker(delegate { SafeLog(msg); }));
            }
            else
            {
                this.SafeLog(msg);
            }            
        }

        private void tbClear_Click(object sender, EventArgs e)
        {
            tbLog.Text = string.Empty;
        }
    }
}
