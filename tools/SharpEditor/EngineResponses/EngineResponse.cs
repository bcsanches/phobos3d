using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SharpEditor.EngineResponses
{
    public static class EngineResponse
    {
        public static void ShowErrorDialog(dynamic error)
        {
            System.Windows.Forms.MessageBox.Show("Cannot create asset: " + error.message, "Error", System.Windows.Forms.MessageBoxButtons.OK, System.Windows.Forms.MessageBoxIcon.Error);
        }
    }
}
