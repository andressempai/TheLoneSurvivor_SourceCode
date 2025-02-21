#ifndef EVENT_SYSTEM_H
#define EVENT_SYSTEM_H

#include "Utils/HealthPack.h"
#include "Utils/Armor.h"
#include "Utils/ShakeParams.h"
#include "MetaProgramming.h"

#include <CryEntitySystem/IEntityBasicTypes.h>

#include <array>
#include <vector>
#include <unordered_map>
#include <functional>
#include <string_view>

struct EventBase {};

struct CollectEvent : EventBase {
	CollectEvent(IEntity& collector) : collector_{ collector } {}

	std::reference_wrapper<IEntity> collector_;
};

struct PointDamageEvent : EventBase {
	PointDamageEvent(
			const std::vector<Vec3>& impact_points
		,	const std::vector<int>& part_ids
		,	Vec3 impact_direction
		,	IEntity& damaged_entity
		,	int32 damage
	)	:	impacts_points_{ impact_points }
		,	part_ids_{ part_ids }
		,	impact_direction_{ impact_direction }
		,	damaged_entity_{ damaged_entity }
		,	damage_{ damage }
	{}

	std::vector<Vec3> impacts_points_;
	std::vector<int> part_ids_;
	Vec3 impact_direction_;
	std::reference_wrapper<IEntity> damaged_entity_;
	int32 damage_;
};

struct ExplosiveDamageEvent final : EventBase {
	ExplosiveDamageEvent(Vec3 origin, Vec3 direction, IEntity& damaged_entity, int32 damage)
		:	origin_{ origin }
		,	direction_{ direction }
		,	damaged_entity_{ damaged_entity }
		,	damage_{ damage }
	{}

	Vec3 origin_;
	Vec3 direction_;
	std::reference_wrapper<IEntity> damaged_entity_;
	int32 damage_;
};

struct HealEvent : EventBase {
	HealEvent(IEntity& healed_entity, HealthPack health_pack)
		:	healed_entity_{ healed_entity }
		,	health_pack_{ health_pack }
	{}

	std::reference_wrapper<IEntity> healed_entity_;
	HealthPack health_pack_;
};

struct RepairArmorEvent : EventBase {
	RepairArmorEvent(IEntity& armor_entity, Armor armor)
		:	armor_entity_{ armor_entity }
		,	armor_{ armor }
	{}

	std::reference_wrapper<IEntity> armor_entity_;
	Armor armor_;
};

struct DamageReactionEvent final : EventBase {};

struct DeadEvent final : EventBase {
	DeadEvent(Vec3 hit_point, Vec3 hit_direction, IEntity& dead_entity, int part_id)
		:	hit_point_{ hit_point }
		,	hit_direction_{ hit_direction }
		,	dead_entity_{ dead_entity }
		,	part_id_{ part_id }
	{}

	Vec3 hit_point_;
	Vec3 hit_direction_;
	std::reference_wrapper<IEntity> dead_entity_;
	int part_id_;
};

struct BeforeSwitchWeaponEvent : EventBase{};

struct SwitchWeaponEvent : EventBase {
	explicit SwitchWeaponEvent(std::string_view animation_tag_name) : animation_tag_name_{ animation_tag_name } {}

	std::string_view animation_tag_name_;
};

struct FinishSwitchWeaponEvent : EventBase {};

struct EquipWeaponEvent : EventBase {
	explicit EquipWeaponEvent(std::string_view animation_tag_name) : animation_tag_name_{ animation_tag_name } {}

	std::string_view animation_tag_name_;
};

struct PullTriggerEvent : EventBase {
	explicit PullTriggerEvent(IEntity& gunner) : gunner_{ gunner } {}

	std::reference_wrapper<IEntity> gunner_;
};

struct ReleaseTriggerEvent : EventBase {};

struct RecoilEvent : EventBase {
	explicit RecoilEvent(const ShakeParams& shake_params = ShakeParams{}) : shake_params_{ shake_params } {}

	ShakeParams shake_params_;
};

struct ReloadEvent : EventBase {};
struct EndReloadEvent : EventBase {};
struct CancelReloadEvent : EventBase {};
struct MelleeAttackEvent : EventBase {};
struct AlertEvent : EventBase {};
struct ApplyRootMotionEvent : EventBase {};
struct DontApplyRootMotionEvent : EventBase {};

struct MagazineUpdateEvent : EventBase {
	MagazineUpdateEvent(IEntity& weapon, int32 size, int32 capacity)
		:	weapon_{ weapon }
		,	size_{ size }
		,	capacity_{ capacity }
	{}

	std::reference_wrapper<IEntity> weapon_;
	int32 size_;
	int32 capacity_;
};

struct HealthUpdateEvent : EventBase {
	explicit HealthUpdateEvent(int32 health) : health_{ health } {}

	int32 health_;
};

struct ArmorUpdateEvent : EventBase {
	explicit ArmorUpdateEvent(int32 hardness) : hardness_{ hardness } {}

	int32 hardness_;
};

template <typename... Event>
class EventSystem {
	static_assert((std::is_base_of_v<EventBase, Event> && ...), "The Event must be base of Event Base");

	using TypeList = MP::type_list<Event...>;
	using KeyType = EntityId;
	using CallableType = std::function<void(const EventBase&)>;

public:
	static EventSystem& instance()
	{
		static EventSystem event_system{};
		return event_system;
	}

	template <typename EventType>
	void subscribe(KeyType key, CallableType callback)
	{
		auto& map = get_map<EventType>();
		map.emplace(key, callback);
	}

	template <typename EventType>
	void unsubscribe(KeyType key)
	{
		auto& map = get_map<EventType>();
		if (auto iter = map.find(key); iter != map.end())
			map.erase(iter);
	}

	template <typename EventType>
	void send(KeyType key, const EventType& event)
	{
		auto& map = get_map<EventType>();
		for (auto [begin, end] = map.equal_range(key); begin != end; ++begin)
			std::invoke(begin->second, event);
	}

private:
	EventSystem() = default;
	EventSystem(const EventSystem&) = default;
	EventSystem(EventSystem&&) = default;

	EventSystem& operator=(const EventSystem&) = default;
	EventSystem& operator=(EventSystem&&) = default;

	~EventSystem() = default;

	template <typename EventType>
	auto& get_map()
	{
		static_assert(MP::has_type_v<EventType, TypeList>, "The type must be one of those listed in the parameter pack.");
		return events_[MP::get_pos_v<EventType, TypeList>];
	}

	std::array<std::unordered_multimap<KeyType, CallableType>, MP::size_v<MP::type_list<Event...>>> events_{};
};

using BoomerShooterEventSystem = EventSystem<
		CollectEvent
	,	PointDamageEvent
	,	ExplosiveDamageEvent
	,	HealEvent
	,	RepairArmorEvent
	,	DamageReactionEvent
	,	DeadEvent
	,	BeforeSwitchWeaponEvent
	,	SwitchWeaponEvent
	,	FinishSwitchWeaponEvent
	,	EquipWeaponEvent
	,	PullTriggerEvent
	,	ReleaseTriggerEvent
	,	RecoilEvent
	,	ReloadEvent
	,	EndReloadEvent
	,	CancelReloadEvent
	,	MelleeAttackEvent
	,	AlertEvent
	,	MagazineUpdateEvent
	,	HealthUpdateEvent
	,	ArmorUpdateEvent
	,	ApplyRootMotionEvent
	,	DontApplyRootMotionEvent
>;

#endif // !EVENT_SYSTEM_H