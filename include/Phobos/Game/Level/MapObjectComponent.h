#ifndef PH_GAME_MAP_OBJECT_COMPONENT_H
#define PH_GAME_MAP_OBJECT_COMPONENT_H

#include <Phobos/DisableCopy.h>

#include <Phobos/Error.h>
#include <Phobos/Register/TableFwd.h>
#include <Phobos/Types.h>

#include "Phobos/Game/GameAPI.h"

#include <functional>
#include <memory>

namespace Phobos
{
	namespace Game
	{
		class MapObject;
		class MapObjectComponentAccess;

		class PH_GAME_API MapObjectComponent
		{
			PH_DISABLE_COPY(MapObjectComponent);

			public:
				typedef void (MapObjectComponent::*Proc_t)();
				typedef std::unique_ptr<MapObjectComponent> UniquePtr_t;

				class LoadFinishedAccess
				{
					friend class MapObject;

					private:
						static void LoadFinished(MapObjectComponent &comp, const Register::Table &table)
						{
							comp.LoadFinished(table);
						}
				};

			public:
				static void TickReminders();
				static void TickFixedUpdate();
				static void TickUpdate();				

				inline MapObject &GetOwner() const
				{
					return m_rclOwner;
				}

				virtual ~MapObjectComponent();

			protected:
				MapObjectComponent(MapObject &owner) :
					m_rclOwner(owner)
				{
					//empty
				}				

				MapObjectComponentAccess AccessMapObject();

				typedef Proc_t ReminderProcType_t;
				void AddReminderImpl(ReminderProcType_t proc, Float_t delay);

				template <typename T>
				inline void AddReminder(void (T::*proc)(), Float_t time)
				{
					this->AddReminderImpl(static_cast<ReminderProcType_t>(proc), time);
				}

				virtual void OnLoadFinished(const Register::Table &table) {};

				template <typename T>
				inline T &GetOwnerComponent()
				{
					return m_rclOwner.GetComponent<T>();
				}

				template <typename T>
				inline void EnableFixedUpdate(void (T::*proc)())
				{
					this->EnableFixedUpdateImpl(static_cast<Proc_t>(proc));
				}

				template <typename T>
				inline void DisableFixedUpdate(void (T::*proc)())
				{
					this->DisableFixedUpdateImpl(static_cast<Proc_t>(proc));
				}

				template <typename T>
				inline void EnableUpdate(void (T::*proc)())
				{
					this->EnableUpdateImpl(static_cast<Proc_t>(proc));
				}

				template <typename T>
				inline void DisableUpdate(void (T::*proc)())
				{
					this->DisableUpdateImpl(static_cast<Proc_t>(proc));
				}				

			private:
				void LoadFinished(const Register::Table &table);

				void EnableFixedUpdateImpl(Proc_t proc);
				void DisableFixedUpdateImpl(Proc_t proc);

				void EnableUpdateImpl(Proc_t proc);
				void DisableUpdateImpl(Proc_t proc);

			private:
				MapObject	&m_rclOwner;
		};		
	}
}

#endif
