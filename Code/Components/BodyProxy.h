#ifndef BODY_PROXY_H
#define BODY_PROXY_H

#include "BodyManager.h"

#include <CryEntitySystem/IEntityComponent.h>

template <typename T, typename = void>
struct has_set_body : std::false_type {};

template <typename T>
struct has_set_body<T, std::void_t<decltype(std::declval<T>().set_body(nullptr))>> : std::true_type {};

template <
		typename Body
	,	typename ComponentData
	,	typename EventToSubscribeList
>
class BodyProxy final : public IEntityComponent {
public:
	friend void ReflectType(Schematyc::CTypeDesc<BodyProxy>& desc)
	{
		desc.SetGUID(ComponentData::guid);
		desc.SetEditorCategory(ComponentData::category.data());
		desc.SetName(Schematyc::CTypeName(ComponentData::name.data()));
		desc.SetLabel(ComponentData::label.data());
		desc.SetDescription(ComponentData::description.data());
		desc.SetComponentFlags(EEntityComponentFlags::Singleton);
	}

	static void register_component(Schematyc::CEnvRegistrationScope& scope)
	{
		scope.Register(Schematyc::EnvComponent::template MakeShared<BodyProxy>(SCHEMATYC_SOURCE_FILE_INFO));
	}

private:
	void Initialize() override
	{
		auto& body = BodyManager::instance().create_body<Body>(GetEntityId());
		auto& body_part = std::get<Body>(body);

		set_body_into_events(body_part, EventToSubscribeList{});

		send_health_update_event(body_part);
		send_armor_update_event(body_part);

		std::apply([&](auto&&... event) { (event.subcribe_to_event(GetEntityId()), ...); }, events_);
	}

	void OnShutDown() override
	{
		BodyManager::instance().remove_body(GetEntityId());
		std::apply([this](auto&&... event) { (event.unscribe_to_event(GetEntityId()), ...); }, events_);
	}

	void send_armor_update_event(Body& body_part)
	{
		if (body_part.armor_)
			BoomerShooterEventSystem::instance().send(GetEntityId(), ArmorUpdateEvent{ body_part.armor_->hardness() });
		else
			BoomerShooterEventSystem::instance().send(GetEntityId(), ArmorUpdateEvent{ 0 });
	}

	void send_health_update_event(Body& body_part)
	{
		BoomerShooterEventSystem::instance().send(GetEntityId(), HealthUpdateEvent{ body_part.health_pack_.health() });
	}

	template <typename Event>
	static constexpr void set_body_into_event_impl(Body& body, Event& event)
	{
		if constexpr (has_set_body<Event>::value)
			event.set_body(std::addressof(body));
	}

	template <template <typename...> typename TypeListLike, typename... Events>
	constexpr void set_body_into_events(Body& body, TypeListLike<Events...>)
	{
		(set_body_into_event_impl(body, std::get<Events>(events_)), ...);
	}

	MP::make_tuple_from_type_list_like_t<EventToSubscribeList> events_{};
};

#endif // !BODY_PROXY_H