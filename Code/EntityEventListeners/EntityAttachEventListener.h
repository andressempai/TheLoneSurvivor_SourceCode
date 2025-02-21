#ifndef ENTITY_ATTACH_EVENT_LISTERNER_H
#define ENTITY_ATTACH_EVENT_LISTERNER_H

#include <CryEntitySystem/IEntity.h>

namespace EntityEventListeners {
	struct EntityAttachEventListener : IEntityEventListener {
		void OnEntityEvent(IEntity* pEntity, const SEntityEvent& event) override;

		static constexpr auto event_to_listen = Cry::Entity::EEvent::AttachedToParent;
	};
}	// namespace EntityEventListeners

#endif // !ENTITY_ATTACH_EVENT_LISTERNER_H