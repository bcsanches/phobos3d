using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SharpEditor
{
    public static class RemoteProcedureService
    {
        private static System.Windows.Forms.Control m_cntInvokeControl = new System.Windows.Forms.Control();

        private static int m_iId = 0;
        private static EngineClient m_Client;

        public static event EventHandler Ready;

        public delegate void ResponseHandler(long id, dynamic result, dynamic error);

        private static Dictionary<long, ResponseHandler> mHandlers = new Dictionary<long, ResponseHandler>();

        public static long Call(string name, object param, ResponseHandler handler)
        {
            var parameters = param != null ? ", \"params\":" + SimpleJson.SerializeObject(param) : "";

            if (handler != null)
            {
                long id = ++m_iId;

                //{"jsonrpc":"2.0", "method":"name", "id":id}
                m_Client.Send("{\"jsonrpc\": \"2.0\", \"method\":\"" + name + "\"" + parameters + ", \"id\":" + id + "}");

                mHandlers.Add(id, handler);

                return id;
            }
            else
            {
                m_Client.Send("{\"jsonrpc\": \"2.0\", \"method\":\"" + name + "\"" + parameters + "}");

                return 0;
            }           
        }

        public static void Start()
        {
            if (m_Client != null)
            {
                throw new InvalidOperationException("Network already service running");
            }

            m_cntInvokeControl.CreateControl();

            m_Client = new EngineClient();

            m_Client.Connected += mNetworkService_Connected;
            m_Client.MessageReceived += mNetworkService_MessageReceived;

            m_Client.Start();
        }

        public static void Stop()
        {
            m_cntInvokeControl.Dispose();
            m_cntInvokeControl = null;

            if (m_Client != null)
            {
                m_Client.Stop();
                m_Client = null;
            }
        }

        private static void DispatchMessage(ResponseHandler handler, long id, dynamic json)
        {
            handler(id, json.result, json.error);
        }

        static void mNetworkService_MessageReceived(object sender, EngineEventArgs args)
        {
            dynamic json = SimpleJson.DeserializeObject(args.Message);

            long id = json.id;

            var handler = mHandlers[id];

            mHandlers.Remove(id);

            if (m_cntInvokeControl.InvokeRequired)
                m_cntInvokeControl.Invoke(new System.Windows.Forms.MethodInvoker(delegate { DispatchMessage(handler, id, json); }));
            else
                DispatchMessage(handler, id, json);                       
        }
        private static void mNetworkService_Connected(object sender, EventArgs e)
        {
            if (Ready != null)
            {
                var args = new EventArgs();
                Ready(sender, args);
            }            
        }   
    }
}
