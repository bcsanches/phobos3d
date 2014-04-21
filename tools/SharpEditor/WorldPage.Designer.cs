namespace SharpEditor
{
    partial class WorldPage
    {
        /// <summary> 
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary> 
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Component Designer generated code

        /// <summary> 
        /// Required method for Designer support - do not modify 
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            System.Windows.Forms.TabControl tabControl1;
            System.Windows.Forms.TabPage tabPage2;
            this.m_trView = new System.Windows.Forms.TreeView();
            tabControl1 = new System.Windows.Forms.TabControl();
            tabPage2 = new System.Windows.Forms.TabPage();
            tabControl1.SuspendLayout();
            tabPage2.SuspendLayout();
            this.SuspendLayout();
            // 
            // tabControl1
            // 
            tabControl1.Controls.Add(tabPage2);
            tabControl1.Dock = System.Windows.Forms.DockStyle.Fill;
            tabControl1.Location = new System.Drawing.Point(0, 0);
            tabControl1.Name = "tabControl1";
            tabControl1.SelectedIndex = 0;
            tabControl1.Size = new System.Drawing.Size(208, 356);
            tabControl1.TabIndex = 1;
            // 
            // tabPage2
            // 
            tabPage2.BackColor = System.Drawing.SystemColors.Control;
            tabPage2.Controls.Add(this.m_trView);
            tabPage2.Location = new System.Drawing.Point(4, 22);
            tabPage2.Name = "tabPage2";
            tabPage2.Padding = new System.Windows.Forms.Padding(3);
            tabPage2.Size = new System.Drawing.Size(200, 330);
            tabPage2.TabIndex = 1;
            tabPage2.Text = "World";
            // 
            // m_trView
            // 
            this.m_trView.Dock = System.Windows.Forms.DockStyle.Fill;
            this.m_trView.FullRowSelect = true;
            this.m_trView.HideSelection = false;
            this.m_trView.Location = new System.Drawing.Point(3, 3);
            this.m_trView.Name = "m_trView";
            this.m_trView.Size = new System.Drawing.Size(194, 324);
            this.m_trView.TabIndex = 0;
            // 
            // WorldPage
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(tabControl1);
            this.Name = "WorldPage";
            this.Size = new System.Drawing.Size(208, 356);
            tabControl1.ResumeLayout(false);
            tabPage2.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.TreeView m_trView;
    }
}
