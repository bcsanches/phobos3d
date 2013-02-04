using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SharpEditor.Assets
{
    public class CategoryManagerEventArgs: EventArgs
    {
        public Category Category
        {
            get;
            private set;
        }

        public CategoryManagerEventArgs(Category category)
        {
            Category = category;
        }
    }

    public static class CategoryManager
    {
        static private HashSet<Category> m_hshCategories = new HashSet<Category>();
        static private Dictionary<string, Category> m_mapCategories = new Dictionary<string,Category>();        

        public static event EventHandler<CategoryManagerEventArgs> CategoryAdded;

        public static Category RegisterCategory(string name)
        {
            Category category;
            if (m_mapCategories.TryGetValue(name, out category))
                return category;

            category = new Category(name);
            m_hshCategories.Add(category);
            m_mapCategories.Add(name, category);

            if (CategoryAdded != null)
            {
                var args = new CategoryManagerEventArgs(category);
                CategoryAdded(null, args);
            }

            return category;
        }
    }
}
