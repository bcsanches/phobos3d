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
       
        private static bool IsMappingOf<T>(Type type)
        {
            return !type.IsGenericType && typeof(T).IsAssignableFrom(type) && !type.IsAbstract && !type.IsInterface;
        }

        public static void Start()
        {            
            foreach (Type t in Assembly.GetExecutingAssembly().GetTypes().Where(x => IsMappingOf<EngineResponses.IEngineResponse>(x)))
            {
                m_mapRequests.Add(t.Name, t);
            }

            RemoteProcedureService.Ready += RemoteProcedureService_Ready;
        }

        static void RemoteProcedureService_Ready(object sender, EventArgs e)
        {
            //            
            RemoteProcedureService.Call("EnableEditMode", null, null);
            RemoteProcedureService.Call("AssetList", null, new EngineResponses.AssetListResponse().Process);
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
    }
}
