using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SharpEditor.EngineResponses
{
    public class AssetListResponse : IEngineResponse
    {
        public void Process(long id, dynamic result, dynamic error)
        {
            if (error != null)
                return;

            foreach (dynamic obj in result)
            {
                var asset = new Assets.Asset(obj);
                Assets.Manager.AddAsset(asset);
            }
        }
    }
}
