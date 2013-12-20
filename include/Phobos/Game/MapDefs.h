/*
Phobos 3d
July 2013
Copyright (c) 2005-2013 Bruno Sanches  http://code.google.com/p/phobos3d

This software is provided 'as-is', without any express or implied warranty.
In no event will the authors be held liable for any damages arising from the use of this software.
Permission is granted to anyone to use this software for any purpose, 
including commercial applications, and to alter it and redistribute it freely, 
subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.
*/


#ifndef PHOBOS_GAME_MAP_DEFS_H
#define PHOBOS_GAME_MAP_DEFS_H


//
//
//Object types
#define PH_GAME_OBJECT_TYPE_TERRAIN_PAGE "terrain_page"
#define PH_GAME_OBJECT_TYPE_TERRAIN "terrain"
#define PH_GAME_OBJECT_TYPE_STATIC "static"
#define PH_GAME_OBJECT_TYPE_ENTITY "entity"
#define PH_GAME_OBJECT_TYPE_STATIC_LIGHT "static_light"
#define PH_GAME_OBJECT_TYPE_SCENE_MANAGER "scene_manager"

//Default keys
#define PH_GAME_OBJECT_KEY_MESH "mesh"
#define PH_GAME_OBJECT_KEY_TYPE "type"
#define PH_GAME_OBJECT_KEY_POSITION "position"
#define PH_GAME_OBJECT_KEY_SCALE "scale"
#define PH_GAME_OBJECT_KEY_ORIENTATION "orientation"
#define PH_GAME_OBJECT_KEY_CAST_SHADOWS "castShadows"
#define PH_GAME_OBJECT_KEY_LIGHT_TYPE "lightType"
#define PH_GAME_OBJECT_KEY_LIGHT_RANGE "lightRange"
#define PH_GAME_OBJECT_KEY_PARENT_NODE "parentNode"

#define PH_GAME_OBJECT_KEY_WORLD_POSITION "worldPosition"
#define PH_GAME_OBJECT_KEY_WORLD_ORIENTATION "worldOrientation"
#define PH_GAME_OBJECT_KEY_WORLD_SCALE "worldScale"

#define PH_GAME_OBJECT_KEY_RENDER_OBJECT_HANDLER "renderObjectHandler"

#define PH_GAME_OBJECT_LIGHT_TYPE_POINT "point"
#define PH_GAME_OBJECT_LIGHT_TYPE_SPOT "spot"
#define PH_GAME_OBJECT_LIGHT_TYPE_DIRECTIONAL "directional"

#endif