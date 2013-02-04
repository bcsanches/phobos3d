using System;

namespace SharpEditor.Assets
{
    public class Asset
    {
        private string m_strName;        
        private Category m_Category;

        public Asset(dynamic json)
        {
            m_strName = json.name;
            m_Category = CategoryManager.RegisterCategory(json.category);
        }

        public string Name
        {
            get { return m_strName; }
        }
    }
}
