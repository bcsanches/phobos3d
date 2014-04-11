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

        private Assets.Category GetSelectedCategory()
        {
            return (Assets.Category)cbCategories.SelectedItem;
        }

        protected override void OnLoad(EventArgs e)
        {
            base.OnLoad(e);

            if(this.ParentForm != null)
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
            var asset = e.Asset;

            //ignore if not in the current category
            var selectedCategory = this.GetSelectedCategory();
            if ((selectedCategory != m_CategoryAll) && (e.Asset.Category != selectedCategory))
                return;

            this.AddAsset(e.Asset);
        }

        private void AddAsset(Assets.Asset asset)
        {
            var item = new ListViewItem(asset.Name);
            item.Tag = asset;

            lvAssets.Items.Add(item);
        }

        private void lvAssets_DoubleClick(object sender, EventArgs e)
        {
            var asset = (Assets.Asset) lvAssets.SelectedItems[0].Tag;

            EditorService.CreateMapObject(asset);

            /*
                {
                    "command":"CreateObject",
                    "asset":"barrel.mesh",
                    "category":

             */
        }

        private void cbCategories_SelectedIndexChanged(object sender, EventArgs e)
        {
            lvAssets.Clear();

            var selectedCategory = this.GetSelectedCategory();

            var assetsList = selectedCategory == m_CategoryAll ? Assets.Manager.GetAllAssets() : Assets.Manager.GetAssetsForCategory(selectedCategory);

            var nameFilter = tbSearch.Text.Trim().ToUpper();
            if (!string.IsNullOrEmpty(nameFilter))
            {
                assetsList = assetsList.Where(x => x.Name.ToUpper().Contains(nameFilter)).ToList();
            }

            foreach (var asset in assetsList)
            {
                this.AddAsset(asset);
            }
        }

        private void tbSearch_TextChanged(object sender, EventArgs e)
        {
            this.cbCategories_SelectedIndexChanged(null, null);
        }        
    }
}
