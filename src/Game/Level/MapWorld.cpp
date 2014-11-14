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

#include "Phobos/Game/Level/MapWorld.h"

#include <Phobos/Exception.h>
#include <Phobos/Log.h>
#include <Phobos/Path.h>

#include <Phobos/OgreEngine/Render.h>

#include <Phobos/Register/Manager.h>
#include <Phobos/Register/Hive.h>
#include <Phobos/Register/Table.h>

#include "Phobos/Game/Level/LightComponent.h"
#include "Phobos/Game/Level/RigidBodyComponent.h"

#include "Phobos/Game/Level/MapDefs.h"
#include "Phobos/Game/Level/MapObject.h"
#include "Phobos/Game/Level/MapObjectComponentFactory.h"

#include "Phobos/Game/RegisterUtils.h"
#include "Phobos/Game/Level/MapDefs.h"

#include <OgreSceneNode.h>

#include <Terrain/OgreTerrainGroup.h>
#include <Terrain/OgreTerrainPrerequisites.h>

#include <tuple>
#include <utility>

PH_SINGLETON_PROCS(Phobos::Game::MapWorld,);
PH_DEFINE_SINGLETON_VAR(Phobos::Game::MapWorld);

//
//
//Terrain
//
//

namespace
{		
	class Terrain
	{		
		public:
			Terrain(Phobos::StringRef_t levelPath, const Phobos::Register::Table &data, Phobos::Game::LightComponent *terrainLight);
			Terrain(Terrain &&);
			~Terrain();

		private:
			Terrain(const Terrain &);			
			Terrain &operator=(const Terrain &);

		private:
			std::unique_ptr<Ogre::TerrainGlobalOptions> m_upGlobalOptions;
			Ogre::TerrainGroup *m_pclTerrainGroup;
	};	
}

Terrain::Terrain(Terrain &&rhs):
	m_upGlobalOptions(std::move(rhs.m_upGlobalOptions)),
	m_pclTerrainGroup(rhs.m_pclTerrainGroup)
{
	rhs.m_pclTerrainGroup = nullptr;
}

Terrain::Terrain(Phobos::StringRef_t levelPath, const Phobos::Register::Table &data, Phobos::Game::LightComponent *terrainLight):
	m_upGlobalOptions(new Ogre::TerrainGlobalOptions())
{	
	m_upGlobalOptions->setMaxPixelError(data.GetFloat("tuning::maxpixelerror"));
	m_upGlobalOptions->setLightMapSize(data.GetInt("lightmap::texturesize"));
	m_upGlobalOptions->setCompositeMapSize(data.GetInt("tuning::compositemaptexturesize"));
	m_upGlobalOptions->setCompositeMapDistance(data.GetFloat("tuning::compositemapdistance"));
	m_upGlobalOptions->setLayerBlendMapSize(data.GetInt("blendmap::texturesize"));

	auto &render = Phobos::OgreEngine::Render::GetInstance();

	//u16TerrainSize = dict.GetInt("pagemapsize");
	//pclTerrainGroup = render->CreateTerrainGroup(Ogre::Terrain::ALIGN_X_Z, u16TerrainSize, dict.GetFloat("pageworldsize"));
	m_pclTerrainGroup = render.CreateTerrainGroup(Ogre::Terrain::ALIGN_X_Z, data.GetInt("pagemapsize"), data.GetFloat("pageworldsize"));
	m_pclTerrainGroup->setOrigin(Ogre::Vector3::ZERO);
	m_pclTerrainGroup->setFilenameConvention("Page", "ogt");

	if(terrainLight != nullptr)
	{
		m_upGlobalOptions->setLightMapDirection(terrainLight->GetDirection());
		m_upGlobalOptions->setCompositeMapAmbient(render.GetAmbientColor());
		m_upGlobalOptions->setCompositeMapDiffuse(terrainLight->GetDiffuseColour());
	}

	Phobos::String_t baseName = Phobos::String_t(levelPath.data()) + "/" + data.GetString("terrainDir") + "/";

	for(auto &pair : data)
	{
		auto &pageTable = *static_cast<const Phobos::Register::Table*>(pair.second);

		int pageX = pageTable.GetInt("pagex");
		int pageY = pageTable.GetInt("pagey");

		Phobos::String_t name = baseName + m_pclTerrainGroup->generateFilename(pageX, pageY);
		m_pclTerrainGroup->defineTerrain(pageX, pageY, name);
		m_pclTerrainGroup->loadTerrain(pageX, pageY, true);
	}
}

Terrain::~Terrain()
{	
	Phobos::OgreEngine::Render::GetInstance().DestroyTerrainGroup(m_pclTerrainGroup);
}

