using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SharpEditor
{
    public class MapObject
    {
        public MapObject(string name, int id)
        {
            Name = name;
            Id = id;
        }

        public string Name
        {
            get;
            private set;
        }


        public int Id
        {
            get;
            private set;
        }        
    }
}
