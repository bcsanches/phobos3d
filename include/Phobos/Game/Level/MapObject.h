#ifndef PH_GAME_MAP_OBJECT_H
#define PH_GAME_MAP_OBJECT_H

#include <algorithm>

#include <OgrePrerequisites.h>

#include <Phobos/Exception.h>
#include <Phobos/Memory.h>
#include <Phobos/Node.h>
#include <Phobos/OgreEngine/Math/Transform.h>
#include <Phobos/OgreEngine/Utils.h>
#include <Phobos/Register/TableFwd.h>
#include <Phobos/String.h>

#include "Phobos/Game/GameAPI.h"
#include "Phobos/Game/Level/MapDefs.h"
#include "Phobos/Game/Level/MapObjectComponent.h"

namespace Phobos
{
	namespace Game
	{		
		class PH_GAME_API MapObject : public Node
		{
			private:
				typedef std::vector<MapObjectComponent::UniquePtr_t> ComponentsVector_t;

			public:														
				class PH_GAME_API ComponentEnumerator
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
				inline MapObject(const String_t &name, OgreEngine::SceneNodeUniquePtr_t &&sceneNode, const Register::Table &table) :
					Node(name, NodeFlags::PRIVATE_CHILDREN),
					m_upSceneNode(std::move(sceneNode)),
					m_rclTable(table)
				{
					this->LoadComponents();
				}

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

				const Ogre::Vector3 &GetWorldPosition() const;

				const Ogre::Vector3 &GetWorldScale() const;

				const Ogre::Quaternion &GetWorldOrientation() const;

				inline Engine::Math::Transform MakeWorldTransform() const
				{
					return Engine::Math::Transform(
						this->GetWorldPosition(),
						this->GetWorldOrientation()
					);
				}

				void SetTransform(const Engine::Math::Transform &transform);

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
				
				template <typename T>
				T &GetComponent();				

				PH_DECLARE_MEMORY_OPERATORS;

				friend class MapObjectComponentAccess;
				
			private:
				void AttachOgreObject(Ogre::MovableObject &object);		

				void LoadComponents();

			private:				
				ComponentsVector_t					m_vecComponents;
				OgreEngine::SceneNodeUniquePtr_t	m_upSceneNode;				

				const Register::Table	&m_rclTable;
		};		

		template <typename T>
		T &MapObject::GetComponent()
		{			
			auto enumerator = this->MakeEnumerator(T::GetComponentName().c_str());

			if (!enumerator.Next())
			{
				std::stringstream stream;

				stream << "Object type " << T::GetComponentName() << "not found on " << this->GetName() << " components";
				PH_RAISE(Phobos::OBJECT_NOT_FOUND_EXCEPTION, "MapObject::GetComponent<T>", stream.str());
			}

			return static_cast<T &>(*enumerator.GetCurrent());
		}
	}
}

#endif