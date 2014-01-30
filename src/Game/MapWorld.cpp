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

#include "Phobos/Game/MapWorld.h"

#include <Phobos/Exception.h>
#include <Phobos/HandlerList.h>
#include <Phobos/Path.h>

#include <Phobos/OgreEngine/Render.h>
#include <Phobos/OgreEngine/Math/Transform.h>

#include <Phobos/Register/Hive.h>
#include <Phobos/Register/Table.h>

#include "Phobos/Game/MapDefs.h"

#include "Phobos/Game/RegisterUtils.h"

#include "Phobos/Game/Physics/Manager.h"
#include "Phobos/Game/Physics/RigidBody.h"
#include "Phobos/Game/Physics/Settings.h"
#include "Phobos/Game/Physics/PhysicsUtils.h"

#include <OgreEntity.h>
#include <OgreSceneNode.h>

#include <Terrain/OgreTerrainGroup.h>
#include <Terrain/OgreTerrainPrerequisites.h>

#include <tuple>
#include <utility>

PH_SINGLETON_PROCS(Phobos::Game::MapWorld,);
PH_DEFINE_SINGLETON_VAR(Phobos::Game::MapWorld);

namespace Phobos
{
	namespace Game
	{
		class SceneNodeObject
		{				
			private:
				Ogre::SceneNode *m_pclSceneNode;
				Ogre::Entity	*m_pclEntity;
				Ogre::Light		*m_pclLight;

				Physics::RigidBody	m_clRigidBody;

				SceneNodeObject(const SceneNodeObject &);
				SceneNodeObject &operator=(const SceneNodeObject &);

			public:
				SceneNodeObject(Ogre::SceneNode *sceneNode, Ogre::Entity *entity, Ogre::Light *light):			
					m_pclSceneNode(sceneNode),
					m_pclEntity(entity),
					m_pclLight(light)
				{
					//empty
				}

				SceneNodeObject(SceneNodeObject &&rhs):
					m_pclSceneNode(std::move(rhs.m_pclSceneNode)),
					m_pclEntity(std::move(rhs.m_pclEntity)),
					m_pclLight(std::move(rhs.m_pclLight)),
					m_clRigidBody(std::move(rhs.m_clRigidBody))
				{
					rhs.m_pclSceneNode = nullptr;
					rhs.m_pclLight = nullptr;
					rhs.m_pclEntity = nullptr;
				}
								
				~SceneNodeObject()
				{
					auto &render = Phobos::OgreEngine::Render::GetInstance();

					if(m_pclSceneNode)
					{
						render.DestroySceneNode(m_pclSceneNode);
						m_pclSceneNode = NULL;
					}

					if(m_pclLight)
					{
						render.DestroyLight(m_pclLight);
						m_pclLight = NULL;
					}

					if(m_pclEntity)
					{
						render.DestroyEntity(m_pclEntity);
						m_pclEntity = NULL;
					}
				}	

				SceneNodeObject &operator=(SceneNodeObject &&rhs)
				{
					std::swap(rhs.m_pclSceneNode, m_pclSceneNode);
					std::swap(rhs.m_pclEntity, m_pclEntity);
					std::swap(rhs.m_pclLight, m_pclLight);
					std::swap(rhs.m_clRigidBody, m_clRigidBody);

					return *this;
				}

				inline void AttachLight(Ogre::Light *light)
				{				
					PH_ASSERT(!m_pclLight);
			
					m_pclLight = light;
					m_pclSceneNode->attachObject(m_pclLight);
				}

				inline void AttachEntity(Ogre::Entity *entity)
				{
					PH_ASSERT(!m_pclEntity);

					m_pclEntity = entity;
					m_pclSceneNode->attachObject(entity);
				}		

				inline void SetRigidBody(Physics::RigidBody &&body)
				{
					m_clRigidBody = std::move(body);
				}

				inline void SetParentNode(Ogre::SceneNode *node)
				{
					m_pclSceneNode->getParent()->removeChild(m_pclSceneNode);
					node->addChild(m_pclSceneNode);
				}

				//Why using those _ functions: http://89.151.96.106/forums/viewtopic.php?f=22&t=62386
				//http://www.ogre3d.org/forums/viewtopic.php?p=221113
				//http://www.ogre3d.org/tikiwiki/-SceneNode
				inline const Ogre::Vector3 &GetWorldPosition() const
				{
					return m_pclSceneNode->_getDerivedPosition();
				}

				inline const Ogre::Vector3 &GetWorldScale() const
				{
					return m_pclSceneNode->_getDerivedScale();
				}

				inline const Ogre::Quaternion &GetWorldOrientation() const
				{
					return m_pclSceneNode->_getDerivedOrientation();
				}

