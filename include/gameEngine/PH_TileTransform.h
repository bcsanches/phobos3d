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

#include <Phobos/Types.h>

#include "PH_GameEngineAPI.h"

namespace Phobos
{
	class PH_GAME_ENGINE_API TileTransform
	{
		public:
			enum Direction_e
			{
				//Those values are the angle in degree
				DIR_NORTH = 180,

				DIR_NORTH_EAST = 135,
				DIR_NORTH_WEST = 225,

				DIR_EAST = 90,

				DIR_SOUTH = 0,

				DIR_SOUTH_WEST = 315,
				DIR_SOUTH_EAST = 45,

				DIR_WEST = 270
			};

			enum Height_e
			{
				HGT_FLOOR,
				HGT_MIDDLE,
				HGT_CEILING,

				HGT_BELOW_CEILING,
				HGT_ABOVE_FLOOR
			};

			enum Position_e
			{
				POS_CENTER,
				POS_NORTH_WEST,
				POS_NORTH_CENTERED,
				POS_NORTH_EAST,
				POS_EAST_CENTERED,
				POS_SOUTH_EAST,
				POS_SOUTH_CENTERED,
				POS_SOUTH_WEST,
				POS_WEST_CENTERED
			};

		public:
			TileTransform(UInt_t row, UInt_t col, Direction_e dir, Height_e height, Position_e position);

			inline int GetRow() const
			{
				return m_iRow;
			}

			inline int GetCol() const
			{
				return m_iCol;
			}

			Direction_e GetDirection() const
			{
				return m_eDirection;
			}

			Height_e GetHeight() const
			{
				return m_eHeight;
			}

			Position_e GetPosition() const
			{
				return m_ePosition;
			}

		private:
			UInt_t m_iRow;
			UInt_t m_iCol;

			Direction_e m_eDirection;
			Height_e	m_eHeight;
			Position_e	m_ePosition;
	};
}

#endif
