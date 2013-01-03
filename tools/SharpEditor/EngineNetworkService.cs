using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SharpEditor
{
    class EngineNetworkService
    {
        private static WebSocket4Net.WebSocket mSocket = new WebSocket4Net.WebSocket("ws://localhost:2325");
        private static bool mConnected = false;

        public static void Start()
        {
            mSocket.Opened += socket_Opened;
            mSocket.Closed += socket_Closed;
            mSocket.DataReceived += socket_DataReceived;
            mSocket.Error += socket_Error;

            mSocket.Open();
        }

        static void socket_Error(object sender, SuperSocket.ClientEngine.ErrorEventArgs e)
        {
            //System.Windows.Forms.MessageBox.Show("socket_Error");
            StatusBarService.Status = e.Exception.Message;
            mConnected = false;

            if (mSocket.State != WebSocket4Net.WebSocketState.Open)
            {
                //Keep trying
                mSocket.Open();
            }
            else
            {
                StatusBarService.Status = "Network service is lost";
                System.Windows.Forms.MessageBox.Show("Network service is lost");
            }
        }

        static void socket_DataReceived(object sender, WebSocket4Net.DataReceivedEventArgs e)
        {
            System.Windows.Forms.MessageBox.Show("socket_DataReceived");            
        }

        static void socket_Closed(object sender, EventArgs e)
        {
            mConnected = false;
            StatusBarService.Status = "Connection closed.";
        }

        static void socket_Opened(object sender, EventArgs e)
        {
            StatusBarService.Status = "Connected to engine.";
            mConnected = true;
        }
    }
}
