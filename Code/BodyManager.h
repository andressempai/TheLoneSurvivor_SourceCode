#ifndef BODY_MANAGER_H
#define BODY_MANAGER_H

#include "Utils/Body.h"
#include "MetaProgramming.h"

#include <variant>
#include <unordered_map>

class BodyManager {
public:
	using BodyList = MP::type_list<PlayerSpine, ZombiePelvis, PerderosPelvis>;
	using AnyBody = MP::transform_to_type_list_like_t<BodyList, std::variant>;

	static BodyManager& instance()
	{
		static BodyManager body_manager{};
		return body_manager;
	}

	template <typename BodyType>
	AnyBody& create_body(EntityId id)
	{
		static_assert(MP::has_type_v<BodyType, BodyList>, "BodyType is not in the BodyList");
		const auto [iter, _] = bodies_.emplace(id, BodyType{});
		return iter->second;
	}

	AnyBody& get_body(EntityId id)
	{
		return bodies_.at(id);
	}

	void remove_body(EntityId id)
	{
		bodies_.erase(id);
	}

private:
	BodyManager() = default;
	BodyManager(const BodyManager&) = delete;
	BodyManager(BodyManager&&) = delete;
	BodyManager& operator=(const BodyManager&) = delete;
	BodyManager& operator=(BodyManager&&) = delete;
	~BodyManager() = default;

	std::unordered_map<EntityId, AnyBody> bodies_{};
};

#endif // !BODY_MANAGER_H