#ifndef BODY_H
#define BODY_H

#include "Utils/HealthPack.h"
#include "Utils/Armor.h"
#include "EventSystem.h"

#include <CryAnimation/ICryMannequin.h>

#include <string_view>
#include <optional>
#include <tuple>
#include <type_traits>

using namespace std::string_view_literals;

template <typename TData, typename... Children>
class TBody {
public:
	using ArmorSlot = std::optional<Armor>;

	static constexpr auto children_count = sizeof...(Children);

	template <typename BaseBody>
	void impact_damage(
			IEntity& entity
		,	Vec3 impact_point
		,	Vec3 impact_direction
		,	int part_id
		,	int damage
		,	BaseBody& base_body
	)
	{
		auto* const character = entity.GetCharacter(0);
		string part_name = character->GetIDefaultSkeleton().GetJointNameByID(part_id);
		
		if (strcmp(name_.data(), part_name.c_str()) == 0) {
			auto damage_to_apply = [&]()
			{
				if (armor_)
					return armor_->harm(damage);

				return damage;
			}();
			
			health_pack_.harm(damage_to_apply);

			if constexpr (name_ != BaseBody::name_)
				base_body.health_pack_.harm(damage_to_apply);
			
			if ((health_pack_.is_empty() and vital_) or base_body.health_pack_.is_empty()) {
				BoomerShooterEventSystem::instance().send(
						entity.GetId()
					,	DeadEvent{ impact_point, impact_direction, entity, part_id }
				);
				return;
			}

			BoomerShooterEventSystem::instance().send(entity.GetId(), DamageReactionEvent{});

			if (armor_)
				if (armor_->is_broken())
					armor_.reset();

			return;
		}

		apply_point_damage_to_each_child(
				std::make_index_sequence<children_count>{}
			,	entity
			,	impact_point
			,	impact_direction
			,	part_id, damage
			,	base_body
		);
	}

	void explosion_damage(IEntity& entity, Vec3 origin, Vec3 direction, int damage)
	{
		auto damage_to_apply = [&]()
		{
			if (armor_)
				return armor_->harm(damage);

			return damage;
		}();

		health_pack_.harm(damage_to_apply);

		if (health_pack_.is_empty()) {
			BoomerShooterEventSystem::instance().send(entity.GetId(), DeadEvent{ origin, direction, entity , -1 });
			return;
		}

		if (armor_ and armor_->is_broken())
			armor_.reset();
	}

	bool heal(const HealthPack& health_pack)
	{
		if (not health_pack_.is_full()) {
			health_pack_.fill(health_pack);
			return true;
		}

		return false;
	}

	bool repair_armor(Armor armor)
	{
		if (armor_) {
			if (not armor_->is_in_good_condition()) {
				armor_->repair(armor);
				return true;
			}
		}
		else {
			armor_ = armor;
			return true;
		}

		return false;
	}

	template <std::size_t... Is, typename BaseBody>
	void apply_point_damage_to_each_child(
			std::index_sequence<Is...>
		,	IEntity& entity
		,	Vec3 impact_point
		,	Vec3 impact_direction
		,	int part_id
		,	int damage
		,	BaseBody& base_body
	)
	{
		if constexpr (not std::is_same_v<decltype(children_), void*>)
			(std::get<Is>(children_).template impact_damage(
					entity
				,	impact_point
				,	impact_direction
				,	part_id
				,	damage
				,	base_body
			), ...);
	}

	std::conditional_t<(children_count > 0), std::tuple<Children...>, void*> children_{};

	static constexpr std::string_view name_{ TData::name_ };
	ArmorSlot armor_{ TData::armor_ };
	HealthPack health_pack_{ TData::health_pack_ };
	bool vital_{ TData::is_vital_ };
};

constexpr auto without_armor = std::nullopt;

struct PlayerSpineBodyData {
	static constexpr auto name_ = "spine"sv;
	static constexpr auto armor_ = without_armor;
	static constexpr auto health_pack_ = extra_large_health_pack;
	static constexpr auto is_vital_ = true;
};

struct ZombieHeadBodyData {
	static constexpr auto name_ = "head"sv;
	static constexpr auto armor_ = without_armor;
	static constexpr auto health_pack_ = small_health_pack;
	static constexpr auto is_vital_ = true;
};

struct ZombieNeckBodyData {
	static constexpr auto name_ = "neck"sv;
	static constexpr auto armor_ = without_armor;
	static constexpr auto health_pack_ = small_health_pack;
	static constexpr auto is_vital_ = true;
};

struct ZombieTorsoBodyData {
	static constexpr auto name_ = "spine_02"sv;
	static constexpr auto armor_ = without_armor;
	static constexpr auto health_pack_ = large_health_pack;
	static constexpr auto is_vital_ = true;
};

struct ZombiePelvisBodyData {
	static constexpr auto name_ = "pelvis"sv;
	static constexpr auto armor_ = without_armor;
	static constexpr auto health_pack_ = large_health_pack;
	static constexpr auto is_vital_ = true;
};

struct ZombieLHipBodyData {
	static constexpr auto name_ = "l_hip"sv;
	static constexpr auto armor_ = without_armor;
	static constexpr auto health_pack_ = medium_health_pack;
	static constexpr auto is_vital_ = false;
};

struct ZombieLKneeBodyData {
	static constexpr auto name_ = "l_knee"sv;
	static constexpr auto armor_ = without_armor;
	static constexpr auto health_pack_ = medium_health_pack;
	static constexpr auto is_vital_ = false;
};

struct ZombieLAnkleBodyData {
	static constexpr auto name_ = "l_ankle"sv;
	static constexpr auto armor_ = without_armor;
	static constexpr auto health_pack_ = medium_health_pack;
	static constexpr auto is_vital_ = false;
};

