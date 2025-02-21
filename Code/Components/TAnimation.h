#ifndef TANIMATION_H
#define TANIMATION_H

#include "DevelopmentTools.h"
#include "MetaProgramming.h"

#include <CryEntitySystem/IEntityComponent.h>

template <
		typename Data
	,	typename UserCode
	,	typename EventToSubscribeList
	,	typename ComponentData
>
class TAnimation final : public IEntityComponent {
public:
	static void register_component(Schematyc::CEnvRegistrationScope& scope)
	{
		scope.Register(Schematyc::EnvComponent::template MakeShared<TAnimation>(SCHEMATYC_SOURCE_FILE_INFO));
	}

	static void ReflectType(Schematyc::CTypeDesc<TAnimation>& desc)
	{
		desc.SetGUID(ComponentData::guid);
		desc.SetEditorCategory(ComponentData::category.data());
		desc.SetName(Schematyc::CTypeName(ComponentData::name.data()));
		desc.SetLabel(ComponentData::label.data());
		desc.SetDescription(ComponentData::description.data());
	}

private:
	void Initialize() override
	{
		destroy_mannequin();
		initialize_mannequin();

		CRY_ASSERT(controller_definition_ and action_controller_);

		unsubscribe_all_event_from_component();
		subscribe_all_event_to_component();

		user_code_.initialize_animation(*controller_definition_, *action_controller_);
	}

	void OnShutDown() override
	{
		destroy_mannequin();
		unsubscribe_all_event_from_component();
	}

	void ProcessEvent(const SEntityEvent& event) override
	{
		switch (event.event) {
		case Cry::Entity::EEvent::EditorPropertyChanged:
			Initialize();
			break;
		case Cry::Entity::EEvent::Update:
		{
			if (action_controller_) {
				action_controller_->Update(event.fParam[0]);
				user_code_.update(*m_pEntity, *action_controller_);
			}
		}
		break;

		}
	}

	Cry::Entity::EventFlags GetEventMask() const override
	{
		return Cry::Entity::EEvent::EditorPropertyChanged | Cry::Entity::EEvent::Update;
	}

	void initialize_mannequin()
	{
		using namespace DevelopmentTools;

		destroy_mannequin();

		if (controller_definition_path_.empty() or animation_database_path_.empty())
			return;

		controller_definition_ = load_controller_definition(controller_definition_path_.data());
		if (not controller_definition_)
			return;

		animation_database_ = load_animation_data_base(animation_database_path_.data());
		if (not animation_database_)
			return;

		animation_context_ = create_animation_context(*controller_definition_);
		if (not animation_context_)
			return;

		action_controller_ = create_action_controller(m_pEntity, *animation_context_);
		if (not action_controller_)
			return;

		for (const auto& scope_context : scope_contexts_) {
			if (scope_context.empty())
				break;

			const auto scope_context_id = find_scope_context(*controller_definition_, scope_context.data());
			action_controller_->SetScopeContext(
					scope_context_id
				,	*m_pEntity
				,	get_character_from_entity(*m_pEntity, 0)
				,	animation_database_
			);
		}

		drive_motion_by_animation(*m_pEntity, 0, animation_drive_motion_);
	}

	void destroy_mannequin()
	{
		if (animation_context_)
			DevelopmentTools::destroy_animation_context(animation_context_);

		if (action_controller_)
			DevelopmentTools::destroy_action_controller(action_controller_);
	}

	void subscribe_all_event_to_component()
	{
		std::apply([this](auto&&... event)
		{
			(event.subcribe_to_event(GetEntityId(), *action_controller_, *controller_definition_), ...);
		}, events_);
	}

	void unsubscribe_all_event_from_component()
	{
		std::apply([this](auto&&... event) { (event.unscribe_to_event(GetEntityId()), ...); }, events_);
	}

	template <typename Data>
	static constexpr auto try_initialize_user_code(Data)
	{
		if constexpr (std::is_constructible_v<UserCode, Data>)
			return UserCode{ Data{} };
		else
			return UserCode{};
	}

	template <template <typename...> typename TypeListLike, typename... Events>
	static constexpr auto initialize_event_tuple(UserCode& user_code, TypeListLike<Events...>)
	{
		return std::make_tuple(Events{ user_code }...);
	}

	UserCode user_code_{ try_initialize_user_code(Data{}) };
	MP::make_tuple_from_type_list_like_t<EventToSubscribeList> events_{
		initialize_event_tuple(user_code_, EventToSubscribeList{})
	};

	std::array<std::string_view, Data::scope_contexts_.size()> scope_contexts_{ Data::scope_contexts_ };
	std::string_view controller_definition_path_{ Data::controller_definition_file_path_ };
	std::string_view animation_database_path_{ Data::animation_data_base_file_path_ };
	const SControllerDef* controller_definition_{};
	const IAnimationDatabase* animation_database_{};
	std::unique_ptr<SAnimationContext> animation_context_{};
	IActionController* action_controller_{};
	bool animation_drive_motion_{ Data::animation_drive_motion_ };
};

#endif // !TANIMATION_H