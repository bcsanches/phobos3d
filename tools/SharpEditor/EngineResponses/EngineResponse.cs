using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SharpEditor.EngineResponses
{
    public static class EngineResponse
    {
        public static void ShowErrorDialog(string message, dynamic error)
        {
            System.Windows.Forms.MessageBox.Show(message + " " + error.message, "Error", System.Windows.Forms.MessageBoxButtons.OK, System.Windows.Forms.MessageBoxIcon.Error);
        }
    }
}