				inline void SetPosition(const Ogre::Vector3 &position)
				{
					m_pclSceneNode->setPosition(position);
				}

				inline void SetOrientation(const Ogre::Quaternion &orientation)
				{
					m_pclSceneNode->setOrientation(orientation);
				}

				inline bool HasEntity() const
				{
					return m_pclEntity ? true : false;
				}

				inline const Ogre::Entity *GetEntity() const
				{
					return m_pclEntity;
				}
		};
	}
}

namespace
{		
	class Terrain
	{		
		public:
			Terrain(Phobos::StringRef_t levelPath, const Phobos::Register::Table &data, Ogre::Light *terrainLight);
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

Terrain::Terrain(Phobos::StringRef_t levelPath, const Phobos::Register::Table &data, Ogre::Light *terrainLight):
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
		m_upGlobalOptions->setLightMapDirection(terrainLight->getDirection());
		m_upGlobalOptions->setCompositeMapAmbient(render.GetAmbientColor());
		m_upGlobalOptions->setCompositeMapDiffuse(terrainLight->getDiffuseColour());
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

namespace
{
	class MapWorldImpl;

	//Internal shortcut
	static MapWorldImpl *g_pclMapWorld = nullptr;			

	class MapWorldImpl: public Phobos::Game::MapWorld
	{
		private:
			typedef Phobos::HandlerList<Phobos::Game::SceneNodeObject> SceneNodeList_t;
					
		public:
			MapWorldImpl()				
			{
				g_pclMapWorld = this;
			}

			virtual ~MapWorldImpl() override
			{
				g_pclMapWorld = nullptr;
			}

			virtual Phobos::Game::SceneNodeKeeper AcquireDynamicSceneNodeKeeper(Phobos::StringRef_t serial) override;
			void DestroyDynamicNode(Phobos::Handler h);

			virtual Phobos::Handler MakeObject(Phobos::Register::Table &table) override;

		protected:			
			virtual void Load(Phobos::StringRef_t levelPath, const Phobos::Register::Hive &hive) override;			
			virtual void Unload() override;			

		private:			
			SceneNodeList_t			m_lstNodes;		

