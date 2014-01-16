using System;
using System.Runtime.Serialization;

namespace SharpEditor.Assets
{
    [DataContract]
    public class Category
    {
        private string m_strName;

        public Category(string name)
        {
            m_strName = name;
        }

        [DataMember(Name = "name")]
        public string Name
        {
            get { return m_strName; }
        }
    }
}
