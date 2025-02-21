#ifndef ENTITY_ANIMATION_EVENT_LISTERNER_H
#define ENTITY_ANIMATION_EVENT_LISTERNER_H

#include <CryEntitySystem/IEntity.h>

namespace EntityEventListeners {
	struct EntityAnimationEventListener : IEntityEventListener {
		void OnEntityEvent(IEntity* pEntity, const SEntityEvent& event) override;
		void set_entity_id(EntityId id);

		static constexpr auto event_to_listen = Cry::Entity::EEvent::AnimationEvent;
		EntityId entity_id_{ INVALID_ENTITYID };
	};
}	// namespace EntityEventListeners

#endif // !ENTITY_ANIMATION_EVENT_LISTERNER_H