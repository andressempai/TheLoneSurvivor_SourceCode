#ifndef USER_INTERFACE_H
#define USER_INTERFACE_H

#include "DevelopmentTools.h"
#include "MetaProgramming.h"

#include <CryEntitySystem/IEntityComponent.h>

enum struct UserInterfaceState : uint8 { visible, hidden };
static void ReflectType(Schematyc::CTypeDesc<UserInterfaceState>& desc)
{
	desc.SetGUID("{85251125-DA14-4E82-849F-C5F1F15828C5}"_cry_guid);
	desc.AddConstant(UserInterfaceState::visible, "visible", "Visible");
	desc.AddConstant(UserInterfaceState::hidden, "hidden", "Hidden");
}

template <typename T, typename = void>
struct has_user_interface_state {
	static constexpr auto value = UserInterfaceState::visible;
};

template <typename T>
struct has_user_interface_state<T, std::void_t<decltype(T::user_interface_state_)>> {
	static constexpr auto value = T::user_interface_state_;
};

template <
		typename Data
	,	typename ComponentData
	,	typename EventToSubscribeList
	,	typename EventListenerList
>
class UserInterface final : public IEntityComponent {
public:
	static void register_component(Schematyc::CEnvRegistrationScope& scope)
	{
		scope.Register(Schematyc::EnvComponent::template MakeShared<UserInterface>(SCHEMATYC_SOURCE_FILE_INFO));
	}

	static void ReflectType(Schematyc::CTypeDesc<UserInterface>& desc)
	{
		desc.SetGUID(ComponentData::guid);
		desc.SetEditorCategory(ComponentData::category.data());
		desc.SetName(Schematyc::CTypeName(ComponentData::name.data()));
		desc.SetLabel(ComponentData::label.data());
		desc.SetDescription(ComponentData::description.data());
		desc.AddMember(
				&UserInterface::state_
			,	'uist'
			,	"state"
			,	"State"
			,	"User Interface's State"
			,	UserInterfaceState::visible
		);
	}

	void show_ui_element()
	{
		if (ui_element_)
			DevelopmentTools::set_ui_element_visible(*ui_element_, true);
	}

	void hide_ui_element()
	{
		if (ui_element_)
			DevelopmentTools::set_ui_element_visible(*ui_element_, false);
	}

	bool is_ui_element_visible() const
	{
		return ui_element_ and DevelopmentTools::is_ui_element_visible(*ui_element_);
	}

private:
	void Initialize() override
	{
		std::apply([this](auto&&... event) { (event.subcribe_to_event(GetEntityId()), ...); }, events_);

		auto add_as_ui_element_event_listener = [this](auto&&... event_listener)
		{
			(ui_element_->AddEventListener(std::addressof(event_listener), event_listener.event_name_.data()), ...);
		};
		std::apply(add_as_ui_element_event_listener, ui_element_event_subscribers_);

		if (state_ == UserInterfaceState::visible)
			show_ui_element();
	}

	void OnShutDown() override
	{
		std::apply([this](auto&&... event) { (event.unscribe_to_event(GetEntityId()), ...); }, events_);
		auto remove_as_ui_element_event_listener = [this](auto&&... event_listener)
		{
			(ui_element_->RemoveEventListener(std::addressof(event_listener)), ...);
		};
		std::apply(remove_as_ui_element_event_listener, ui_element_event_subscribers_);
	}

	Cry::Entity::EventFlags GetEventMask() const override
	{
		return Cry::Entity::EEvent::EditorPropertyChanged;
	}

	void ProcessEvent(const SEntityEvent& event) override
	{
		if (state_ == UserInterfaceState::visible)
			show_ui_element();
		else
			hide_ui_element();
	}

	template <template <typename...> typename TypeListLike, typename... Events>
	static constexpr auto initialize_event_tuple(ObjectPointer<IUIElement> ui_element, TypeListLike<Events...>)
	{
		return std::make_tuple(Events{ *ui_element }...);
	}

	ObjectPointer<IUIElement> ui_element_{ DevelopmentTools::get_ui_element(Data::ui_element_name_.data()) };
	MP::make_tuple_from_type_list_like_t<EventToSubscribeList> events_{
		initialize_event_tuple(ui_element_, EventToSubscribeList{})
	};
	MP::make_tuple_from_type_list_like_t<EventListenerList> ui_element_event_subscribers_{};
	UserInterfaceState state_{ has_user_interface_state<Data>::value };
};

#endif // !USER_INTERFACE_H