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
#include <Phobos/OgreEngine/Utils.h>
#include <Phobos/OgreEngine/Math/Transform.h>

#include <Phobos/Register/Manager.h>
#include <Phobos/Register/Hive.h>
#include <Phobos/Register/Table.h>

#include "Phobos/Game/Level/LightComponent.h"

#include "Phobos/Game/Level/MapDefs.h"
#include "Phobos/Game/Level/MapObject.h"
#include "Phobos/Game/Level/MapObjectComponentFactory.h"

#include "Phobos/Game/RegisterUtils.h"

#include "Phobos/Game/Physics/Manager.h"
#include "Phobos/Game/Physics/RigidBody.h"
#include "Phobos/Game/Physics/Settings.h"
#include "Phobos/Game/Physics/PhysicsUtils.h"

#include "Phobos/Game/Level/MapDefs.h"

#include <OgreEntity.h>
#include <OgreLight.h>
#include <OgreMeshManager.h>
#include <OgreSceneNode.h>

#include <Terrain/OgreTerrainGroup.h>
#include <Terrain/OgreTerrainPrerequisites.h>

#include <boost/pool/object_pool.hpp>

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

static Phobos::Game::Physics::RigidBody CreateStaticObjectRigidBody(const Ogre::Mesh &mesh, const Phobos::Engine::Math::Transform &transform, const Ogre::Vector3 &scale, const Phobos::Game::Physics::CollisionTag &collisionTag)
{	
	const auto &meshName = mesh.getName();

	Phobos::Path path(meshName);
	path.StripExtension();

	using namespace Phobos;

	auto &physicsManager = Game::Physics::Manager::GetInstance();

	auto collisionDef = Game::Physics::Settings::TryGetStaticMeshCollisionShapeDef(path.GetStr());

#if 1

	auto body = collisionDef != NULL ?
		physicsManager.CreateRigidBody(Game::Physics::RBT_STATIC, transform, 0, collisionTag, Game::Physics::Utils::CreateCollisionShape(*collisionDef, scale)) :
		physicsManager.CreateMeshRigidBody(Game::Physics::RBT_STATIC, transform, 0, collisionTag, mesh, scale)
	;	

	return body;
#endif
}

static bool LoadPhysics(Phobos::Game::MapObject::Data &mapObjectData, const Phobos::Register::Table &dict, Phobos::StringRef_t mapObjectType)
{
	using namespace Phobos;

	if (auto physicsTypeName = dict.TryGetString(PH_MAP_OBJECT_KEY_PHYSICS_TYPE))
	{
		auto physicsType = Game::StringToPhysicsType(physicsTypeName->c_str());
		if (physicsType == Game::PhysicsTypes::NONE)
			return true;

		if ((mapObjectType.compare(PH_MAP_OBJECT_TYPE_STATIC) == 0) && (physicsType != Game::PhysicsTypes::STATIC))
		{
			LogMakeStream() << "[Phobos::Game::MapWorld] Error: Static object can only have physics of type STATIC, object " << dict.GetName() << " not loaded";
			return false;
		}

		Engine::Math::Transform transform(
			mapObjectData.GetWorldPosition(),
			mapObjectData.GetWorldOrientation()
		);

		if (physicsType == Game::PhysicsTypes::STATIC)
		{
#if 1
			String_t meshName = Phobos::FixMeshName(dict.GetString(PH_MAP_OBJECT_KEY_MESH));			

			auto pMesh = Ogre::MeshManager::getSingleton().load(meshName, Ogre::ResourceGroupManager::AUTODETECT_RESOURCE_GROUP_NAME);

			mapObjectData.SetRigidBody(
				CreateStaticObjectRigidBody(
					*pMesh,
					transform, mapObjectData.GetWorldScale(), 
					Game::Physics::Settings::CreateStaticWorldCollisionTag()
				),
				physicsType
			);
#endif
		}
		else if (physicsType == Game::PhysicsTypes::DYNAMIC)
		{
			auto collisionTag = Game::Physics::Settings::LoadCollisionTag(dict);		

			Float_t mass = dict.GetFloat("mass");

			auto &physicsManager = Game::Physics::Manager::GetInstance();

			mapObjectData.SetRigidBody(
				physicsManager.CreateRigidBody(
					Game::Physics::RBT_DYNAMIC,
					transform,
					mass,
					collisionTag,
					Game::Physics::Utils::CreateCollisionShape(dict, Ogre::Vector3(1, 1, 1))
				),
				physicsType
			);			
		}		
	}
	
	return true;
}

