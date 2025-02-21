#ifndef EXPLOSIVE_H
#define EXPLOSIVE_H

#include "../EventSystem.h"
#include "../DevelopmentTools.h"

#include <utility>

struct Explosive {
	void make_explosion(IEntity& explosive_entity, f32 damage_to_make)
	{
		auto explosion_params = pe_explosion{};
		explosion_params.epicenter = explosion_params.epicenterImp = explosive_entity.GetWorldPos();
		explosion_params.rmin = 0.001f;
		explosion_params.rmax = 6.0f;
		explosion_params.r = 5.0f;
		explosion_params.impulsivePressureAtR = 2.5f;
	
		auto ignored_entities = std::vector<IPhysicalEntity*>{};
		const auto entities_flags = ent_living | ent_rigid | ent_sleeping_rigid;
		const auto entities = DevelopmentTools::simulate_explosion(explosion_params, ignored_entities, entities_flags);
		for (auto* const physical_entity : entities) {
			const auto pos_status = DevelopmentTools::get_physics_status<pe_status_pos>(physical_entity);
			const auto max_explosive_radius = (explosion_params.rmax * explosion_params.rmax) / 2.0f;
			const auto distance = pos_status.pos.GetSquaredDistance(explosive_entity.GetWorldPos()) / max_explosive_radius;
	
			const auto entity_info = [](IPhysicalEntity* physical_entity)
			{
				const auto entity = DevelopmentTools::search_entity_by_physics(physical_entity);
				if (DevelopmentTools::has_physic_type(*physical_entity, PE_LIVING))
					if (const auto child_entity = DevelopmentTools::get_child(*entity))
						return std::make_pair(child_entity, child_entity->GetId());

				return std::make_pair(entity, entity->GetId());
			}(physical_entity);
			
			const auto damage = crymath::ceil(crymath::clamp(damage_to_make * (1.0f - distance), 0.0f, damage_to_make));
			BoomerShooterEventSystem::instance().send(
					entity_info.second
				,	ExplosiveDamageEvent{
							(entity_info.first->GetWorldPos() - explosion_params.epicenter).normalized()
						,	ZERO
						,	*entity_info.first
						,	static_cast<int>(damage)
					}
			);
		}
	
	#ifndef RELEASE
		auto* const debugger = gEnv->pGameFramework->GetIPersistantDebug();
		debugger->Begin("explosiondebbuger", true);
		debugger->AddSphere(explosive_entity.GetWorldPos(), 6.0f, Col_Red, 10);
	#endif // !RELEASE
	
		DevelopmentTools::play_2d_sound("small_explosion");
	}
};

#endif // !EXPLOSIVE_H