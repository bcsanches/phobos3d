using System;

namespace SharpEditor.Assets
{
    public class Asset
    {
        private string m_strName;
        private string m_strType;
        private Category m_Category;

        public Asset(dynamic json)
        {
            m_strName = json.name;
            m_strType = json.type;
            m_Category = CategoryManager.RegisterCategory(json.category);
        }

        public string Name
        {
            get { return m_strName; }
        }

        public Category Category
        {
            get { return m_Category; }
        }
    }
}