static void SetWorldTransformOnTable(Phobos::Register::Table &table, const Phobos::Game::MapObject &node)
{
	Phobos::Register::SetVector3(table, PH_MAP_OBJECT_KEY_WORLD_POSITION, node.GetWorldPosition());
	Phobos::Register::SetVector3(table, PH_MAP_OBJECT_KEY_WORLD_SCALE, node.GetWorldScale());
	Phobos::Register::SetQuaternion(table, PH_MAP_OBJECT_KEY_WORLD_ORIENTATION, node.GetWorldOrientation());
}

//
//
//MapWorldImpl
//
//

namespace
{
	class MapWorldImpl;

	//Internal shortcut
	static MapWorldImpl *g_pclMapWorld = nullptr;		

	class MapWorldImpl: public Phobos::Game::MapWorld
	{
		public:
			MapWorldImpl()
			{
				g_pclMapWorld = this;				
			}

			virtual ~MapWorldImpl() override
			{
				g_pclMapWorld = nullptr;
			}

			virtual void OnFixedUpdate() override;

			virtual void OnStart() override;
			virtual void OnUpdate() override;
						
			//The return object is valid as long the current level is valid, like a iterator is valid as long the conteiner is not changed
			//The pointer can be deleted by the caller, MapWorld will notice and clean up
			virtual Phobos::Game::MapObject *CreateObject(Phobos::Register::Table &table) override;			

			virtual Phobos::Game::MapObject &GetMapObject(const Phobos::String_t &name) override;

		protected:			
			virtual void Load(Phobos::StringRef_t levelPath, const Phobos::Register::Hive &hive) override;			
			virtual void Unload() override;					

		private:
			std::unique_ptr<Phobos::Game::MapObject> MakeMapObject(Phobos::OgreEngine::SceneNodeUniquePtr_t &&sceneNode, Phobos::Register::Table &table);

		private:
			std::vector<Terrain>						m_vecTerrains;			
	};	
}

std::unique_ptr<Phobos::Game::MapObject> MapWorldImpl::MakeMapObject(Phobos::OgreEngine::SceneNodeUniquePtr_t &&sceneNode, Phobos::Register::Table &table)
{
	using namespace Phobos;

	return  std::make_unique<Game::MapObject>(table.GetName(), std::move(sceneNode), std::ref(table));	
}

void MapWorldImpl::Load(Phobos::StringRef_t levelPath, const Phobos::Register::Hive &hive)
{
	using namespace Phobos;

	auto &render = OgreEngine::Render::GetInstance();
	
	std::vector<Register::Table *> vecTerrains;
	std::map<String_t, std::tuple<const String_t *, OgreEngine::SceneNodeUniquePtr_t, Register::Table &, StringRef_t, Game::MapObject *>> mapObjects;	
	
	for(auto it : hive)		
	{
		auto &dict = *static_cast<Register::Table *>(it.second);

		StringRef_t type = dict.GetString(PH_MAP_OBJECT_KEY_TYPE);
		if (type.compare(PH_MAP_OBJECT_TYPE_TERRAIN) == 0)
		{
			vecTerrains.push_back(&dict);
			continue;
		}
		else if (type.compare(PH_MAP_OBJECT_TYPE_SCENE_MANAGER) == 0)
		{
			render.SetAmbientColor(Register::GetColour(dict, "ambient"));
			continue;
		}

		auto sceneNode = render.CreateSceneNode(dict.GetName());		

		sceneNode->setPosition(Register::GetVector3(dict, PH_MAP_OBJECT_KEY_POSITION));
		sceneNode->setOrientation(Register::GetQuaternion(dict, PH_MAP_OBJECT_KEY_ORIENTATION));
		sceneNode->setScale(Register::GetVector3(dict, PH_MAP_OBJECT_KEY_SCALE));
						
		mapObjects.insert(std::make_pair(dict.GetName(), std::make_tuple(dict.TryGetString(PH_MAP_OBJECT_KEY_PARENT_NODE), std::move(sceneNode), std::ref(dict), type, nullptr)));
	}
	
	//
	//Reconstruct the scene hierarchy
	for (auto &pair : mapObjects)
	{
		auto parentName = std::get<0>(pair.second);
		if (parentName)
		{
			auto parentNode = std::get<1>(mapObjects.find(*parentName)->second).get();
			auto currentNode = std::get<1>(pair.second).get();

			currentNode->getParent()->removeChild(currentNode);
			parentNode->addChild(currentNode);
		}
	}
	
	//Construct MapObject
	for (auto &pair : mapObjects)
	{
		auto ptrMapObject = MakeMapObject(std::move(std::get<1>(pair.second)), std::get<2>(pair.second));

		auto pMapObject = ptrMapObject.get();

		this->AddPrivateChild(std::move(ptrMapObject));		

		std::get<4>(pair.second) = pMapObject;
	}

	if (!vecTerrains.empty())
	{
		Phobos::Game::LightComponent *terrainLight = nullptr;

		//find a suitable light for terrain
		for (auto &pair : mapObjects)
		{
			auto type = std::get<3>(pair.second);
			if (type.compare(PH_MAP_OBJECT_TYPE_STATIC_LIGHT))
				continue;

			auto pObject = std::get<4>(pair.second);

			auto enumerator = pObject->MakeEnumerator(PH_LIGHT_COMPONENT_NAME);

			while (enumerator.Next())
			{
				auto light = static_cast<Game::LightComponent *>(enumerator.GetCurrent());

				if (!light->IsDirectional())
					continue;

				terrainLight = light;
				break;
			}

			if (terrainLight)
				break;
		}

		//
		//Load terrain
		for (auto &table : vecTerrains)
		{
			m_vecTerrains.emplace_back(levelPath, *table, terrainLight);
		}
	}	
}