static void SetWorldTransformOnTable(Phobos::Register::Table &table, const Phobos::Game::MapObject &node)
{
	Phobos::Register::SetVector3(table, PH_MAP_OBJECT_KEY_WORLD_POSITION, node.GetWorldPosition());
	Phobos::Register::SetVector3(table, PH_MAP_OBJECT_KEY_WORLD_SCALE, node.GetWorldScale());
	Phobos::Register::SetQuaternion(table, PH_MAP_OBJECT_KEY_WORLD_ORIENTATION, node.GetWorldOrientation());
}

static void CreateMapObjectComponents(Phobos::Game::MapObject &object, const Phobos::Register::Table &table)
{
	using namespace Phobos;

	if (const String_t *components = table.TryGetString(PH_MAP_OBJECT_KEY_COMPONENTS))
	{
		String_t componentName;
		size_t pos = 0;

		auto &factory = Game::MapObjectComponentFactory::GetInstance();

		while (StringSplitBy(componentName, *components, '|', pos, &pos))
		{
			auto comp(factory.Create(componentName, object, table));

			object.AddComponent(std::move(comp));
		}
	}
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
			struct MapObjectPoolTraits
			{
				typedef Phobos::Game::MapObject *pointer;

				void operator()(pointer ptr)
				{
					g_pclMapWorld->m_clPool.destroy(ptr);
				}
			};

			typedef std::unique_ptr<MapObjectTraits::pointer, MapObjectPoolTraits> MapObjectPoolUniquePtr_t;
					
		public:
			MapWorldImpl():
				m_clPool(4096)
			{
				g_pclMapWorld = this;				
			}

			virtual ~MapWorldImpl() override
			{
				g_pclMapWorld = nullptr;
			}

			virtual void OnStart() override;
			virtual void OnUpdate() override;
						
			virtual Phobos::Game::MapObject *CreateObject(Phobos::Register::Table &table) override;
			virtual void DestroyObject(Phobos::Game::MapObject *ptr) override;

		protected:			
			virtual void Load(Phobos::StringRef_t levelPath, const Phobos::Register::Hive &hive) override;			
			virtual void Unload() override;					

		private:
			MapObjectUniquePtr_t MakeMapObject(Phobos::Game::MapObject::Data &&mapObjectData, Phobos::Register::Table &table);

		private:			
			std::vector<MapObjectPoolUniquePtr_t>		m_vecObjects;

			std::vector<Phobos::Game::MapObject*>		m_vecDynamicBodies;
			std::vector<Terrain>						m_vecTerrains;

			boost::object_pool<Phobos::Game::MapObject> m_clPool;
	};	
}

Phobos::Game::MapWorld::MapObjectUniquePtr_t MapWorldImpl::MakeMapObject(Phobos::Game::MapObject::Data &&mapObjectData, Phobos::Register::Table &table)
{
	using namespace Phobos;

	//Make a sink, because boost pool cannot construct with move semantics
	Game::MapObject::DataSink dataSink(std::move(mapObjectData));	

	MapObjectPoolUniquePtr_t upMapObject(m_clPool.construct(std::ref(dataSink), std::ref(table)));

	//backup reference to object
	auto *object = upMapObject.get();

	//push it to object list
	m_vecObjects.push_back(std::move(upMapObject));

	//if dynamic body, track it on physics
	if (object->GetPhysicsType() == Phobos::Game::PhysicsTypes::DYNAMIC)
	{
		m_vecDynamicBodies.push_back(object);
	}

	//return a safe pointer to caller
	return MapObjectUniquePtr_t(object);
}

