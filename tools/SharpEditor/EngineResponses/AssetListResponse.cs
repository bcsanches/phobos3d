using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SharpEditor.EngineResponses
{
    public class AssetListResponse : IEngineResponse
    {
        public void Process(dynamic json)
        {
            var array = json.assets;
            foreach (dynamic obj in array)
            {
                var asset = new Assets.Asset(json);                
            }
        }
    }
}