			std::vector<Terrain>	m_vecTerrains;
	};	
}

static Ogre::Light &LoadLight(Phobos::Game::SceneNodeObject &temp, const Phobos::Register::Table &dict)
{
	using namespace Phobos;

	auto light = Phobos::OgreEngine::Render::GetInstance().CreateLight();
	
	temp.AttachLight(light);
			
	light->setCastShadows(dict.GetBool(PH_MAP_OBJECT_KEY_CAST_SHADOWS));

	StringRef_t lightType = dict.GetString(PH_MAP_OBJECT_KEY_LIGHT_TYPE);

	if(lightType.compare(PH_MAP_OBJECT_LIGHT_TYPE_POINT) == 0)
	{
		light->setType(Ogre::Light::LT_POINT);
	}
	else if(lightType.compare(PH_MAP_OBJECT_LIGHT_TYPE_DIRECTIONAL) == 0)
	{
		light->setType(Ogre::Light::LT_DIRECTIONAL);		
	}
	else if(lightType.compare(PH_MAP_OBJECT_LIGHT_TYPE_SPOT) == 0)
	{
		light->setType(Ogre::Light::LT_SPOTLIGHT);
		Ogre::Vector3 lightRange = Register::GetVector3(dict, PH_MAP_OBJECT_KEY_LIGHT_RANGE);
		light->setSpotlightRange(Ogre::Degree(lightRange.x), Ogre::Degree(lightRange.y), lightRange.z);
	}
	else
	{
		std::stringstream stream;

		stream << "Invalid light type " << lightType;
		PH_RAISE(INVALID_PARAMETER_EXCEPTION, "MapWorld::LoadLight", stream.str());
	}

	float attenuation[4];
	dict.Get4Float(attenuation, "attenuation");
	light->setAttenuation(attenuation[0], attenuation[1], attenuation[2], attenuation[3]);

	light->setDiffuseColour(Register::GetColour(dict, "diffuse"));		

	if(light->getType() != Ogre::Light::LT_POINT)
		light->setDirection(Register::GetVector3(dict, "direction"));

	light->setPowerScale(dict.GetFloat("power"));
	light->setSpecularColour(Register::GetColour(dict, "specular"));	

	return *light;
}

Phobos::Game::Physics::RigidBody CreateStaticObjectRigidBody(const Ogre::Entity &entity, const Phobos::Engine::Math::Transform &transform, const Ogre::Vector3 &scale, const Phobos::Game::Physics::CollisionTag &collisionTag)
{		
	const Ogre::MeshPtr mesh = entity.getMesh();
	const auto &meshName = mesh->getName();
		
	Phobos::Path path(meshName);
	path.StripExtension();

	using namespace Phobos;

	auto &physicsManager = Game::Physics::Manager::GetInstance();

	auto collisionDef = Game::Physics::Settings::TryGetStaticMeshCollisionShapeDef(path.GetStr());

#if 1

	auto body = collisionDef != NULL ?
		physicsManager.CreateRigidBody(Game::Physics::RBT_STATIC, transform, 0, collisionTag, Game::Physics::Utils::CreateCollisionShape(*collisionDef, scale)) :
		physicsManager.CreateMeshRigidBody(Game::Physics::RBT_STATIC, transform, 0, collisionTag, *mesh, scale)
	;

	body.Register();

	return body;	
#endif
}

static void SetWorldTransformOnTable(Phobos::Register::Table &table, const Phobos::Game::SceneNodeObject &node)
{	
	Phobos::Register::SetVector3(table, PH_MAP_OBJECT_KEY_WORLD_POSITION, node.GetWorldPosition());
	Phobos::Register::SetVector3(table, PH_MAP_OBJECT_KEY_WORLD_SCALE, node.GetWorldScale());
	Phobos::Register::SetQuaternion(table, PH_MAP_OBJECT_KEY_WORLD_ORIENTATION, node.GetWorldOrientation());
}

void MapWorldImpl::Load(Phobos::StringRef_t levelPath, const Phobos::Register::Hive &hive)
{
	using namespace Phobos;

	auto &render = OgreEngine::Render::GetInstance();

	std::vector<std::tuple<const String_t *, Handler, Register::Table &>> vecObjectsCache;
	std::vector<Register::Table *> vecTerrains;

	Ogre::Light *terrainLight = nullptr;

	auto staticCollisionTag = Game::Physics::Settings::CreateStaticWorldCollisionTag();
	
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

		Game::SceneNodeObject object(sceneNode, nullptr, nullptr);		

		sceneNode->setPosition(Register::GetVector3(dict, PH_MAP_OBJECT_KEY_POSITION));
		sceneNode->setOrientation(Register::GetQuaternion(dict, PH_MAP_OBJECT_KEY_ORIENTATION));
		sceneNode->setScale(Register::GetVector3(dict, PH_MAP_OBJECT_KEY_SCALE));

		if(auto str = dict.TryGetString(PH_MAP_OBJECT_KEY_MESH))
		{
			object.AttachEntity(render.CreateEntity(*str));			
		}

		if(ref.compare(PH_MAP_OBJECT_TYPE_STATIC_LIGHT) == 0)
		{
			auto &light = LoadLight(object, dict);

			if(!terrainLight && light.getType() == Ogre::Light::LT_DIRECTIONAL)
			{
				terrainLight = &light;
			}
		}
		else if((ref.compare(PH_MAP_OBJECT_TYPE_STATIC) == 0) && (object.HasEntity()))
		{			
			Engine::Math::Transform transform(				
				object.GetWorldPosition(),
				object.GetWorldOrientation()
			);		

			object.SetRigidBody(CreateStaticObjectRigidBody(*object.GetEntity(), transform, object.GetWorldScale(), staticCollisionTag));
		}
		
		auto result = m_lstNodes.Add(std::move(object));
				
		//Insert a key to allow entities to retrieve a handler to their nodes, so they can control it
		dict.SetString(PH_MAP_OBJECT_KEY_RENDER_OBJECT_HANDLER, result.first.ToString());	
			
		vecObjectsCache.emplace_back(dict.TryGetString(PH_MAP_OBJECT_KEY_PARENT_NODE), result.first, std::ref(dict));			
	}

	//
	//Reconstruct the scene hierarchy
	for(auto tuple : vecObjectsCache)
	{
		auto parentName = std::get<0>(tuple);
		if(parentName)
		{
			auto parentNode = render.GetSceneNode(*parentName);
			m_lstNodes.Get(std::get<1>(tuple)).SetParentNode(parentNode);
		}
	}

	//
	//Load terrain
	for(auto &table : vecTerrains)
	{
		m_vecTerrains.emplace_back(levelPath, *table, terrainLight);
	}

