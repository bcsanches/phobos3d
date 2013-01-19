using System;

namespace SharpEditor.Assets
{
    public class Asset
    {
        private string m_strName;

        public Asset(dynamic json)
        {
            m_strName = json.name;
            AssetCategoryManager.AddCategory(json.category);
        }
    }
}
