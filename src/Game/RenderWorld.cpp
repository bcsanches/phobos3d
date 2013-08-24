#include "Phobos/Game/RenderWorld.h"

#include <Phobos/Exception.h>

#include <Phobos/OgreEngine/Render.h>
#include <Phobos/Register/Hive.h>
#include <Phobos/Register/Table.h>

#include "Phobos/Game/MapDefs.h"

#include "Phobos/Game/RegisterUtils.h"

#include <OgreEntity.h>
#include <OgreSceneNode.h>

#include <tuple>
#include <utility>

PH_SINGLETON_PROCS(Phobos::Game::RenderWorld,);
PH_DEFINE_SINGLETON_VAR(Phobos::Game::RenderWorld);

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
	class RenderWorldImpl;

	//Internal shortcut
	static RenderWorldImpl *g_pclRenderWorld = nullptr;	

	#define MAX_HANDLERS 0xFFFFu

	template <typename T>
	class ObjectList
	{
		public:
			ObjectList():
				m_u16NextFreeSlot(MAX_HANDLERS)
			{
				//empty
			}

			std::pair<Phobos::UInt32_t, T&> Add(T &&object)
			{
				auto slot = m_u16NextFreeSlot;

				if(slot == MAX_HANDLERS)
				{					
					m_vecObjects.emplace_back(std::make_pair(MiniHandler(), std::move(object)));
					slot = m_vecObjects.size()-1;
				}
				else
				{					
					m_u16NextFreeSlot = m_vecObjects[slot].first.m_u16NextFreeSlot;

					m_vecObjects[slot].second = std::move(object);
				}

				return std::make_pair((m_vecObjects[slot].first.m_u16Serial << 16) | slot, std::ref(m_vecObjects[slot].second));
			}

			T Remove(Phobos::UInt16_t index, Phobos::UInt16_t serial)
			{
				if(index >= m_vecObjects.size())
				{
					PH_RAISE(Phobos::INVALID_PARAMETER_EXCEPTION, "[RenderWorldImpl::DestroyDynamicNode]", "handler is out of bounds");
				}

				if(m_vecObjects[index].first.m_u16Serial != serial)
				{
					PH_RAISE(Phobos::OBJECT_NOT_FOUND_EXCEPTION, "[RenderWorldImpl::DestroyDynamicNode]", "Object already destroyed");
				}				

				//increment serial to avoid re-use
				m_vecObjects[index].first.m_u16Serial++;

				//queue free slot
				m_vecObjects[index].first.m_u16NextFreeSlot = m_u16NextFreeSlot;
				m_u16NextFreeSlot = index;

				//destory it
				return std::move(m_vecObjects[index].second);
			}

			template <typename CARRIER>
			CARRIER Acquire(Phobos::StringRef_t handler)
			{
				using namespace Phobos;

				UInt32_t uHandler = std::stoi(handler.data());
				
				auto index = this->GetValidIndex(uHandler);

				return CARRIER(m_vecObjects[index].second, index, m_vecObjects[index].first.m_u16Serial);
			}

			T &Get(Phobos::UInt32_t uHandler)
			{				
				return m_vecObjects[GetValidIndex(uHandler)].second;
			}

			void Clear()
			{
				m_u16NextFreeSlot = MAX_HANDLERS;
				m_vecObjects.clear();
			}

		private:			

			Phobos::UInt16_t GetValidIndex(Phobos::UInt32_t uHandler)
			{
				Phobos::UInt16_t index = uHandler & 0x0000FFFF;
				Phobos::UInt16_t serial = (uHandler >> 16);

				if(index >= m_vecObjects.size())
				{
					PH_RAISE(Phobos::INVALID_PARAMETER_EXCEPTION, "[RenderWorldImpl::AcquireDynamicSceneNodeHandler]", "handler is out of bounds");
				}

				if(m_vecObjects[index].first.m_u16Serial != serial)
				{
					PH_RAISE(Phobos::OBJECT_NOT_FOUND_EXCEPTION, "[RenderWorldImpl::AcquireDynamicSceneNodeHandler]", "Invalid serial for handler");
				}

				//return an valid index
				return index;
			}

			struct MiniHandler
			{
				Phobos::UInt16_t m_u16Serial;
				Phobos::UInt16_t m_u16NextFreeSlot;

				MiniHandler():
					m_u16Serial(1),
					m_u16NextFreeSlot(MAX_HANDLERS)
				{
					//empty
				}
			};

		private:
			typedef std::vector<std::pair<MiniHandler, T>> Vector_t;

			Vector_t m_vecObjects;

			Phobos::UInt16_t m_u16NextFreeSlot;
	};	

	class RenderWorldImpl: public Phobos::Game::RenderWorld
	{
		public:
			RenderWorldImpl()				
			{
				g_pclRenderWorld = this;
			}

			virtual ~RenderWorldImpl() override
			{
				g_pclRenderWorld = nullptr;
			}

			virtual Phobos::Game::SceneNodeHandler AcquireDynamicSceneNodeHandler(Phobos::StringRef_t serial) override;
			void DestroyDynamicNode(Phobos::UInt16_t index, Phobos::UInt16_t serial);

		protected:			
			virtual void Load(const Phobos::Register::Hive &hive) override;
			virtual void Unload() override;

		private:
			typedef ObjectList<Phobos::Game::SceneNodeObject> SceneNodeList_t;

			SceneNodeList_t			m_lstNodes;			
	};	
}

