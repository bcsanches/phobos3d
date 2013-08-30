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

#include <Phobos/OgreEngine/Render.h>
#include <Phobos/Register/Hive.h>
#include <Phobos/Register/Table.h>

#include "Phobos/Game/MapDefs.h"

#include "Phobos/Game/RegisterUtils.h"

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
					m_pclSceneNode(rhs.m_pclSceneNode),
					m_pclEntity(rhs.m_pclEntity),
					m_pclLight(rhs.m_pclLight)
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
			
	light->setCastShadows(dict.GetBool(PH_GAME_OBJECT_KEY_CAST_SHADOWS));

	StringRef_t lightType = dict.GetString(PH_GAME_OBJECT_KEY_LIGHT_TYPE);

	if(lightType.compare(PH_GAME_OBJECT_LIGHT_TYPE_POINT) == 0)
	{
		light->setType(Ogre::Light::LT_POINT);
	}
	else if(lightType.compare(PH_GAME_OBJECT_LIGHT_TYPE_DIRECTIONAL) == 0)
	{
		light->setType(Ogre::Light::LT_DIRECTIONAL);		
	}
	else if(lightType.compare(PH_GAME_OBJECT_LIGHT_TYPE_SPOT) == 0)
	{
		light->setType(Ogre::Light::LT_SPOTLIGHT);
		Ogre::Vector3 lightRange = Register::GetVector3(dict, PH_GAME_OBJECT_KEY_LIGHT_RANGE);
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

void MapWorldImpl::Load(Phobos::StringRef_t levelPath, const Phobos::Register::Hive &hive)
{
	using namespace Phobos;

	auto &render = OgreEngine::Render::GetInstance();

	std::vector<std::tuple<const String_t *, Handler, Register::Table &>> vecObjectsCache;
	std::vector<Register::Table *> vecTerrains;

	Ogre::Light *terrainLight = nullptr;
	
	for(auto it : hive)		
	{
		auto &dict = *static_cast<Register::Table *>(it.second);

		StringRef_t ref = dict.GetString(PH_GAME_OBJECT_KEY_TYPE);
		if(ref.compare(PH_GAME_OBJECT_TYPE_TERRAIN) == 0)
		{
			vecTerrains.push_back(&dict);
			continue;
		}

		if(ref.compare(PH_GAME_OBJECT_TYPE_SCENE_MANAGER) == 0)
		{
			render.SetAmbientColor(Register::GetColour(dict, "ambient"));
			continue;
		}

		auto sceneNode = render.CreateSceneNode(dict.GetName());

		Game::SceneNodeObject object(sceneNode, nullptr, nullptr);		

		sceneNode->setPosition(Register::GetVector3(dict, PH_GAME_OBJECT_KEY_POSITION));
		sceneNode->setOrientation(Register::GetQuaternion(dict, PH_GAME_OBJECT_KEY_ORIENTATION));
		sceneNode->setScale(Register::GetVector3(dict, PH_GAME_OBJECT_KEY_SCALE));

		if(auto str = dict.TryGetString(PH_GAME_OBJECT_KEY_MESH))
		{
			object.AttachEntity(render.CreateEntity(*str));			
		}

		if(ref.compare(PH_GAME_OBJECT_TYPE_STATIC_LIGHT) == 0)
		{
			auto &light = LoadLight(object, dict);

			if(!terrainLight && light.getType() == Ogre::Light::LT_DIRECTIONAL)
			{
				terrainLight = &light;
			}
		}
		
		auto result = m_lstNodes.Add(std::move(object));
				
		//Insert a key to allow entities to retrieve a handler to their nodes, so they can control it
		dict.SetString(PH_GAME_OBJECT_KEY_RENDER_OBJECT_HANDLER, result.first.ToString());	
			
		vecObjectsCache.emplace_back(dict.TryGetString(PH_GAME_OBJECT_KEY_PARENT_NODE), result.first, std::ref(dict));
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

		Phobos::Register::SetVector3(dict, PH_GAME_OBJECT_KEY_WORLD_POSITION, object.GetWorldPosition());
		Phobos::Register::SetVector3(dict, PH_GAME_OBJECT_KEY_WORLD_SCALE, object.GetWorldScale());
		Phobos::Register::SetQuaternion(dict, PH_GAME_OBJECT_KEY_WORLD_ORIENTATION, object.GetWorldOrientation());		
	}	
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

