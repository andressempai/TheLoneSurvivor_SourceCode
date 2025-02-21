#ifndef SUBSCRIBE_TO_EXPLOSIVE_DAMAGE_EVENT_H
#define SUBSCRIBE_TO_EXPLOSIVE_DAMAGE_EVENT_H

template <typename TData, typename... TChildren>
class TBody;

struct PlayerSpineBodyData;

using EntityId = unsigned int;

namespace GameEvents::Body {
	struct SubscribeToExplosiveDamageEvent {
		void subcribe_to_event(EntityId entity_id);
		void unscribe_to_event(EntityId entity_id);
		void set_body(TBody<PlayerSpineBodyData>* body);

		TBody<PlayerSpineBodyData>* body_;
	};
}	// namespace GameEvents::Body

#endif // !SUBSCRIBE_TO_EXPLOSIVE_DAMAGE_EVENT_H