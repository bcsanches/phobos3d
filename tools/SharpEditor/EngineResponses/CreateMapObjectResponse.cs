using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SharpEditor.EngineResponses
{
    class CreateMapObjectResponse : IEngineResponse
    {
        public void Process(long id, dynamic result, dynamic error)
        {
            if (error != null)
            {
                EngineResponse.ShowErrorDialog("Cannot create asset:", error);                
                return;
            }

            //System.Windows.Forms.MessageBox.Show(result.name);
            MapObject obj = new MapObject(result.name, (int)result.id);

            EditorService.AddMapObject(obj);
        }
    }
}
