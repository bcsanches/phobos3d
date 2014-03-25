using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Reflection;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace SharpEditor
{
    public static class EditorService
    {
        private static Dictionary<string, Type> m_mapRequests = new Dictionary<string,Type>();
        private static Panel m_pnlEnginePanel;
       
        private static bool IsMappingOf<T>(Type type)
        {
            return !type.IsGenericType && typeof(T).IsAssignableFrom(type) && !type.IsAbstract && !type.IsInterface;
        }

        public static void Start(Panel enginePanel)
        {
            m_pnlEnginePanel = enginePanel;

            foreach (Type t in Assembly.GetExecutingAssembly().GetTypes().Where(x => IsMappingOf<EngineResponses.IEngineResponse>(x)))
            {
                m_mapRequests.Add(t.Name, t);
            }

            RemoteProcedureService.Ready += RemoteProcedureService_Ready;
        }

        private static void CallInitProcedures()
        {
            //            
            RemoteProcedureService.Call("EnableEditMode", null, null);
            RemoteProcedureService.Call("AssetList", null, new EngineResponses.AssetListResponse().Process);

            if (EngineService.IsExternalProcess())
                RemoteProcedureService.Call("AttachToWindow", new { window = m_pnlEnginePanel.Handle.ToString() }, new AttachToWindowResponse().Process);
        }

        static void RemoteProcedureService_Ready(object sender, EventArgs e)
        {
            if(m_pnlEnginePanel.InvokeRequired)
            {
                m_pnlEnginePanel.Invoke(new System.Windows.Forms.MethodInvoker(delegate { CallInitProcedures(); }));
            }
            else
            {
                CallInitProcedures();
            }            
        }

        public static void Stop()
        {
            //empty
        }

        public static void CreateNewWorld()
        {
            RemoteProcedureService.Call("NewWorld", null, null);
        }

        public static void CreateMapObject(Assets.Asset asset)
        {
            RemoteProcedureService.Call("CreateMapObject", asset, new EngineResponses.CreateMapObjectResponse().Process);
        }

        class AttachToWindowResponse : EngineResponses.IEngineResponse
        {            
            [DllImport("user32.dll", SetLastError = true)]
            private static extern IntPtr GetWindow(IntPtr hwnd, int cmd);

            public void Process(long id, dynamic result, dynamic error)
            {
                if (error != null)
                {
                    EngineResponses.EngineResponse.ShowErrorDialog("Cannot attach engine window:", error);
                    return;
                }

                //5 is GW_CHILD
                IntPtr engineWnd = GetWindow(m_pnlEnginePanel.Handle, 5);
                if (engineWnd == null)
                {
                    MessageBox.Show("Window attached, but cannot find it as child of engine panel", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);

                    return;
                }

                EngineService.CaptureByWindow(engineWnd, m_pnlEnginePanel);
            }
        }
    }
}