	//
	//Save final transformation in world coordinates
	for(auto tuple : vecObjectsCache)
	{		
		auto &object = m_lstNodes.Get(std::get<1>(tuple));
		auto &dict = std::get<2>(tuple);

		SetWorldTransformOnTable(dict, object);	
	}	
}

Phobos::Handler MapWorldImpl::MakeObject(Phobos::Register::Table &table)
{
	using namespace Phobos;

	auto &render = OgreEngine::Render::GetInstance();	

	StringRef_t ref = table.GetString(PH_MAP_OBJECT_KEY_TYPE);
	if ((ref.compare(PH_MAP_OBJECT_TYPE_TERRAIN) == 0) || (ref.compare(PH_MAP_OBJECT_TYPE_SCENE_MANAGER) == 0))
	{
		std::stringstream stream;

		stream << "Object " << ref << " not supported";
		PH_RAISE(INVALID_OPERATION_EXCEPTION, "MapWorldImpl::MakeObject", stream.str());		
	}

	auto sceneNode = render.CreateSceneNode(table.GetName());
	
	Game::SceneNodeObject object(sceneNode, nullptr, nullptr);

	sceneNode->setPosition(Register::GetVector3(table, PH_MAP_OBJECT_KEY_POSITION));
	sceneNode->setOrientation(Register::GetQuaternion(table, PH_MAP_OBJECT_KEY_ORIENTATION));
	sceneNode->setScale(Register::GetVector3(table, PH_MAP_OBJECT_KEY_SCALE));

	if (auto str = table.TryGetString(PH_MAP_OBJECT_KEY_MESH))
	{
		object.AttachEntity(render.CreateEntity(*str));
	}

	if (ref.compare(PH_MAP_OBJECT_TYPE_STATIC_LIGHT) == 0)
	{
		LoadLight(object, table);		
	}
	else if ((ref.compare(PH_MAP_OBJECT_TYPE_STATIC) == 0) && (object.HasEntity()))
	{
		Engine::Math::Transform transform(
			object.GetWorldPosition(),
			object.GetWorldOrientation()
		);

		auto staticCollisionTag = Game::Physics::Settings::CreateStaticWorldCollisionTag();
		object.SetRigidBody(CreateStaticObjectRigidBody(*object.GetEntity(), transform, object.GetWorldScale(), staticCollisionTag));
	}

	auto result = m_lstNodes.Add(std::move(object));

	//Insert a key to allow entities to retrieve a handler to their nodes, so they can control it
	table.SetString(PH_MAP_OBJECT_KEY_RENDER_OBJECT_HANDLER, result.first.ToString());

	auto parentName = table.TryGetString(PH_MAP_OBJECT_KEY_PARENT_NODE);
	if (parentName)
	{
		object.SetParentNode(render.GetSceneNode(*parentName));
	}
	
	SetWorldTransformOnTable(table, result.second);

	return result.first;
}

void MapWorldImpl::Unload()
{
	m_vecTerrains.clear();
	m_lstNodes.Clear();
}

Phobos::Game::SceneNodeKeeper MapWorldImpl::AcquireDynamicSceneNodeKeeper(Phobos::StringRef_t handler)
{
	return m_lstNodes.Acquire<Phobos::Game::SceneNodeKeeper>(Phobos::Handler(handler));	
}

void MapWorldImpl::DestroyDynamicNode(Phobos::Handler h)
{
	//Ignore nulls
	if(!h.GetSerial())
		return;

	m_lstNodes.Remove(h);	
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

Phobos::Game::SceneNodeKeeper::SceneNodeKeeper():
	m_pclSceneNode(nullptr)
{
	//empty
}

Phobos::Game::SceneNodeKeeper::SceneNodeKeeper(SceneNodeObject &object, Handler h):
	m_pclSceneNode(&object),
	m_hHandler(h)
{
	PH_ASSERT(m_hHandler && "Constructor with object requires a valid serial");
}

Phobos::Game::SceneNodeKeeper::SceneNodeKeeper(SceneNodeKeeper &&other):
	m_pclSceneNode(nullptr)	
{
	*this = std::move(other);	
}

Phobos::Game::SceneNodeKeeper &Phobos::Game::SceneNodeKeeper::operator=(SceneNodeKeeper &&rhs)
{
	g_pclMapWorld->DestroyDynamicNode(m_hHandler);
	
	this->m_pclSceneNode = rhs.m_pclSceneNode;
	this->m_hHandler = rhs.m_hHandler;	

	rhs.m_hHandler = Handler();
	rhs.m_pclSceneNode = nullptr;

	return *this;
}

void Phobos::Game::SceneNodeKeeper::SetPosition(const Ogre::Vector3 &position)
{
	PH_ASSERT(m_pclSceneNode && "Invalid handler");

	m_pclSceneNode->SetPosition(position);
}

void Phobos::Game::SceneNodeKeeper::SetOrientation(const Ogre::Quaternion &orientation)
{
	PH_ASSERT(m_pclSceneNode && "Invalid handler");

	m_pclSceneNode->SetOrientation(orientation);
}

Phobos::Game::SceneNodeKeeper::~SceneNodeKeeper()
{	
	g_pclMapWorld->DestroyDynamicNode(m_hHandler);
}

