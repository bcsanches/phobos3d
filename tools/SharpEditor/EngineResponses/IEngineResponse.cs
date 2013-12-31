using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SharpEditor.EngineResponses
{
    interface IEngineResponse
    {
        void Process(long id, dynamic result, dynamic error);
    }
}
