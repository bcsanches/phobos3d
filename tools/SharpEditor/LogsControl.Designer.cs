namespace SharpEditor
{
    partial class LogsControl
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
            this.tabControl = new System.Windows.Forms.TabControl();
            this.tabEditorLog = new System.Windows.Forms.TabPage();
            this.tabEngineLog = new System.Windows.Forms.TabPage();
            this.mEditorLogControl = new SharpEditor.LogControl();
            this.tabControl.SuspendLayout();
            this.tabEditorLog.SuspendLayout();
            this.SuspendLayout();
            // 
            // tabControl
            // 
            this.tabControl.Controls.Add(this.tabEditorLog);
            this.tabControl.Controls.Add(this.tabEngineLog);
            this.tabControl.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tabControl.Location = new System.Drawing.Point(0, 0);
            this.tabControl.Name = "tabControl";
            this.tabControl.SelectedIndex = 0;
            this.tabControl.Size = new System.Drawing.Size(483, 150);
            this.tabControl.TabIndex = 0;
            // 
            // tabEditorLog
            // 
            this.tabEditorLog.BackColor = System.Drawing.SystemColors.Control;
            this.tabEditorLog.Controls.Add(this.mEditorLogControl);
            this.tabEditorLog.Location = new System.Drawing.Point(4, 22);
            this.tabEditorLog.Name = "tabEditorLog";
            this.tabEditorLog.Padding = new System.Windows.Forms.Padding(3);
            this.tabEditorLog.Size = new System.Drawing.Size(475, 124);
            this.tabEditorLog.TabIndex = 0;
            this.tabEditorLog.Text = "Editor";
            // 
            // tabEngineLog
            // 
            this.tabEngineLog.BackColor = System.Drawing.SystemColors.Control;
            this.tabEngineLog.Location = new System.Drawing.Point(4, 22);
            this.tabEngineLog.Name = "tabEngineLog";
            this.tabEngineLog.Padding = new System.Windows.Forms.Padding(3);
            this.tabEngineLog.Size = new System.Drawing.Size(475, 124);
            this.tabEngineLog.TabIndex = 1;
            this.tabEngineLog.Text = "Engine";
            // 
            // mEditorLogControl
            // 
            this.mEditorLogControl.Dock = System.Windows.Forms.DockStyle.Fill;
            this.mEditorLogControl.Location = new System.Drawing.Point(3, 3);
            this.mEditorLogControl.Name = "mEditorLogControl";
            this.mEditorLogControl.Size = new System.Drawing.Size(469, 118);
            this.mEditorLogControl.TabIndex = 0;
            // 
            // LogsControl
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.tabControl);
            this.Name = "LogsControl";
            this.Size = new System.Drawing.Size(483, 150);
            this.tabControl.ResumeLayout(false);
            this.tabEditorLog.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.TabControl tabControl;
        private System.Windows.Forms.TabPage tabEditorLog;
        private System.Windows.Forms.TabPage tabEngineLog;
        public LogControl mEditorLogControl;
    }
}
