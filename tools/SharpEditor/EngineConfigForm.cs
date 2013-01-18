using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace SharpEditor
{
    public partial class EngineConfigForm : Form
    {
        public EngineConfigForm()
        {
            InitializeComponent();

            txtPhobosPath.Text = Properties.Settings.Default.EnginePath;
            txtWorkingDirectory.Text = Properties.Settings.Default.EngineWorkingDirectory;
            txtSystemPath.Text = Properties.Settings.Default.EnginePathVariable;
            cbEnternalRun.Checked = Properties.Settings.Default.EngineExternalStart;
        }

        private void BrowseFolder(TextBox textBox)
        {
            using (FolderBrowserDialog dialog = new FolderBrowserDialog())
            {                
                dialog.SelectedPath = textBox.Text;
                dialog.ShowNewFolderButton = false;                
                
                if (dialog.ShowDialog() == DialogResult.OK)
                {
                    textBox.Text = dialog.SelectedPath;
                }
            }
        }

        private void btnBrowseEngine_Click(object sender, EventArgs e)
        {
            this.BrowseFolder(txtPhobosPath);
        }

        private void btnBrowseWorkingDirectory_Click(object sender, EventArgs e)
        {
            this.BrowseFolder(txtWorkingDirectory);
        }

        private void btnOk_Click(object sender, EventArgs e)
        {   
            Properties.Settings.Default.EnginePath = txtPhobosPath.Text;
            Properties.Settings.Default.EngineWorkingDirectory = txtWorkingDirectory.Text;
            Properties.Settings.Default.EnginePathVariable = txtSystemPath.Text;
            Properties.Settings.Default.EngineExternalStart = cbEnternalRun.Checked;

            Properties.Settings.Default.Save();
        }
    }
}
