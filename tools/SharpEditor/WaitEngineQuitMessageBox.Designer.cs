namespace SharpEditor
{
    partial class WaitEngineQuitMessageBox
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

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            System.Windows.Forms.Label label1;
            this.m_btnCancel = new System.Windows.Forms.Button();
            this.m_lblDots = new System.Windows.Forms.Label();
            this.m_tmrDots = new System.Windows.Forms.Timer(this.components);
            label1 = new System.Windows.Forms.Label();
            this.SuspendLayout();
            // 
            // label1
            // 
            label1.AutoSize = true;
            label1.Location = new System.Drawing.Point(12, 9);
            label1.Name = "label1";
            label1.Size = new System.Drawing.Size(117, 13);
            label1.TabIndex = 1;
            label1.Text = "Waiting engine to finish";
            // 
            // m_btnCancel
            // 
            this.m_btnCancel.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.m_btnCancel.Cursor = System.Windows.Forms.Cursors.WaitCursor;
            this.m_btnCancel.DialogResult = System.Windows.Forms.DialogResult.Cancel;
            this.m_btnCancel.Location = new System.Drawing.Point(270, 45);
            this.m_btnCancel.Name = "m_btnCancel";
            this.m_btnCancel.Size = new System.Drawing.Size(75, 23);
            this.m_btnCancel.TabIndex = 0;
            this.m_btnCancel.Text = "&Cancel";
            this.m_btnCancel.UseVisualStyleBackColor = true;
            this.m_btnCancel.UseWaitCursor = true;
            // 
            // m_lblDots
            // 
            this.m_lblDots.AutoSize = true;
            this.m_lblDots.Location = new System.Drawing.Point(125, 8);
            this.m_lblDots.Name = "m_lblDots";
            this.m_lblDots.Size = new System.Drawing.Size(16, 13);
            this.m_lblDots.TabIndex = 2;
            this.m_lblDots.Text = "...";
            // 
            // WaitEngineQuitMessageBox
            // 
            this.AcceptButton = this.m_btnCancel;
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(357, 80);
            this.ControlBox = false;
            this.Controls.Add(this.m_lblDots);
            this.Controls.Add(label1);
            this.Controls.Add(this.m_btnCancel);
            this.Cursor = System.Windows.Forms.Cursors.AppStarting;
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "WaitEngineQuitMessageBox";
            this.ShowInTaskbar = false;
            this.Text = "Waiting...";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button m_btnCancel;
        private System.Windows.Forms.Label m_lblDots;
        private System.Windows.Forms.Timer m_tmrDots;
    }
}