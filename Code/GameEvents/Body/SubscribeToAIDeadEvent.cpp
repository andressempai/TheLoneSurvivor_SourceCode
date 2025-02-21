#include "GameEvents/Body/SubscribeToAIDeadEvent.h"
#include "EventSystem.h"
#include "DevelopmentTools.h"

namespace GameEvents::Body {
	void SubscribeToAIDeadEvent::subcribe_to_event(EntityId entity_id)
	{
		BoomerShooterEventSystem::instance().subscribe<DeadEvent>(entity_id, [this](const EventBase& event)
		{
			const auto& dead_event = static_cast<const DeadEvent&>(event);
			auto& dead_entity = dead_event.dead_entity_.get();
			if (const auto parent_entity = DevelopmentTools::get_parent(dead_entity)) {
				dead_entity.DetachThis(IEntity::EAttachmentFlags::ATTACHMENT_KEEP_TRANSFORMATION);
				DevelopmentTools::destroy_entity_by_id(parent_entity->GetId());
			}
	
			if (not TimerManager::instance().is_timer_active(dead_timer_id_))
				TimerManager::instance().make_timer(dead_timer_id_, 10.0f, false, [id = dead_entity.GetId()]()
				{
					DevelopmentTools::destroy_entity_by_id(id);
				});
	
			if (not DevelopmentTools::has_physic_type(*dead_entity.GetPhysicalEntity(), PE_ARTICULATED)) {
				DevelopmentTools::destroy_physics(dead_entity, 0);
	
				SEntityPhysicalizeParams params{};
				params.type = PE_ARTICULATED;
				params.nSlot = 0;
				params.nLod = 1;
				params.fStiffnessScale = 1.0f;
				params.mass = 71.0f;
				params.nFlagsAND = aef_recorded_physics;
				DevelopmentTools::create_physics(dead_entity, params);
	
				auto collision_class_params = DevelopmentTools::get_physics_params<pe_params_collision_class>(dead_entity);
				collision_class_params.collisionClassOR.ignore = collision_class_living;
				DevelopmentTools::update_physics_params(dead_entity, collision_class_params);
			}
	
			DevelopmentTools::impulse_entity_by_physics(
					dead_entity
				,	dead_event.hit_point_
				,	dead_event.hit_direction_.normalized() * 250.0f
				,	dead_event.part_id_
			);
		});
	}

	void SubscribeToAIDeadEvent::unscribe_to_event(EntityId entity_id)
	{
		BoomerShooterEventSystem::instance().unsubscribe<DeadEvent>(entity_id);
	}
}	// namespace GameEvents::Body