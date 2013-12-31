using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SharpEditor
{
    public static class RemoteProcedureService
    {
        private static System.Windows.Forms.Control mInvokeControl = new System.Windows.Forms.Control();

        private static int mId = 0;
        private static EngineClient mClient;

        public static event EventHandler Ready;

        public delegate void ResponseHandler(long id, dynamic result, dynamic error);

        private static Dictionary<long, ResponseHandler> mHandlers = new Dictionary<long, ResponseHandler>();

        public static long Call(string name, object param, ResponseHandler handler)
        {
            //{\"method\":\"AssetList\"}
            //mNetworkService.Send("{\"method\":\"AssetList\"}");

            if (handler != null)
            {
                long id = ++mId;

                //{"jsonrpc":"2.0", "method":"name", "id":id}
                mClient.Send("{\"jsonrpc\": \"2.0\", \"method\":\"" + name + "\", \"id\":" + id + "}");

                mHandlers.Add(id, handler);

                return id;
            }
            else
            {
                mClient.Send("{\"jsonrpc\": \"2.0\", \"method\":\"" + name + "\"}");

                return 0;
            }           
        }

        public static void Start()
        {
            if (mClient != null)
            {
                throw new InvalidOperationException("Network already service running");
            }

            mInvokeControl.CreateControl();

            mClient = new EngineClient();

            mClient.Connected += mNetworkService_Connected;
            mClient.MessageReceived += mNetworkService_MessageReceived;

            mClient.Start();
        }

        public static void Stop()
        {
            mInvokeControl.Dispose();
            mInvokeControl = null;

            if (mClient != null)
            {
                mClient.Stop();
                mClient = null;
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

            if (mInvokeControl.InvokeRequired)
                mInvokeControl.Invoke(new System.Windows.Forms.MethodInvoker(delegate { DispatchMessage(handler, id, json); }));
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
