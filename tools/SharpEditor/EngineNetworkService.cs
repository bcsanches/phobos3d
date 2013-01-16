using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SharpEditor
{
    class EngineNetworkService
    {
        private WebSocket4Net.WebSocket mSocket = new WebSocket4Net.WebSocket("ws://localhost:2325");
        private bool mConnected = false;

        public event EventHandler Connected;

        public void Start()
        {
            mSocket.Opened += socket_Opened;
            mSocket.Closed += socket_Closed;            
            mSocket.MessageReceived += mSocket_MessageReceived;
            mSocket.Error += socket_Error;

            mSocket.Open();
        }        

        public void Stop()
        {
            if (mConnected)
            {
                mSocket.Close();
                mConnected = false;
            }
        }

        public void Send(string cmd)
        {
            mSocket.Send(cmd);
        }

        void socket_Error(object sender, SuperSocket.ClientEngine.ErrorEventArgs e)
        {
            //System.Windows.Forms.MessageBox.Show("socket_Error");
            LogService.Log(e.Exception.Message);
            
            mConnected = false;

            if (mSocket.State != WebSocket4Net.WebSocketState.Open)
            {
                //Keep trying
                mSocket.Open();
            }
            else
            {
                LogService.Log("Network service is lost");
                System.Windows.Forms.MessageBox.Show("Network service is lost");
            }
        }

        void mSocket_MessageReceived(object sender, WebSocket4Net.MessageReceivedEventArgs e)
        {
            LogService.Log("socket_DataReceived:" + e.Message);
        }
        
        void socket_Closed(object sender, EventArgs e)
        {
            mConnected = false;
            LogService.Log("Connection closed.");
        }

        void socket_Opened(object sender, EventArgs e)
        {
            LogService.Log("Connected to engine.");
            mConnected = true;

            if (Connected != null)
            {
                var args = new EventArgs();
                Connected(this, args);
            }
        }
    }
}
