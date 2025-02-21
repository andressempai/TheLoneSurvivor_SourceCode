#include "EntityEventListeners/EntityAnimationEventListener.h"
#include "DevelopmentTools.h"

namespace EntityEventListeners {
	void EntityAnimationEventListener::OnEntityEvent(IEntity* pEntity, const SEntityEvent& event)
	{
		if (const auto entity = DevelopmentTools::search_entity_by_id(entity_id_))
			entity->SendEvent(event);
	}

	void EntityAnimationEventListener::set_entity_id(EntityId id)
	{
		entity_id_ = id;
	}
}	// namespace EntityEventListeners