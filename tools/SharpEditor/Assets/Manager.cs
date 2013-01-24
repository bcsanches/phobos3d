using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SharpEditor.Assets
{
    public class AssetManagerEventArgs : EventArgs
    {
        public Asset Asset
        {
            get;
            private set;
        }

        public AssetManagerEventArgs(Asset asset)
        {
            Asset = asset;
        }
    }

    public static class Manager
    {
        private static Dictionary<string, Asset> m_mapAssets = new Dictionary<string,Asset>();

        public static event EventHandler<AssetManagerEventArgs> AssetAdded;

        public static void AddAsset(Asset asset)
        {
            if (m_mapAssets.ContainsKey(asset.Name))
            {
                throw new InvalidOperationException("Asset " + asset.Name + " already exists");
            }

            m_mapAssets.Add(asset.Name, asset);

            if (AssetAdded != null)
            {
                var args = new AssetManagerEventArgs(asset);
                AssetAdded(null, args);
            }
        }
    }
}
