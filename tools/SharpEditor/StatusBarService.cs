using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace SharpEditor
{
    public static class StatusBarService
    {
        private static ToolStripStatusLabel mLabel;

        public static void SetLabel(ToolStripStatusLabel label)
        {
            mLabel = label;
        }

        public static string Status
        {
            set
            {
                mLabel.Text = value;
            }
        }
    }
}
