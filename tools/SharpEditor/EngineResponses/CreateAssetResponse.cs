using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SharpEditor.EngineResponses
{
    class CreateAssetResponse : IEngineResponse
    {
        public void Process(long id, dynamic result, dynamic error)
        {
            if (error != null)
            {
                EngineResponse.ShowErrorDialog(error);                
                return;
            }            
        }
    }
}
