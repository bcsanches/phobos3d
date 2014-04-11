using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace SharpEditor
{
    public static class StatusBarService
    {
        private static ToolStripStatusLabel m_tlblLabel;

        public static void SetLabel(ToolStripStatusLabel label)
        {
            m_tlblLabel = label;
        }

        public static string Status
        {
            set
            {
                m_tlblLabel.Text = value;
            }
        }
    }
}
