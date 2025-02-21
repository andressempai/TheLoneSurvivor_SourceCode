#ifndef DVENTITY_H
#define DVENTITY_H

#include "../MetaProgramming.h"
#include "../DevelopmentTools.h"

template <typename T, typename = void>
struct HasSetEntityId : std::false_type {};

template <typename T>
struct HasSetEntityId<T, std::void_t<decltype(std::declval<T>().set_entity_id(INVALID_ENTITYID))>> : std::true_type {};

template <
		typename TagType
	,	typename ChildEntityList
	,	typename ChildEntityEventListenerList
	,	typename ComponentsList
	,	typename DataType
	,	typename ComponentData
>
class DVEntity : public IEntityComponent {
	struct CreateComponent {
		explicit CreateComponent(DVEntity& entity, const DataType& data) : entity_{ entity }, data_{ data } {}

		template <typename Component>
		bool operator()() const
		{
			auto& component = std::get<Component>(entity_.components_);

			using ComponentType = std::remove_pointer_t<Component>;
			if constexpr (std::is_constructible_v<ComponentType, DataType>)
				component = entity_.m_pEntity->GetOrCreateComponentClass<ComponentType>(data_);
			else 
				component = entity_.m_pEntity->GetOrCreateComponent<ComponentType>();

			return false;
		}

		DVEntity& entity_;
		const DataType& data_;
	};

	struct CreateChildEntity {
		CreateChildEntity(DVEntity& entity) : entity_{ entity } {}

		template <typename Component>
		bool operator()() const
		{
			auto spawn_params = SEntitySpawnParams{};
			spawn_params.sName = "child_entity";
			if (const auto child_entity = DevelopmentTools::create_entity<Component>(spawn_params)) {
				add_event_listener(child_entity, ChildEntityEventListenerList{});

				using empty_list = MP::type_list<>;
				using listeners_with_set_entity_id = MP::conditional_push_back_elements_t<
						HasSetEntityId
					,	empty_list
					,	ChildEntityEventListenerList
				>;

				if constexpr (not MP::is_empty_v<listeners_with_set_entity_id>)
					set_entity_id(entity_.get().GetEntityId(), listeners_with_set_entity_id{});

				entity_.get().m_pEntity->AttachChild(child_entity);
			}

			return false;
		}

		template <typename EventListener>
		void add_event_listener_impl(IEntity* child_entity) const
		{
			auto& event_listener = std::get<EventListener>(entity_.get().child_entity_event_listeners_);
			child_entity->AddEventListener(EventListener::event_to_listen, std::addressof(event_listener));
		}

		template <template <typename...> typename TypeListLike, typename... EventListener>
		void add_event_listener(IEntity* child_entity, TypeListLike<EventListener...>) const
		{
			(add_event_listener_impl<EventListener>(child_entity), ...);
		}

		template <typename EventListener>
		void set_entity_id_impl(EntityId id) const
		{
			auto& event_listener = std::get<EventListener>(entity_.get().child_entity_event_listeners_);
			event_listener.set_entity_id(id);
		}

		template <template <typename...> typename TypeListLike, typename... EventListener>
		void set_entity_id(EntityId id, TypeListLike<EventListener...>) const
		{
			(set_entity_id_impl<EventListener>(id), ...);
		}

		std::reference_wrapper<DVEntity> entity_;
	};

public:
	using PointerToComponentList = MP::replace_each_type_in_list_with_t<ComponentsList, std::add_pointer_t>;
	using ComponentsTuple = MP::make_tuple_from_type_list_like_t<PointerToComponentList>;
	using ChildEntityEventListenerTuple = MP::make_tuple_from_type_list_like_t<ChildEntityEventListenerList>;
	using Data = DataType;
	using Tag = TagType;

	static void register_component(Schematyc::CEnvRegistrationScope& scope)
	{
		scope.Register(Schematyc::EnvComponent::template MakeShared<DVEntity>(SCHEMATYC_SOURCE_FILE_INFO));
	}

	static void ReflectType(Schematyc::CTypeDesc<DVEntity>& desc)
	{
		desc.SetGUID(ComponentData::guid);
		desc.SetEditorCategory(ComponentData::category.data());
		desc.SetName(Schematyc::CTypeName(ComponentData::name.data()));
		desc.SetLabel(ComponentData::label.data());
		desc.SetDescription(ComponentData::description.data());
		desc.SetComponentFlags({ EEntityComponentFlags::Singleton, EEntityComponentFlags::HiddenFromUser });
	}

private:
	void Initialize() override
	{
		MP::for_each_type(CreateComponent{ *this, DataType{} }, ComponentsTuple{});

		if constexpr (not MP::is_empty_v<ChildEntityList>)
			MP::for_each_type(CreateChildEntity{ *this }, ChildEntityList{});
	}

	ComponentsTuple components_{};
	ChildEntityEventListenerTuple child_entity_event_listeners_{};
};

#endif	// !DVENTITY_H