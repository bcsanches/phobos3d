/*
Phobos 3d
May 2012
Copyright (c) 2005-2012 Bruno Sanches  http://code.google.com/p/phobos3d

This software is provided 'as-is', without any express or implied warranty.
In no event will the authors be held liable for any damages arising from the use of this software.
Permission is granted to anyone to use this software for any purpose, 
including commercial applications, and to alter it and redistribute it freely, 
subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.
*/

#ifndef PH_TILE_TRANSFORM_H
#define PH_TILE_TRANSFORM_H

#include <PH_Types.h>

#include "PH_GameEngineAPI.h"

namespace Phobos
{
	class PH_GAME_ENGINE_API TileTransform_c
	{
		public:
			enum Direction_e
			{
				//Keep those on sequece, so angle can be easily determined
				DIR_NORTH = 2,
				DIR_EAST = 1,
				DIR_SOUTH = 0,
				DIR_WEST = 3
			};

		public:
			TileTransform_c(UInt_t row, UInt_t col, Direction_e dir);

		private:
			UInt_t iRow;
			UInt_t iCol;

			Direction_e eDirection;
	};
}

#endif
