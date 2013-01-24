using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SharpEditor.Assets
{
    public class Category
    {
        private string m_strName;

        public Category(string name)
        {
            m_strName = name;
        }

        public string Name
        {
            get { return m_strName; }
        }
    }
}
