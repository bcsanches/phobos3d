using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SharpEditor
{
    class EngineEventArgs: EventArgs
    {        
        public EngineEventArgs(string message)
        {
            Message = message;
        }

        public string Message
        {
            private set;
            get;
        }
    }

    class EngineClient
    {
        private WebSocket4Net.WebSocket m_Socket = new WebSocket4Net.WebSocket("ws://localhost:2325");
        private bool m_fConnected = false;

        public event EventHandler Connected;
        public event EventHandler<EngineEventArgs> MessageReceived;

        public void Start()
        {
            m_Socket.Opened += socket_Opened;
            m_Socket.Closed += socket_Closed;            
            m_Socket.MessageReceived += mSocket_MessageReceived;
            m_Socket.Error += socket_Error;

            m_Socket.Open();
        }        

        public void Stop()
        {
            if (m_fConnected)
            {
                m_Socket.Close();
                m_fConnected = false;
            }
        }

        public void Send(string cmd)
        {
            m_Socket.Send(cmd);
        }

        void socket_Error(object sender, SuperSocket.ClientEngine.ErrorEventArgs e)
        {
            //System.Windows.Forms.MessageBox.Show("socket_Error");
            LogService.Log(e.Exception.Message);
            
            m_fConnected = false;

            if (m_Socket.State != WebSocket4Net.WebSocketState.Open)
            {
                //Keep trying
                m_Socket.Open();
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

            if (MessageReceived != null)
            {
                EngineEventArgs args = new EngineEventArgs(e.Message);
                MessageReceived(sender, args);
            }
        }
        
        void socket_Closed(object sender, EventArgs e)
        {
            m_fConnected = false;

            if(LogService.Running)
                LogService.Log("Connection closed.");
        }

        void socket_Opened(object sender, EventArgs e)
        {
            LogService.Log("Connected to engine.");
            m_fConnected = true;

            if (Connected != null)
            {
                var args = new EventArgs();
                Connected(this, args);
            }
        }
    }
}
