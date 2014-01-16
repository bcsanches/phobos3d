using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace SharpEditor
{
    public partial class MainForm : Form
    {
        public MainForm()
        {
            InitializeComponent();

            StatusBarService.SetLabel(mStatusLabel);
        }

        protected override void OnLoad(EventArgs e)
        {
            LogService.Start(mLogsControl.mEditorLogControl);            
            EditorService.Start();

            base.OnLoad(e);

            try
            {
                EngineService.Start(mEnginePanel);
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message);
            }
        }

        protected override void OnClosed(EventArgs e)
        {
            base.OnClosed(e);

            EditorService.Stop();
            EngineService.Stop();            
            LogService.Stop();
        }

        private void aboutToolStripMenuItem_Click(object sender, EventArgs e)
        {
            using (AboutBox box = new AboutBox())
            {
                box.ShowDialog();
            }
        }

        private void optionsToolStripMenuItem_Click(object sender, EventArgs e)
        {
            EngineService.ShowEngineConfigDialog();
        }

        private void newToolStripMenuItem_Click(object sender, EventArgs e)
        {
            EditorService.CreateNewWorld();
        }
    }
}

