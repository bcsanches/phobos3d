#include "Phobos/Game/Level/MapObjectComponent.h"

#include "Phobos/Game/Level/MapObjectComponentAccess.h"

#include <Phobos/Engine/Core.h>

namespace
{
	class Reminder
	{
		public:
			typedef std::function<void(Phobos::Game::MapObjectComponent*)> ProcType_t;

			Reminder(Phobos::Game::MapObjectComponent &target, ProcType_t proc);

			inline const Phobos::Game::MapObjectComponent &GetTarget() const;

			inline void Fire();

		private:
			Phobos::Game::MapObjectComponent &m_rclTarget;

			std::function<void(Phobos::Game::MapObjectComponent*)> m_pfnProc;
	};

	Reminder::Reminder(Phobos::Game::MapObjectComponent &target, ProcType_t proc):
		m_rclTarget(target),
		m_pfnProc(proc)
	{
		//empty
	}

	inline const Phobos::Game::MapObjectComponent &Reminder::GetTarget() const
	{
		return m_rclTarget;
	}

	inline void Reminder::Fire()
	{
		m_pfnProc(&m_rclTarget);
	}

	typedef std::multimap<Phobos::Float_t, Reminder> ReminderMultiMap_t;

	static ReminderMultiMap_t g_mmapReminders;

	void CancelReminders(Phobos::Game::MapObjectComponent &target)
	{
		for (auto it = g_mmapReminders.begin(); it != g_mmapReminders.end();)
		{
			auto eraseIt = it++;

			if (&eraseIt->second.GetTarget() == &target)
			{
				g_mmapReminders.erase(eraseIt);
			}
		}
	}
}

namespace Phobos
{
	namespace Game
	{ 
		MapObjectComponent::~MapObjectComponent()
		{
			CancelReminders(*this);
		}

		MapObjectComponentAccess MapObjectComponent::AccessMapObject()
		{
			return MapObjectComponentAccess(m_rclOwner);
		}

		void MapObjectComponent::LoadFinished(const Register::Table &table)
		{			
			this->OnLoadFinished(table);
		}

		void MapObjectComponent::AddReminder(ReminderProcType_t proc, Float_t time)
		{
			g_mmapReminders.insert(std::make_pair(time + Engine::Core::GetInstance().GetGameTimer().m_fpTotalTicks, Reminder(*this, proc)));
		}

		void MapObjectComponent::TickReminders()
		{
			auto ticks = Engine::Core::GetInstance().GetGameTimer().m_fpTotalTicks;

			while (!g_mmapReminders.empty())
			{
				auto it = g_mmapReminders.begin();

				if (it->first > ticks)
					break;

				Reminder event(it->second);
				g_mmapReminders.erase(it);

				event.Fire();
			}
		}
	}
}
