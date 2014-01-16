using System;
using System.Runtime.Serialization;

namespace SharpEditor.Assets
{
    [DataContract]
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

        [DataMember(Name = "name")]
        public string Name
        {
            get { return m_strName; }
        }

        [DataMember(Name = "type")]
        public string Type
        {
            get { return m_strType; }
        }

        [IgnoreDataMember]
        public Category Category
        {
            get { return m_Category; }
        }
    }
}