static void LoadLight(Phobos::Game::SceneNodeObject &temp, const Phobos::Register::Table &dict)
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
		PH_RAISE(INVALID_PARAMETER_EXCEPTION, "RenderWorld::LoadLight", stream.str());
	}

	float attenuation[4];
	dict.Get4Float(attenuation, "attenuation");
	light->setAttenuation(attenuation[0], attenuation[1], attenuation[2], attenuation[3]);

	light->setDiffuseColour(Register::GetColour(dict, "diffuse"));		

	if(light->getType() != Ogre::Light::LT_POINT)
		light->setDirection(Register::GetVector3(dict, "direction"));

	light->setPowerScale(dict.GetFloat("power"));
	light->setSpecularColour(Register::GetColour(dict, "specular"));		
}

void RenderWorldImpl::Load(const Phobos::Register::Hive &hive)
{
	using namespace Phobos;

	auto &render = OgreEngine::Render::GetInstance();

	std::vector<std::tuple<const String_t *, UInt32_t, Register::Table &>> vecObjectsCache;
	
	for(auto it : hive)		
	{
		auto &dict = *static_cast<Register::Table *>(it.second);

		StringRef_t ref = dict.GetString(PH_GAME_OBJECT_KEY_TYPE);
		if(ref.compare(PH_GAME_OBJECT_TYPE_TERRAIN) == 0)
			continue;

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
			LoadLight(object, dict);
		}
		
		auto result = m_lstNodes.Add(std::move(object));
				
		//Insert a key to allow entities to retrieve a handler to their nodes, so they can control it
		dict.SetString(PH_GAME_OBJECT_KEY_RENDER_OBJECT_HANDLER, std::to_string(result.first));	
			
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

void RenderWorldImpl::Unload()
{
	m_lstNodes.Clear();
}

Phobos::Game::SceneNodeHandler RenderWorldImpl::AcquireDynamicSceneNodeHandler(Phobos::StringRef_t handler)
{
	return m_lstNodes.Acquire<Phobos::Game::SceneNodeHandler>(handler);	
}

void RenderWorldImpl::DestroyDynamicNode(Phobos::UInt16_t index, Phobos::UInt16_t serial)
{
	//Ignore nulls
	if(!serial)
		return;

	m_lstNodes.Remove(index, serial);	
}

namespace Phobos
{
	namespace Game
	{
		RenderWorld &RenderWorld::CreateInstance()
		{
			PH_ASSERT(!ipInstance_gl);

			ipInstance_gl.reset(PH_NEW RenderWorldImpl());

			return *ipInstance_gl;
		}
	}
}

Phobos::Game::SceneNodeHandler::SceneNodeHandler():
	m_pclSceneNode(nullptr),
	m_u16Index(0),
	m_u16Serial(0)
{
	//empty
}

Phobos::Game::SceneNodeHandler::SceneNodeHandler(SceneNodeObject &object, UInt16_t index, UInt16_t serial):
	m_pclSceneNode(&object),
	m_u16Index(index),
	m_u16Serial(serial)
{
	PH_ASSERT(serial && "Constructor with object requires a valid serial");
}

Phobos::Game::SceneNodeHandler::SceneNodeHandler(SceneNodeHandler &&other):
	m_pclSceneNode(nullptr),
	m_u16Index(0),
	m_u16Serial()
{
	*this = std::move(other);	
}

Phobos::Game::SceneNodeHandler &Phobos::Game::SceneNodeHandler::operator=(SceneNodeHandler &&rhs)
{
	g_pclRenderWorld->DestroyDynamicNode(m_u16Index, m_u16Serial);

	m_u16Serial = 0;
	m_pclSceneNode = nullptr;

	this->m_pclSceneNode = rhs.m_pclSceneNode;
	this->m_u16Serial = rhs.m_u16Serial;
	this->m_u16Index = rhs.m_u16Index;

	rhs.m_u16Serial = 0;
	rhs.m_pclSceneNode = nullptr;

	return *this;
}

void Phobos::Game::SceneNodeHandler::SetPosition(const Ogre::Vector3 &position)
{
	PH_ASSERT(m_pclSceneNode && "Invalid handler");

	m_pclSceneNode->SetPosition(position);
}

void Phobos::Game::SceneNodeHandler::SetOrientation(const Ogre::Quaternion &orientation)
{
	PH_ASSERT(m_pclSceneNode && "Invalid handler");

	m_pclSceneNode->SetOrientation(orientation);
}

Phobos::Game::SceneNodeHandler::~SceneNodeHandler()
{	
	g_pclRenderWorld->DestroyDynamicNode(m_u16Index, m_u16Serial);
}

