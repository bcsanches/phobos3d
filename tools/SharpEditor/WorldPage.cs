using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace SharpEditor
{
    public partial class WorldPage : UserControl
    {
        public WorldPage()
        {
            InitializeComponent();
        }

        public void AddMapObject(MapObject obj)
        {
            var node = m_trView.Nodes.Add(obj.Id.ToString(), obj.Name);
            node.Tag = obj;
        }

        public MapObject SelectedItem
        {
            get
            {
                var node = m_trView.SelectedNode;
                if (node == null)
                    return null;

                return (MapObject)node.Tag;
            }            
        }
    }
}