void MapWorldImpl::Load(Phobos::StringRef_t levelPath, const Phobos::Register::Hive &hive)
{
	using namespace Phobos;

	auto &render = OgreEngine::Render::GetInstance();
	
	std::vector<Register::Table *> vecTerrains;
	std::map<String_t, std::tuple<const String_t *, Ogre::SceneNode *, Register::Table &, Game::MapObject &, StringRef_t>> mapObjects;
	
	for(auto it : hive)		
	{
		auto &dict = *static_cast<Register::Table *>(it.second);

		StringRef_t ref = dict.GetString(PH_MAP_OBJECT_KEY_TYPE);
		if(ref.compare(PH_MAP_OBJECT_TYPE_TERRAIN) == 0)
		{
			vecTerrains.push_back(&dict);
			continue;
		}
		else if(ref.compare(PH_MAP_OBJECT_TYPE_SCENE_MANAGER) == 0)
		{
			render.SetAmbientColor(Register::GetColour(dict, "ambient"));
			continue;
		}

		auto sceneNode = render.CreateSceneNode(dict.GetName());

		Game::MapObject::Data mapObjectData(sceneNode);

		sceneNode->setPosition(Register::GetVector3(dict, PH_MAP_OBJECT_KEY_POSITION));
		sceneNode->setOrientation(Register::GetQuaternion(dict, PH_MAP_OBJECT_KEY_ORIENTATION));
		sceneNode->setScale(Register::GetVector3(dict, PH_MAP_OBJECT_KEY_SCALE));

		if (!LoadPhysics(mapObjectData, dict, ref))
			continue;		
		
		auto ptrMapObject = MakeMapObject(std::move(mapObjectData), dict);

		mapObjects.insert(std::make_pair(dict.GetName(), std::make_tuple(dict.TryGetString(PH_MAP_OBJECT_KEY_PARENT_NODE), sceneNode, std::ref(dict), std::ref(*ptrMapObject.get()), ref)));

		//release it so it is not destroyed
		ptrMapObject.release();
	}

	//
	//Reconstruct the scene hierarchy
	for (auto pair : mapObjects)
	{
		auto parentName = std::get<0>(pair.second);
		if (parentName)
		{
			auto parentNode = std::get<1>(mapObjects.find(*parentName)->second);
			auto currentNode = std::get<1>(pair.second);

			currentNode->getParent()->removeChild(currentNode);
			parentNode->addChild(std::get<1>(pair.second));			
		}
	}

	//
	//Save final transformation in world coordinates
#if 1
	for (auto pair : mapObjects)
	{		
		auto &object = std::get<3>(pair.second);
		auto &dict = std::get<2>(pair.second);

		SetWorldTransformOnTable(dict, object);	

		if (object.GetPhysicsType() != Phobos::Game::PhysicsTypes::NONE)
		{
			object.SyncPhysicsToScene();
			object.RegisterBody();
		}
	}	
#endif	
	for (auto pair : mapObjects)
	{
		auto &object = std::get<3>(pair.second);
		auto &dict = std::get<2>(pair.second);

		CreateMapObjectComponents(object, dict);
	}

	if (!vecTerrains.empty())
	{
		Phobos::Game::LightComponent *terrainLight = nullptr;

		//find a suitable light for terrain
		for (auto pair : mapObjects)
		{
			auto type = std::get<4>(pair.second);
			if (type.compare(PH_MAP_OBJECT_TYPE_STATIC_LIGHT))
				continue;

			auto &object = std::get<3>(pair.second);

			auto enumerator = object.MakeEnumerator(PH_LIGHT_COMPONENT_NAME);

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
	
	Game::MapObject::Data object(sceneNode);

	sceneNode->setPosition(Register::GetVector3(table, PH_MAP_OBJECT_KEY_POSITION));
	sceneNode->setOrientation(Register::GetQuaternion(table, PH_MAP_OBJECT_KEY_ORIENTATION));
	sceneNode->setScale(Register::GetVector3(table, PH_MAP_OBJECT_KEY_SCALE));

	if (!LoadPhysics(object, table, ref))
		PH_RAISE(INVALID_OPERATION_EXCEPTION, "MapWorldImpl::MakeObject", "Cannot create physics state");

	auto ptrObject = this->MakeMapObject(std::move(object), table);	
	auto parentName = table.TryGetString(PH_MAP_OBJECT_KEY_PARENT_NODE);
	if (parentName)
	{

		PH_RAISE(INVALID_OPERATION_EXCEPTION, "MapWorld", "SetParent Not implemented");
#if 0
		object.SetParentNode(render.GetSceneNode(*parentName));
#endif
	}
	
	SetWorldTransformOnTable(table, *ptrObject.get());

	CreateMapObjectComponents(*ptrObject.get(), table);

	auto *finalPtr = ptrObject.release();
	return finalPtr;
}

void MapWorldImpl::Unload()
{
	m_vecTerrains.clear();
	m_vecDynamicBodies.clear();
	m_vecObjects.clear();
}

void MapWorldImpl::DestroyObject(Phobos::Game::MapObject *ptr)
{
	if (!ptr)
		return;		

	m_vecObjects.erase(std::remove_if(m_vecObjects.begin(), m_vecObjects.end(), [ptr](const MapObjectPoolUniquePtr_t &current){ return current.get() == ptr; }), m_vecObjects.end());
	m_vecDynamicBodies.erase(std::remove_if(m_vecDynamicBodies.begin(), m_vecDynamicBodies.end(), [ptr](Phobos::Game::MapObject *current){return current == ptr; }), m_vecDynamicBodies.end());	
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

void MapWorldImpl::OnUpdate()
{
	for (auto h : m_vecDynamicBodies)
	{
		h->SyncSceneToPhysics();		
	}
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
