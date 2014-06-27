#ifndef PH_GAME_MAP_OBJECT_H
#define PH_GAME_MAP_OBJECT_H

#include <algorithm>

#include <OgrePrerequisites.h>

#include <Phobos/Node.h>
#include <Phobos/OgreEngine/Math/Transform.h>
#include <Phobos/Register/TableFwd.h>
#include <Phobos/String.h>

#include "Phobos/Game/Level/MapDefs.h"
#include "Phobos/Game/Level/MapObjectComponent.h"

namespace Phobos
{
	namespace Game
	{		
		class MapObject: public Node
		{
			private:
				typedef std::vector<MapObjectComponent::UniquePtr_t> ComponentsVector_t;

			public:
				class Data
				{
					friend MapObject;

					private:
						Ogre::SceneNode		*m_pclSceneNode;

					public:
						Data(Ogre::SceneNode *sceneNode) :
							m_pclSceneNode(sceneNode)							
						{
							//empty
						}

						Data(Data &&rhs) :
							m_pclSceneNode(std::move(rhs.m_pclSceneNode))
						{
							rhs.m_pclSceneNode = nullptr;
						}

						Data(const Data &) = delete;
						Data &operator=(const Data &) = delete;

						~Data();

						Data &operator=(Data &&rhs)
						{
							std::swap(rhs.m_pclSceneNode, m_pclSceneNode);

							return *this;
						}			

						const Ogre::Vector3 &GetWorldPosition() const;
						const Ogre::Vector3 &GetWorldScale() const;
						const Ogre::Quaternion &GetWorldOrientation() const;

						void SetPosition(const Ogre::Vector3 &position);
						void SetOrientation(const Ogre::Quaternion &orientation);
					};		
					
					class ComponentEnumerator
					{
						public:							
							ComponentEnumerator(const ComponentEnumerator &rhs);

							ComponentEnumerator &&operator=(ComponentEnumerator &&rhs) = delete;
							ComponentEnumerator &operator=(const ComponentEnumerator &rhs) = delete;

						private:
							ComponentEnumerator(MapObject &object, const char *type);

							friend class MapObject;

						public:
							bool Next();

							MapObjectComponent *GetCurrent();

						private:
							const char	*m_pszType;
							MapObject	&m_rclMapObject;

							ComponentsVector_t::iterator m_itCurrent;
							ComponentsVector_t::iterator m_itPosition;
							ComponentsVector_t::iterator m_itEnd;
					};

			public:
				inline MapObject(const String_t &name, Data &&data, const Register::Table &table) :
					Node(name, NodeFlags::PRIVATE_CHILDREN),
					m_clData(std::move(data)),
					m_rclTable(table)
				{
					//empty
				}

#if 0
				inline MapObject(MapObject &&data) :
					m_clData(std::move(data.m_clData)),
					m_vecComponents(std::move(data.m_vecComponents)),
					m_rclTable(data.m_rclTable)
				{
					//empty
				}

				inline MapObject(const String_t &name, DataSink &sink, const Register::Table &table) :
					Node(name, NodeFlags::PRIVATE_CHILDREN),
					m_clData(std::move(sink.m_clData)),
					m_rclTable(table)
				{

				}

#endif

				~MapObject();

				MapObject() = delete;		
				MapObject(const MapObject &) = delete;
				MapObject(const MapObject &&) = delete;
				MapObject &operator=(MapObject &) = delete;
				MapObject &operator=(MapObject &&) = delete;

#if 0
				inline MapObject &operator=(MapObject &&rhs)
				{
					std::swap(rhs.m_clData, m_clData);

					return *this;
				}
#endif

				inline const Ogre::Vector3 &GetWorldPosition() const
				{
					return m_clData.GetWorldPosition();
				}

				inline const Ogre::Vector3 &GetWorldScale() const
				{
					return m_clData.GetWorldScale();
				}

				inline const Ogre::Quaternion &GetWorldOrientation() const
				{
					return m_clData.GetWorldOrientation();
				}

				inline Engine::Math::Transform MakeWorldTransform() const
				{
					return Engine::Math::Transform(
						this->GetWorldPosition(),
						this->GetWorldOrientation()
					);
				}

#if 0
				inline PhysicsTypes GetPhysicsType() const
				{
					return m_clData.m_ePhysicsType;
				}

				void SyncSceneToPhysics();
				void SyncPhysicsToScene();

				inline void RegisterBody()
				{
					m_clData.RegisterBody();
				}
#endif

				void AddComponent(const String_t &typeName);
				void AddComponent(MapObjectComponent::UniquePtr_t &&component);

				inline ComponentEnumerator MakeEnumerator(const char *type)
				{
					return ComponentEnumerator(*this, type);
				}

				void *operator new(size_t sz);
				void operator delete(void *ptr);

#if 0
				void AttachObjectToBone(
					const Char_t *boneName,
					Ogre::MovableObject &movable,
					const Ogre::Quaternion &offsetOrientation = Ogre::Quaternion::IDENTITY,
					const Ogre::Vector3 &offsetPosition = Ogre::Vector3::ZERO
					);

				void AttachObjectToBone(
					const String_t &boneName,
					Ogre::MovableObject &movable,
					const Ogre::Quaternion &offsetOrientation = Ogre::Quaternion::IDENTITY,
					const Ogre::Vector3 &offsetPosition = Ogre::Vector3::ZERO
					);

				Ogre::Bone &GetBone(const char *boneName);
				Ogre::Bone &GetBone(const String_t &boneName);
#endif
				friend class MapObjectComponentAccess;
				
			private:
				void AttachOgreObject(Ogre::MovableObject &object);

				void SetTransform(const Engine::Math::Transform &transform);

			private:				
				Data					m_clData;
				ComponentsVector_t		m_vecComponents;

				const Register::Table	&m_rclTable;
		};		
	}
}

#endif