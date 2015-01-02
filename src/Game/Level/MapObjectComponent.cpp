#include "Phobos/Game/Level/MapObjectComponent.h"

#include "Phobos/Game/Level/MapObjectComponentAccess.h"

#include <Phobos/Engine/Clocks.h>

namespace
{
	class Reminder
	{
		public:
			typedef void (Phobos::Game::MapObjectComponent::*ProcType_t)();

			Reminder(Phobos::Game::MapObjectComponent &target, ProcType_t proc);

			inline const Phobos::Game::MapObjectComponent &GetTarget() const;

			inline void Fire();

		private:
			Phobos::Game::MapObjectComponent &m_rclTarget;

			ProcType_t m_pfnProc;
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
		(m_rclTarget.*m_pfnProc)();		
	}

	typedef std::multimap<Phobos::Engine::GameClock::time_point, Reminder> ReminderMultiMap_t;

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

namespace
{
	class ProcManager
	{
		private:
			typedef std::vector<std::pair<Phobos::Game::MapObjectComponent *, Phobos::Game::MapObjectComponent::Proc_t>> ProcVector_t;

			ProcVector_t m_vecProcs;

		public:
			void AddProc(Phobos::Game::MapObjectComponent &comp, Phobos::Game::MapObjectComponent::Proc_t proc);
			void RemoveProc(Phobos::Game::MapObjectComponent &comp, Phobos::Game::MapObjectComponent::Proc_t proc);
			void RemoveAllReferences(Phobos::Game::MapObjectComponent &comp);

			void InvokeAll();
	};	

	void ProcManager::AddProc(Phobos::Game::MapObjectComponent &comp, Phobos::Game::MapObjectComponent::Proc_t proc)
	{
		m_vecProcs.emplace_back(&comp, proc);
	}

	void ProcManager::RemoveProc(Phobos::Game::MapObjectComponent &comp, Phobos::Game::MapObjectComponent::Proc_t proc)
	{
		auto element = std::make_pair(&comp, proc);

		auto it = std::find(m_vecProcs.begin(), m_vecProcs.end(), element);

		PH_ASSERT(it != m_vecProcs.end());

		m_vecProcs.erase(it);
	}

	void ProcManager::RemoveAllReferences(Phobos::Game::MapObjectComponent &comp)
	{
		m_vecProcs.erase(std::remove_if(m_vecProcs.begin(), m_vecProcs.end(), [&comp](const ProcVector_t::value_type &element){ return element.first == &comp; }), m_vecProcs.end());
	}

	void ProcManager::InvokeAll()
	{
		for (auto &pair : m_vecProcs)
		{
			(pair.first->*(pair.second))();
		}
	}

	static ProcManager g_clFixedUpdate;
	static ProcManager g_clUpdate;

	static void CancelUpdates(Phobos::Game::MapObjectComponent &target)
	{
		g_clFixedUpdate.RemoveAllReferences(target);
		g_clUpdate.RemoveAllReferences(target);
	}
}

namespace Phobos
{
	namespace Game
	{ 
		MapObjectComponent::~MapObjectComponent()
		{
			CancelReminders(*this);
			CancelUpdates(*this);
		}

		MapObjectComponentAccess MapObjectComponent::AccessMapObject()
		{
			return MapObjectComponentAccess(m_rclOwner);
		}

		void MapObjectComponent::LoadFinished(const Register::Table &table)
		{			
			this->OnLoadFinished(table);
		}

		void MapObjectComponent::AddReminderImpl(ReminderProcType_t proc, Phobos::Chrono::Seconds time)
		{
			g_mmapReminders.insert(std::make_pair(time + Engine::GameClock::Now(), Reminder(*this, proc)));
		}

		void MapObjectComponent::TickReminders()
		{
			auto now = Engine::GameClock::Now();			

			while (!g_mmapReminders.empty())
			{
				auto it = g_mmapReminders.begin();

				if (it->first >= now)
					break;

				Reminder event(it->second);
				g_mmapReminders.erase(it);

				event.Fire();
			}
		}

		void MapObjectComponent::EnableFixedUpdateImpl(Proc_t proc)
		{
			g_clFixedUpdate.AddProc(*this, proc);
		}

		void MapObjectComponent::DisableFixedUpdateImpl(Proc_t proc)
		{
			g_clFixedUpdate.RemoveProc(*this, proc);
		}

		void MapObjectComponent::EnableUpdateImpl(Proc_t proc)
		{
			g_clUpdate.AddProc(*this, proc);
		}

		void MapObjectComponent::DisableUpdateImpl(Proc_t proc)
		{
			g_clUpdate.RemoveProc(*this, proc);
		}

		void MapObjectComponent::TickFixedUpdate()
		{
			g_clFixedUpdate.InvokeAll();
		}

		void MapObjectComponent::TickUpdate()
		{
			g_clUpdate.InvokeAll();
		}
	}
}
