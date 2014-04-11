using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace SharpEditor
{
    public partial class WaitEngineQuitMessageBox : Form
    {
        private int m_iDotsCount = 0;        

        private WaitEngineQuitMessageBox()
        {
            InitializeComponent();

            m_tmrDots.Interval = 100;
            m_tmrDots.Tick += tmrDots_Tick;
        }

        protected override void OnShown(EventArgs e)
        {
            base.OnShown(e);

            m_tmrDots.Start();
        }

        void tmrDots_Tick(object sender, EventArgs e)
        {
            string dots = "";
            for(int i = 0;i < m_iDotsCount; ++i)
                dots += ".";

            ++m_iDotsCount;
            if (m_iDotsCount == 4)
                m_iDotsCount = 0;

            m_lblDots.Text = dots;

            if(EngineService.HasEngineProcessExited)
            {
                m_tmrDots.Stop();

                this.Close();
            }
        }        

        public static new DialogResult ShowDialog()
        {
            WaitEngineQuitMessageBox dialog = new WaitEngineQuitMessageBox();

            return dialog.ShowDialog(null);            
        }
    }
}
