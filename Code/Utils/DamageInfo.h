#ifndef DAMAGE_INFORMATION_H
#define DAMAGE_INFORMATION_H

#include <CryEntitySystem/IEntityBasicTypes.h>

#include <vector>
#include <functional>

struct DamageInformation {
	DamageInformation(
			IEntity& afected_entity
		,	const Vec3& impact_point
		,	const Vec3& impact_direction
		,	int part_id
		,	int32 damage
		,	int32 max_element
	)
		:	afected_entity_{ afected_entity }
		,	impact_direction_{ impact_direction }
		,	damage_{ damage }
	{
		impacts_points_.reserve(max_element);
		part_ids_.reserve(max_element);

		impacts_points_.push_back(impact_point);
		part_ids_.push_back(part_id);
	}

	std::vector<Vec3> impacts_points_{};
	std::vector<int> part_ids_{};
	Vec3 impact_direction_{ ZERO };
	std::reference_wrapper<IEntity> afected_entity_;
	int32 damage_{};
};

#endif // !DAMAGE_INFORMATION_H