#ifndef PH_GAME_MAP_OBJECT_H
#define PH_GAME_MAP_OBJECT_H

#include <algorithm>

#include <OgrePrerequisites.h>

#include "Phobos/Game/Physics/RigidBody.h"
#include "Phobos/Game/MapDefs.h"

namespace Phobos
{
	namespace Game
	{
		class MapObject
		{
			public:
				class Data
				{
					friend MapObject;

					private:
						Ogre::SceneNode *m_pclSceneNode;
						Ogre::Entity	*m_pclEntity;
						Ogre::Light		*m_pclLight;

						Physics::RigidBody	m_clRigidBody;		

						PhysicsTypes		m_ePhysicsType;

					public:
						Data(Ogre::SceneNode *sceneNode, Ogre::Entity *entity, Ogre::Light *light) :
							m_pclSceneNode(sceneNode),
							m_pclEntity(entity),
							m_pclLight(light),
							m_ePhysicsType(PhysicsTypes::NONE)
						{
							//empty
						}

						Data(Data &&rhs) :
							m_pclSceneNode(std::move(rhs.m_pclSceneNode)),
							m_pclEntity(std::move(rhs.m_pclEntity)),
							m_pclLight(std::move(rhs.m_pclLight)),
							m_clRigidBody(std::move(rhs.m_clRigidBody)),
							m_ePhysicsType(rhs.m_ePhysicsType)
						{
							rhs.m_pclSceneNode = nullptr;
							rhs.m_pclLight = nullptr;
							rhs.m_pclEntity = nullptr;
						}

						Data(const Data &) = delete;
						Data &operator=(const Data &) = delete;

						~Data();

						Data &operator=(Data &&rhs)
						{
							std::swap(rhs.m_pclSceneNode, m_pclSceneNode);
							std::swap(rhs.m_pclEntity, m_pclEntity);
							std::swap(rhs.m_pclLight, m_pclLight);
							std::swap(rhs.m_clRigidBody, m_clRigidBody);

							m_ePhysicsType = rhs.m_ePhysicsType;

							return *this;
						}

						void AttachLight(Ogre::Light *light);

						void AttachEntity(Ogre::Entity *entity);

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

						inline bool HasEntity() const
						{
							return m_pclEntity ? true : false;
						}

						inline const Ogre::Entity *GetEntity() const
						{
							return m_pclEntity;
						}

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

			public:
				inline MapObject(Data &&data) :
					m_clData(std::move(data))
				{
					//empty
				}

				inline MapObject(MapObject &&data) :
					m_clData(std::move(data.m_clData))
				{
					//empty
				}

				inline MapObject(DataSink &sink) :
					m_clData(std::move(sink.m_clData))
				{

				}

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

			private:
				Data m_clData;
		};
		
	}
}

#endif