Phobos::Game::MapObject *MapWorldImpl::CreateObject(Phobos::Register::Table &table)
{
	using namespace Phobos;	

	auto &render = OgreEngine::Render::GetInstance();	

	render.SetAmbientColor(Ogre::ColourValue::White);

	StringRef_t ref = table.GetString(PH_MAP_OBJECT_KEY_TYPE);
	if ((ref.compare(PH_MAP_OBJECT_TYPE_TERRAIN) == 0) || (ref.compare(PH_MAP_OBJECT_TYPE_SCENE_MANAGER) == 0))
	{
		std::stringstream stream;

		stream << "Object " << ref << " not supported";
		PH_RAISE(INVALID_OPERATION_EXCEPTION, "MapWorldImpl::MakeObject", stream.str());		
	}

	auto sceneNode = render.CreateSceneNode(table.GetName());
	
	sceneNode->setPosition(Register::GetVector3(table, PH_MAP_OBJECT_KEY_POSITION));
	sceneNode->setOrientation(Register::GetQuaternion(table, PH_MAP_OBJECT_KEY_ORIENTATION));
	sceneNode->setScale(Register::GetVector3(table, PH_MAP_OBJECT_KEY_SCALE));
	
	auto ptrObject = this->MakeMapObject(std::move(sceneNode), table);
	auto parentName = table.TryGetString(PH_MAP_OBJECT_KEY_PARENT_NODE);
	if (parentName)
	{

		PH_RAISE(INVALID_OPERATION_EXCEPTION, "MapWorld", "SetParent Not implemented");
#if 0
		object.SetParentNode(render.GetSceneNode(*parentName));
#endif
	}
	
	SetWorldTransformOnTable(table, *ptrObject.get());
	
	auto *finalPtr = ptrObject.get();

	this->AddPrivateChild(std::move(ptrObject));

	return finalPtr;
}

Phobos::Game::MapObject &MapWorldImpl::GetMapObject(const Phobos::String_t &name)
{
	return static_cast<Phobos::Game::MapObject&>(this->GetChild(name));
}

void MapWorldImpl::Unload()
{
	m_vecTerrains.clear();	
	
	this->RemoveAllChildren();
}

void MapWorldImpl::OnStart()
{
	auto hive = Phobos::Register::TryGetHive(PH_MAP_STATIC_OBJECT_DEF_HIVE);
	if (!hive)
	{
		hive = &Phobos::Register::CreateCustomHive(PH_MAP_STATIC_OBJECT_DEF_HIVE);
	}

	Ogre::StringVectorPtr pList = Ogre::ResourceGroupManager::getSingleton().findResourceNames("PH_GameData", "*.mesh", false);

	for (Ogre::StringVector::iterator it = pList->begin(), end = pList->end(); it != end; ++it)
	{
		Phobos::Path path(it->c_str());

		path.StripExtension();

		if (!hive->TryGetTable(path))
		{
			auto pTable = std::make_unique<Phobos::Register::Table>(path);

			pTable->SetInherited(PH_MAP_STATIC_OBJECT_BASE_DEF);
			pTable->SetString(PH_MAP_OBJECT_KEY_MESH, *it);

			hive->AddTable(std::move(pTable));
		}
	}
}

void MapWorldImpl::OnFixedUpdate()
{
	Phobos::Game::MapObjectComponent::TickReminders();
	Phobos::Game::MapObjectComponent::TickFixedUpdate();
}

void MapWorldImpl::OnUpdate()
{
	Phobos::Game::MapObjectComponent::TickUpdate();
	Phobos::Game::DynamicBodyComponent::SyncAllToPhysics();	
}

namespace Phobos
{
	namespace Game
	{
		MapWorld &MapWorld::CreateInstance()
		{
			PH_ASSERT(!ipInstance_gl);

			ipInstance_gl.reset(PH_NEW MapWorldImpl());

			return *ipInstance_gl;
		}		
	}
}
