#ifndef PH_GAME_MAP_OBJECT_H
#define PH_GAME_MAP_OBJECT_H

#include <algorithm>

#include <OgrePrerequisites.h>

#include <Phobos/Register/TableFwd.h>
#include <Phobos/String.h>

#include "Phobos/Game/Physics/RigidBody.h"
#include "Phobos/Game/Level/MapDefs.h"
#include "Phobos/Game/Level/MapObjectComponent.h"

namespace Phobos
{
	namespace Game
	{		
		class MapObject
		{
			private:
				typedef std::vector<MapObjectComponent::UniquePtr_t> ComponentsVector_t;

			public:
				class Data
				{
					friend MapObject;

					private:
						Ogre::SceneNode		*m_pclSceneNode;

						Physics::RigidBody	m_clRigidBody;		

						PhysicsTypes		m_ePhysicsType;

					public:
						Data(Ogre::SceneNode *sceneNode) :
							m_pclSceneNode(sceneNode),
							m_ePhysicsType(PhysicsTypes::NONE)
						{
							//empty
						}

						Data(Data &&rhs) :
							m_pclSceneNode(std::move(rhs.m_pclSceneNode)),
							m_clRigidBody(std::move(rhs.m_clRigidBody)),
							m_ePhysicsType(rhs.m_ePhysicsType)
						{
							rhs.m_pclSceneNode = nullptr;
						}

						Data(const Data &) = delete;
						Data &operator=(const Data &) = delete;

						~Data();

						Data &operator=(Data &&rhs)
						{
							std::swap(rhs.m_pclSceneNode, m_pclSceneNode);
							std::swap(rhs.m_clRigidBody, m_clRigidBody);

							m_ePhysicsType = rhs.m_ePhysicsType;

							return *this;
						}

						inline void SetRigidBody(Physics::RigidBody &&body, PhysicsTypes type)
						{
							m_clRigidBody = std::move(body);
							m_ePhysicsType = type;
						}						

						const Ogre::Vector3 &GetWorldPosition() const;
						const Ogre::Vector3 &GetWorldScale() const;
						const Ogre::Quaternion &GetWorldOrientation() const;

						void SetPosition(const Ogre::Vector3 &position);
						void SetOrientation(const Ogre::Quaternion &orientation);						

						void RegisterBody();
					};		

					class DataSink
					{
						friend class MapObject;

						public:
							DataSink(Data &&data):
								m_clData(std::move(data))
							{
								//empty
							}

						private:
							Data m_clData;
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
				inline MapObject(Data &&data, const Register::Table &table) :
					m_clData(std::move(data)),
					m_rclTable(table)
				{
					//empty
				}

				inline MapObject(MapObject &&data) :
					m_clData(std::move(data.m_clData)),
					m_vecComponents(std::move(data.m_vecComponents)),
					m_rclTable(data.m_rclTable)
				{
					//empty
				}

				inline MapObject(DataSink &sink, const Register::Table &table) :
					m_clData(std::move(sink.m_clData)),
					m_rclTable(table)
				{

				}

				~MapObject();

				MapObject() = delete;		
				MapObject(const MapObject &) = delete;
				MapObject &operator=(MapObject &) = delete;

				inline MapObject &operator=(MapObject &&rhs)
				{
					std::swap(rhs.m_clData, m_clData);

					return *this;
				}

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

				void AddComponent(const String_t &typeName);
				void AddComponent(MapObjectComponent::UniquePtr_t &&component);

				inline ComponentEnumerator MakeEnumerator(const char *type)
				{
					return ComponentEnumerator(*this, type);
				}

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

			private:				
				Data					m_clData;
				ComponentsVector_t		m_vecComponents;

				const Register::Table	&m_rclTable;
		};		
	}
}

#endif