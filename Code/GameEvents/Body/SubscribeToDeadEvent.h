#ifndef SUBSCRIBE_TO_DEAD_EVENT_H
#define SUBSCRIBE_TO_DEAD_EVENT_H

#include "TimerManager.h"

using EntityId = unsigned int;

namespace GameEvents::Body {
	struct SubscribeToDeadEvent {
		void subcribe_to_event(EntityId entity_id);
		void unscribe_to_event(EntityId entity_id);

		TimerId dead_timer_id_{};
	};
}	// namespace GameEvents::Body

#endif // !SUBSCRIBE_TO_DEAD_EVENT_H