
#include "Phobos/Game/Level/MapObjectComponentFactory.h"

namespace Phobos
{
	namespace Game
	{		
		MapObjectComponentFactory &MapObjectComponentFactory::GetInstance()
		{
			static MapObjectComponentFactory clInstance_gl;

			return clInstance_gl;
		}		
	}
}
