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
    public partial class AssetBrowser : UserControl
    {
        private static readonly Assets.Category m_CategoryAll = new Assets.Category("All");

        public AssetBrowser()
        {
            InitializeComponent();                    
        }

        protected override void OnLoad(EventArgs e)
        {
            base.OnLoad(e);

            this.ParentForm.FormClosed += ParentForm_FormClosed;

            Assets.Manager.AssetAdded += Manager_AssetAdded;
            Assets.CategoryManager.CategoryAdded += CategoryManager_CategoryAdded;

            cbCategories.Items.Add(m_CategoryAll);
            cbCategories.SelectedIndex = 0;
        }

        void CategoryManager_CategoryAdded(object sender, Assets.CategoryManagerEventArgs e)
        {
            cbCategories.Items.Add(e.Category);            
        }

        void ParentForm_FormClosed(object sender, FormClosedEventArgs e)
        {
            Assets.Manager.AssetAdded -= Manager_AssetAdded;
            Assets.CategoryManager.CategoryAdded -= CategoryManager_CategoryAdded;
        }

        void Manager_AssetAdded(object sender, Assets.AssetManagerEventArgs e)
        {
            var item = new ListViewItem(e.Asset.Name);
            item.Tag = e.Asset;

            lvAssets.Items.Add(item);
        }        
    }
}
