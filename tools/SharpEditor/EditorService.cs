using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Reflection;
using System.Text;
using System.Threading.Tasks;

namespace SharpEditor
{
    public static class EditorService
    {
        private static Dictionary<string, Type> m_mapRequests = new Dictionary<string,Type>();
        private static System.Windows.Forms.Control m_invokeControl = new System.Windows.Forms.Control();

        private static void MainThreadProcessMessage(string message)
        {
            dynamic json = SimpleJson.DeserializeObject(message);            

            Type tp = m_mapRequests[json.command];

            var response = (EngineResponses.IEngineResponse)Activator.CreateInstance(tp);

            response.Process(json);
        }

        //Thread safe
        public static void ProcessMessage(string message)
        {
            if(m_invokeControl.InvokeRequired)
                m_invokeControl.Invoke(new System.Windows.Forms.MethodInvoker(delegate {MainThreadProcessMessage(message);}));
            else
                MainThreadProcessMessage(message);                            
        }

        private static bool IsMappingOf<T>(Type type)
        {
            return !type.IsGenericType && typeof(T).IsAssignableFrom(type) && !type.IsAbstract && !type.IsInterface;
        }

        public static void Start()
        {
            m_invokeControl.CreateControl();

            foreach (Type t in Assembly.GetExecutingAssembly().GetTypes().Where(x => IsMappingOf<EngineResponses.IEngineResponse>(x)))
            {
                m_mapRequests.Add(t.Name, t);
            }            
        }

        public static void Stop()
        {
            m_invokeControl.Dispose();
            m_invokeControl = null;
        }
    }
}