struct ZombieRHipBodyData {
	static constexpr auto name_ = "r_hip"sv;
	static constexpr auto armor_ = without_armor;
	static constexpr auto health_pack_ = medium_health_pack;
	static constexpr auto is_vital_ = false;
};

struct ZombieRKneeBodyData {
	static constexpr auto name_ = "r_knee"sv;
	static constexpr auto armor_ = without_armor;
	static constexpr auto health_pack_ = medium_health_pack;
	static constexpr auto is_vital_ = false;
};

struct ZombieRAnkleBodyData {
	static constexpr auto name_ = "r_ankle"sv;
	static constexpr auto armor_ = without_armor;
	static constexpr auto health_pack_ = medium_health_pack;
	static constexpr auto is_vital_ = false;
};

struct ZombieLForeArmBodyData {
	static constexpr auto name_ = "l_forearm"sv;
	static constexpr auto armor_ = without_armor;
	static constexpr auto health_pack_ = medium_health_pack;
	static constexpr auto is_vital_ = false;
};

struct ZombieLArmBodyData {
	static constexpr auto name_ = "l_arm"sv;
	static constexpr auto armor_ = without_armor;
	static constexpr auto health_pack_ = medium_health_pack;
	static constexpr auto is_vital_ = false;
};

struct ZombieLWristBodyData {
	static constexpr auto name_ = "l_wrist"sv;
	static constexpr auto armor_ = without_armor;
	static constexpr auto health_pack_ = medium_health_pack;
	static constexpr auto is_vital_ = false;
};

struct ZombieRForeArmBodyData {
	static constexpr auto name_ = "r_forearm"sv;
	static constexpr auto armor_ = without_armor;
	static constexpr auto health_pack_ = medium_health_pack;
	static constexpr auto is_vital_ = false;
};

struct ZombieRArmBodyData {
	static constexpr auto name_ = "r_arm"sv;
	static constexpr auto armor_ = without_armor;
	static constexpr auto health_pack_ = medium_health_pack;
	static constexpr auto is_vital_ = false;
};

struct ZombieRWristBodyData {
	static constexpr auto name_ = "r_wrist"sv;
	static constexpr auto armor_ = without_armor;
	static constexpr auto health_pack_ = medium_health_pack;
	static constexpr auto is_vital_ = false;
};

struct PerderosLowerJawBodyData {
	static constexpr auto name_ = "lower_jaw_01"sv;
	static constexpr auto armor_ = without_armor;
	static constexpr auto health_pack_ = medium_health_pack;
	static constexpr auto is_vital_ = false;
};

struct PerderosLJawBodyData {
	static constexpr auto name_ = "l_jaw_01"sv;
	static constexpr auto armor_ = without_armor;
	static constexpr auto health_pack_ = medium_health_pack;
	static constexpr auto is_vital_ = false;
};

struct PerderosRJawBodyData {
	static constexpr auto name_ = "r_jaw_01"sv;
	static constexpr auto armor_ = without_armor;
	static constexpr auto health_pack_ = medium_health_pack;
	static constexpr auto is_vital_ = false;
};

struct PerderosTorsoBodyData {
	static constexpr auto name_ = "spine_02"sv;
	static constexpr auto armor_ = without_armor;
	static constexpr auto health_pack_ = ultra_large_health_pack;
	static constexpr auto is_vital_ = true;
};

struct PerderosPelvisBodyData {
	static constexpr auto name_ = "pelvis"sv;
	static constexpr auto armor_ = without_armor;
	static constexpr auto health_pack_ = ultra_large_health_pack;
	static constexpr auto is_vital_ = true;
};

using PlayerSpine = TBody<PlayerSpineBodyData>;

using ZombieHead = TBody<ZombieHeadBodyData>;
using ZombieNeck = TBody<ZombieNeckBodyData, ZombieHead>;
using ZombieLWrist = TBody<ZombieLWristBodyData>;
using ZombieLArm = TBody<ZombieLArmBodyData, ZombieLWrist>;
using ZombieLForeArm = TBody<ZombieLForeArmBodyData, ZombieLArm>;
using ZombieRWrist = TBody<ZombieRWristBodyData>;
using ZombieRArm = TBody<ZombieRArmBodyData, ZombieRWrist>;
using ZombieRForeArm = TBody<ZombieRForeArmBodyData, ZombieRArm>;
using ZombieTorso = TBody<ZombieTorsoBodyData, ZombieNeck, ZombieLForeArm, ZombieRForeArm>;
using ZombieLAnkle = TBody<ZombieLAnkleBodyData>;
using ZombieLKnee = TBody<ZombieLKneeBodyData, ZombieLAnkle>;
using ZombieLHip = TBody<ZombieLHipBodyData, ZombieLKnee>;
using ZombieRAnkle = TBody<ZombieRAnkleBodyData>;
using ZombieRKnee = TBody<ZombieRKneeBodyData, ZombieRAnkle>;
using ZombieRHip = TBody<ZombieRHipBodyData, ZombieRKnee>;
using ZombiePelvis = TBody<ZombiePelvisBodyData, ZombieTorso, ZombieLHip, ZombieRHip>;

using PerderosLowerJaw = TBody<PerderosLowerJawBodyData>;
using PerderosLJaw = TBody<PerderosLJawBodyData>;
using PerderosRJaw = TBody<PerderosRJawBodyData>;
using PerderosTorso = TBody<PerderosTorsoBodyData, PerderosLowerJaw, PerderosLJaw, PerderosRJaw, ZombieLForeArm, ZombieRForeArm>;
using PerderosPelvis = TBody<PerderosPelvisBodyData, PerderosTorso, ZombieLHip, ZombieRHip>;

#endif // !BODY_H