#ifndef DEVELOPMENT_TOOLS_H
#define DEVELOPMENT_TOOLS_H

#include "Utils/ObjectPointer.h"
#include "Utils/Ray.h"

#include <CryEntitySystem/IEntity.h>
#include <CryAction/IActionMapManager.h>
#include <CryGame/IGameFramework.h>
#include <CryPhysics/physinterface.h>
#include <CryAnimation/ICryMannequin.h>
#include <CryAISystem/BehaviorTree/IBehaviorTree.h>
#include <Cry3DEngine/IMaterial.h>
#include <Cry3DEngine/ISurfaceType.h>
#include <CryParticleSystem/IParticles.h>
#include <MaterialEffects/IMFXEffect.h>
#include <CrySystem/Scaleform/IFlashUI.h>
#include <CrySchematyc/CoreAPI.h>

#include <string_view>
#include <vector>
#include <memory>
#include <type_traits>
#include <algorithm>
#include <array>
#include <cstddef>
#include <optional>
#include <random>

struct IGeometry;
struct ICharacterInstance;
struct IAnimationDatabase;
struct SControllerDef;
struct SAnimationContext;
struct IActionController;
struct SCharacterDimensions;
struct SCharacterDynamics;

using namespace std::string_view_literals;

namespace primitives {
	struct primitive;
}

namespace DevelopmentTools {
	using EntitiesVector = std::vector<IEntity*>;
	using AnimationContextPtr = std::unique_ptr<SAnimationContext>;

	enum EComponentEventPriority {
			very_high	= 11
		,	high
		,	medium
		,	low
		,	very_low
	};

	enum {
		invalid_slot_id = -1
	};

	enum {
		local_player_id = LOCAL_PLAYER_ENTITY_ID
	};

	template <typename T>
	auto make_ptr(T* ptr) noexcept
	{
		return ObjectPointer<T>(ptr);
	}

	template <typename T, typename U>
	auto static_ptr_cast(U* ptr) noexcept
	{
		return make_ptr(static_cast<T*>(ptr));
	}

	template <typename T>
	T make_random_number(T min, T max)
	{
		static_assert(std::is_arithmetic_v<T>, "Type must be arithmetic");

		std::random_device random_device{};
		std::mt19937 random_generator{ random_device() };

		if constexpr (std::is_integral_v<T>) {
			std::uniform_int_distribution<T> distribution{ min, max };
			return distribution(random_generator);
		}
		else {
			std::uniform_real_distribution<T> distribution{ min, max };
			return distribution(random_generator);
		}
	}

	inline auto make_vector3(f32 value)
	{
		return Vec3{ value };
	}

	inline auto make_vector3(Vec2 xy, f32 z)
	{
		return Vec3{ xy, z };
	}

	inline auto make_vector3(f32 x, f32 y, f32 z)
	{
		return Vec3{ x, y, z };
	}

	inline auto make_zero_vector3()
	{
		return Vec3{ ZERO };
	}

	inline auto make_unit_vector3()
	{
		return make_vector3(1.0f);
	}

	inline auto make_up_vector3()
	{
		return make_vector3(0.0f, 0.0f, 1.0f);
	}

	inline auto make_down_vector3()
	{
		return make_vector3(0.0f, 0.0f, -1.0f);
	}

	inline auto make_forward_vector3()
	{
		return make_vector3(0.0f, 1.0f, 0.0f);
	}

	inline auto make_back_vector3()
	{
		return make_vector3(0.0f, -1.0f, 0.0f);
	}

	inline auto make_right_vector3()
	{
		return make_vector3(1.0f, 0.0f, 0.0f);
	}

	inline auto make_left_vector3()
	{
		return make_vector3(-1.0f, 0.0f, 0.0f);
	}

	inline auto make_matrix34(const QuatTS& transform)
	{
		return Matrix34{ transform };
	}

	inline auto make_matrix33(const Matrix34 matrix)
	{
		return Matrix33{ matrix };
	}

	inline auto make_ang3(f32 x, f32 y)
	{
		return Ang3{ x, y, 0.0f };
	}

	template <typename First, typename... Tail>
	[[nodiscard]] inline constexpr auto make_array(First&& first, Tail&&... tail)
	{
		static_assert(
				std::conjunction_v<std::is_same<std::decay_t<First>, std::decay_t<Tail>>...>
			,	"All Arguments must be the same type"
		);

		return std::array{ std::forward<First>(first), std::forward<Tail>(tail)... };
	}

	template <typename TComponent, typename... Ts>
	inline auto create_entity(SEntitySpawnParams& spawn_params, Ts&&... constructor_params)
	{
		static_assert(std::is_base_of_v<IEntityComponent, TComponent>, "Component must derived from IEntityComponent");

		const auto entity = make_ptr(gEnv->pEntitySystem->SpawnEntity(spawn_params));
		if (entity)
			entity->CreateComponentClass<TComponent>(std::forward<Ts>(constructor_params)...);

		return entity;
	}

	inline void destroy_entity_by_id(EntityId id)
	{
		gEnv->pEntitySystem->RemoveEntity(id);
	}

	template <typename Component, typename = std::enable_if_t<std::is_base_of_v<IEntityComponent, Component>>>
	[[nodiscard]] inline EntitiesVector get_all_entities_of_component_type()
	{
		std::vector<IEntity*> entities{};
		entities.reserve(100);

		for (auto iter = gEnv->pEntitySystem->GetEntityIterator(); not iter->IsEnd(); iter->Next())
			if (iter->This()->GetComponent<Component>())
				entities.push_back(iter->This());

		return entities;
	}

	[[nodiscard]] inline auto search_entity_by_id(EntityId id)
	{
		return make_ptr(gEnv->pEntitySystem->GetEntity(id));
	}

	[[nodiscard]] inline auto search_entity_by_name(std::string_view name)
	{
		return make_ptr(gEnv->pEntitySystem->FindEntityByName(name.data()));
	}

	[[nodiscard]] inline auto search_entity_by_physics(IPhysicalEntity* physical_entity)
	{
		return make_ptr(gEnv->pEntitySystem->GetEntityFromPhysics(physical_entity));
	}

	[[nodiscard]] inline auto get_child(const IEntity& entity, uint32 child_index = 0)
	{
		return make_ptr(entity.GetChild(child_index));
	}

	[[nodiscard]] inline auto get_parent(const IEntity& entity)
	{
		return make_ptr(entity.GetParent());
	}

	template <typename Component, typename = std::enable_if_t<std::is_base_of_v<IEntityComponent, Component>>>
	[[nodiscard]] inline auto get_component_from_entity(const IEntity& entity)
	{
		return make_ptr(entity.GetComponent<Component>());
	}

	template <typename Component, typename = std::enable_if_t<std::is_base_of_v<IEntityComponent, Component>>>
	inline auto entity_has_component(const IEntity& entity)
	{
		return get_component_from_entity<Component>(entity).operator bool();
	}

	[[nodiscard]] inline auto load_geometry(IEntity& entity, int32 slot, Schematyc::GeomFileName file_path)
	{
		return entity.LoadGeometry(slot, file_path.value.c_str());
	}

	[[nodiscard]] inline auto get_render_node(IEntity& entity, int32 slot)
	{
		return make_ptr(entity.GetSlotRenderNode(slot));
	}

	inline auto create_character(IEntity& entity, int32 slot, Schematyc::CharacterFileName file_path)
	{
		ObjectPointer<ICharacterInstance> temp{};
		if (auto* const character_instance = gEnv->pCharacterManager->CreateInstance(file_path.value)) {
			entity.SetCharacter(character_instance, slot);
			temp = character_instance;
		}

		return temp;
	}

	[[nodiscard]] inline auto load_character(IEntity& entity, int32 slot, Schematyc::CharacterFileName file_path)
	{
		return entity.LoadCharacter(slot, file_path.value.c_str());
	}

	[[nodiscard]] inline auto get_attachment_from_character(ICharacterInstance& character, std::string_view attachment_name)
	{
		return make_ptr(character.GetIAttachmentManager()->GetInterfaceByName(attachment_name.data()));
	}

	inline void load_material(IEntity& entity, int slot, Schematyc::MaterialFileName file_path)
	{
		auto* const material_manager = gEnv->p3DEngine->GetMaterialManager();

		if (slot != invalid_slot_id)
			if (auto* const material = material_manager->LoadMaterial(file_path.value.c_str(), false))
				entity.SetSlotMaterial(slot, material);
	}

	inline void set_shadow_casting(IEntity& entity, bool cast_shadow)
	{
		if (cast_shadow)
			entity.AddFlags(ENTITY_FLAG_CASTSHADOW);
		else
			entity.SetFlags(entity.GetFlags() & ~ENTITY_FLAG_CASTSHADOW);
	}

	inline auto get_character_from_entity(IEntity& entity, int slot)
	{
		return make_ptr(entity.GetCharacter(slot));
	}

	inline auto get_physics_from_entity(const IEntity& entity)
	{
		return make_ptr(entity.GetPhysicalEntity());
	}

	inline auto create_physics(IEntity& entity, SEntityPhysicalizeParams& params)
	{
		entity.Physicalize(params);

		return get_physics_from_entity(entity);
	}

	template <typename... PE_Params>
	void add_complemetary_physics_params(const IEntity& entity, PE_Params&&... params)
	{
		static_assert(sizeof...(PE_Params) > 0, "At least one parameter must be passed");
		static_assert((std::is_base_of_v<pe_params, std::decay_t<PE_Params>> && ...));

		if (const auto physical_entity = get_physics_from_entity(entity))
			(physical_entity->SetParams(std::addressof(params)), ...);
	}

	template <typename PE_Params, typename = std::enable_if_t<std::is_base_of_v<pe_params, std::decay_t<PE_Params>>>>
	void update_physics_params(const IEntity& entity, PE_Params&& params)
	{
		if (const auto physical_entity = get_physics_from_entity(entity))
			physical_entity->SetParams(std::addressof(params));
	}

	inline void destroy_physics(IEntity& entity, int32 slot)
	{
		auto params = SEntityPhysicalizeParams{};
		params.type = PE_NONE;
		params.nSlot = slot;

		entity.Physicalize(params);
	}

	template <typename PE_Params, typename = std::enable_if_t<std::is_base_of_v<pe_params, pe_params>>>
	[[nodiscard]] inline auto get_physics_params(const IEntity& entity)
	{
		auto params = PE_Params{};
		if (const auto physical_entity = get_physics_from_entity(entity))
			physical_entity->GetParams(&params);

		return params;
	}

	template <typename PE_Status, typename = std::enable_if_t<std::is_base_of_v<pe_status, PE_Status>>>
	[[nodiscard]] inline auto get_physics_status(const IEntity& entity)
	{
		auto status = PE_Status{};
		if (const auto physical_entity = get_physics_from_entity(entity))
			physical_entity->GetStatus(&status);

		return status;
	}

	template <typename PE_Status>
	[[nodiscard]] inline auto get_physics_status(IPhysicalEntity* entity)
	{
		return get_physics_status<PE_Status>(*search_entity_by_physics(entity));
	}

	inline auto has_physic_type(const IPhysicalEntity& physical_entity, pe_type type)
	{
		return physical_entity.GetType() == type;
	}

	inline void move_entity_by_physics(const IEntity& entity, Vec3 direction, float delta_time = 0.0f)
	{
		if (const auto physical_entity = get_physics_from_entity(entity)) {
			auto action_move = pe_action_move{};
			action_move.dir = direction;
			action_move.iJump = 0;
			action_move.dt = delta_time;

			physical_entity->Action(&action_move);
		}
	}

	inline void launch_entity_by_physics(const IEntity& entity, Vec3 direction)
	{
		if (const auto physical_entity = get_physics_from_entity(entity)) {
			const auto living_status = get_physics_status<pe_status_living>(physical_entity);

			auto current_velocity = living_status.vel;
			current_velocity.z = 0.0f;

			auto set_velocity_action = pe_action_set_velocity{};
			set_velocity_action.v = current_velocity;

			physical_entity->Action(&set_velocity_action);

			auto move_action = pe_action_move{};
			move_action.dir = direction;
			move_action.iJump = 2;

			physical_entity->Action(&move_action);
		}
	}

	inline void impulse_entity_by_physics(const IEntity& entity, Vec3 point = ZERO, Vec3 force = ZERO, int32 part_id = -1)
	{
		if (const auto physical_entity = get_physics_from_entity(entity)) {
			auto set_velocity_action = pe_action_set_velocity{};
			set_velocity_action.v = ZERO;
			
			physical_entity->Action(&set_velocity_action);

			auto impulse_action = pe_action_impulse{};
			impulse_action.impulse = force;
			impulse_action.point = point.IsZero() ? entity.GetWorldTM().GetColumn3() : point;
			if (part_id >= 0) impulse_action.partid = part_id;

			physical_entity->Action(&impulse_action);
		}
	}

	[[nodiscard]] inline auto find_scope_context(const SControllerDef& controller_definition, std::string_view scope_context)
	{
		return controller_definition.m_scopeContexts.Find(scope_context.data());
	}

	[[nodiscard]] inline auto find_fragment_id(const SControllerDef& controller_definition, std::string_view fragment_name)
	{
		return controller_definition.m_fragmentIDs.Find(fragment_name.data());
	}

	[[nodiscard]] inline auto find_tag_id(const SControllerDef& controller_definition, std::string_view tag_name)
	{
		return controller_definition.m_tags.Find(tag_name.data());
	}

	inline void set_animation_tag(IActionController* action_controller, TagID tag_id, bool value)
	{
		action_controller->GetContext().state.Set(tag_id, value);
	}

	inline void set_motion_params(IEntity& entity, int32 character_slot, EMotionParamID motion_param, f32 value)
	{
		if (const auto character = get_character_from_entity(entity, character_slot))
			character->GetISkeletonAnim()->SetDesiredMotionParam(motion_param, value, 0.0f);
	}

	inline void set_motion_params(ICharacterInstance& character, EMotionParamID motion_param, f32 value)
	{
		character.GetISkeletonAnim()->SetDesiredMotionParam(motion_param, value, 0.0f);
	}

	inline void drive_motion_by_animation(IEntity& entity, int32 character_slot, bool drive_motion)
	{
		if (const auto character = get_character_from_entity(entity, character_slot))
			character->GetISkeletonAnim()->SetAnimationDrivenMotion(drive_motion);
	}

	[[nodiscard]] inline auto load_animation_data_base(Schematyc::MannequinAnimationDatabasePath path)
	{
		auto& mannequin = gEnv->pGameFramework->GetMannequinInterface();
		auto& database_manager = mannequin.GetAnimationDatabaseManager();

		return make_ptr(database_manager.Load(path.value.c_str()));
	}

	[[nodiscard]] inline auto load_controller_definition(Schematyc::MannequinControllerDefinitionPath path)
	{
		auto& mannequin = gEnv->pGameFramework->GetMannequinInterface();
		auto& database_manager = mannequin.GetAnimationDatabaseManager();

		return make_ptr(database_manager.LoadControllerDef(path.value.c_str()));
	}

	[[nodiscard]] inline auto create_animation_context(const SControllerDef& controller_def)
	{
		return std::make_unique<SAnimationContext>(controller_def);
	}

	void inline destroy_animation_context(AnimationContextPtr& animation_context)
	{
		animation_context.release();
	}

	[[nodiscard]] inline auto create_action_controller(IEntity* entity, SAnimationContext& animation_context)
	{
		return make_ptr(gEnv->pGameFramework->GetMannequinInterface().CreateActionController(entity, animation_context));
	}

	inline void destroy_action_controller(IActionController*& action_controller)
	{
		SAFE_RELEASE(action_controller);
	}

	[[nodiscard]] inline auto make_animation_action(
			std::size_t priority
		,	FragmentID fragment_id
		,	IAction::EFlags flags = IAction::BlendOut
	)
	{
		return make_ptr(new TAction<SAnimationContext>(priority, fragment_id, TAG_STATE_EMPTY, flags));
	}

	inline void play_2d_sound(std::string_view name)
	{
		gEnv->pAudioSystem->ExecuteTrigger(CryAudio::StringToId(name.data()));
	}

	[[nodiscard]] inline auto create_transformed_aabb(const Matrix34& matrix, Vec3 size)
	{
		return AABB::CreateTransformedAABB(matrix, { -size * 0.5f, size * 0.5f });
	}

	[[nodiscard]] inline auto create_geometry(int32 type, const primitives::primitive* const prim)
	{
		return make_ptr(gEnv->pPhysicalWorld->GetGeomManager()->CreatePrimitive(type, prim));
	}

	inline auto make_ray(Vec3 origin, Vec3 direction, f32 ray_distance = 1.0f)
	{
		return SRay{ origin, direction * ray_distance };
	}

	inline auto make_ray(const Matrix34& matrix, f32 ray_distance = 1.0f)
	{
		return make_ray(matrix.GetColumn3(), matrix.GetColumn1(), ray_distance);
	}

	class CRayWorldIntersectionControls {
		friend std::optional<ray_hit> ray_cast(const CRayWorldIntersectionControls&);

	public:
		CRayWorldIntersectionControls(const SRay& ray) noexcept
			:	ray_{ ray }
			,	ignored_entities_{ nullptr }
			,	max_ignored_entities_{ 0 }
			,	max_hits_{ 0 }
			,	entity_query_{ 0 }
			,	flags_{ 0 }
		{}

		CRayWorldIntersectionControls& ignored_entities(IPhysicalEntity** entities_to_ignored)
		{
			ignored_entities_ = entities_to_ignored;
			return *this;
		}

		CRayWorldIntersectionControls& max_ignored_entities(int ignored)
		{
			max_ignored_entities_ = ignored;
			return *this;
		}

		CRayWorldIntersectionControls& max_hits(int max)
		{
			max_hits_ = max;
			return *this;
		}

		CRayWorldIntersectionControls& entity_query(uint32 query)
		{
			entity_query_ = query;
			return *this;
		}

		CRayWorldIntersectionControls& flags(uint32 rwiflags)
		{
			flags_ = rwiflags;
			return *this;
		}

	private:
		SRay ray_;
		IPhysicalEntity** ignored_entities_;
		int max_ignored_entities_;
		int max_hits_;
		uint32 entity_query_;
		uint32 flags_;
	};

	inline std::optional<ray_hit> ray_cast(const CRayWorldIntersectionControls& controls)
	{
		auto hit_result = ray_hit{};
	
		if (gEnv->pPhysicalWorld->RayWorldIntersection(
				controls.ray_.origin_
			,	controls.ray_.direction_
			,	controls.entity_query_
			,	controls.flags_
			,	&hit_result
			,	controls.max_hits_
			,	controls.ignored_entities_
			,	controls.max_ignored_entities_
		)) {
			return hit_result;
		}
	
		return std::nullopt;
	}

	template <typename T>
	class TIterator {
		using Category = std::forward_iterator_tag;
		using Distance = std::ptrdiff_t;

		using Pointer = T*;
		using Reference = T&;

	public:
		TIterator(Pointer pointer)
			:	pointer_{ pointer }
		{}

		TIterator operator++()
		{
			++pointer_;
			return *this;
		}

		TIterator operator++(int)
		{
			TIterator temp{ *this };

			++(*this);

			return temp;
		}

		Reference operator*()
		{
			return *pointer_;
		}

		Pointer operator->()
		{
			return *pointer_;
		}

		friend bool operator==(const TIterator& lhs, const TIterator& rhs)
		{
			return lhs.pointer_ == rhs.pointer_;
		}

		friend bool operator!=(const TIterator& lhs, const TIterator& rhs)
		{
			return !(operator==(lhs, rhs));
		}

	private:
		Pointer pointer_;
	};

	template <typename TEntity>
	class TEntityDynArray {
	public:
		TEntityDynArray(TEntity** entities, int number_entities)
			:	entities_{ std::move(entities) }
			,	number_entities_{ number_entities }
		{}

		auto begin() const
		{
			return TIterator<TEntity*>{ entities_ };
		}

		auto end() const
		{
			return TIterator<TEntity*>{ entities_ + number_entities_};
		}

	private:
		TEntity** entities_;
		int number_entities_;
	};

	[[nodiscard]] inline TEntityDynArray<IPhysicalEntity> simulate_explosion(
			pe_explosion& explosion_params
		,	std::vector<IPhysicalEntity*>& ignored_entities
		,	int ent_types = ent_rigid | ent_sleeping_rigid | ent_living | ent_independent
	)
	{
		gEnv->pPhysicalWorld->SimulateExplosion(
				&explosion_params
			,	not ignored_entities.empty() ? ignored_entities.data() : nullptr
			,	not ignored_entities.empty() ? ignored_entities.size() : 0
			,	ent_types
		);

		return { explosion_params.pAffectedEnts, explosion_params.nAffectedEnts };
	}

	[[nodiscard]] inline auto find_particle_effect(Schematyc::ParticleEffectName name)
	{
		return make_ptr(gEnv->pParticleManager->FindEffect(name.value));
	}

	inline auto load_particle_emmiter(
			IEntity& entity
		,	int32 slot
		,	IParticleEffect* particle_effect
		,	const SpawnParams* spawn_params = nullptr
	)
	{
		return entity.LoadParticleEmitter(slot, particle_effect, spawn_params);
	}

	[[nodiscard]] inline auto get_particle_emmiter(IEntity& entity, int32 slot)
	{
		return make_ptr(entity.GetParticleEmitter(slot));
	}

	[[nodiscard]] inline auto get_runtime_effect_id(std::string_view effect_name, int32 surface_index)
	{
		return gEnv->pMaterialEffects->GetEffectId(effect_name.data(), surface_index);
	}

	inline auto execute_runtime_effect_id(TMFXEffectId effect_id, SMFXRunTimeEffectParams& runtime_effect_params)
	{
		return gEnv->pMaterialEffects->ExecuteEffect(effect_id, runtime_effect_params);
	}

	inline auto get_entities_in_box(Vec3 min, Vec3 max, int32 entity_type_mask)
	{
		IPhysicalEntity** result{};
		const auto size = gEnv->pPhysicalWorld->GetEntitiesInBox(min, max, result, entity_type_mask);

		return DevelopmentTools::TEntityDynArray{ result, size };
	}

	inline auto start_behavior_tree_in_entity(const IEntity& entity, std::string_view tree_name)
	{
		return gEnv->pAISystem->GetIBehaviorTreeManager()->StartModularBehaviorTree(entity.GetId(), tree_name.data());
	}

	inline void stop_behavior_tree_in_entity(const IEntity& entity)
	{
		gEnv->pAISystem->GetIBehaviorTreeManager()->StopModularBehaviorTree(entity.GetId());
	}

	inline auto get_blackboard(EntityId entity_id)
	{
		CRY_ASSERT(entity_id != INVALID_ENTITYID);
		return make_ptr(gEnv->pAISystem->GetIBehaviorTreeManager()->GetBehaviorTreeBlackboard(entity_id));
	}

	template <typename T>
	auto get_value_from_blackboard(std::string_view key, BehaviorTree::Blackboard& blackboard)
	{
		T value{};
		blackboard.GetVariable(key.data(), value);
	
		return value;
	}

	template <typename T>
	void set_value_in_blackboard(std::string_view key, BehaviorTree::Blackboard& blackboard, T value)
	{
		blackboard.SetVariable(key.data(), value);
	}

	inline auto get_surface_type(int32 surface_idx)
	{
		return make_ptr(gEnv->p3DEngine->GetMaterialManager()->GetSurfaceType(surface_idx));
	}

	inline auto get_ui_element(std::string_view name)
	{
		return make_ptr(gEnv->pFlashUI->GetUIElement(name.data()));
	}

	inline void set_ui_element_visible(IUIElement& ui_element, bool visible)
	{
		ui_element.SetVisible(visible);
	}

	inline bool is_ui_element_visible(const IUIElement& ui_element)
	{
		return ui_element.IsVisible();
	}

	inline void pause_game()
	{
		gEnv->pGameFramework->PauseGame(true, true);
	}

	inline void resume_game()
	{
		gEnv->pGameFramework->PauseGame(false, true);
	}

	inline auto is_game_pause()
	{
		return gEnv->pGameFramework->IsGamePaused();
	}

	inline void enable_action_map(std::string_view action_map_name)
	{
		gEnv->pGameFramework->GetIActionMapManager()->EnableActionMap(action_map_name.data(), true);
	}

	inline void disable_action_map(std::string_view action_map_name)
	{
		gEnv->pGameFramework->GetIActionMapManager()->EnableActionMap(action_map_name.data(), false);
	}

	inline void add_action_listener(IActionListener* listener, std::string_view action_map_name)
	{
		gEnv->pGameFramework->GetIActionMapManager()->AddExtraActionListener(listener, action_map_name.data());
	}

	inline void remove_action_listener(IActionListener* listener)
	{
		gEnv->pGameFramework->GetIActionMapManager()->RemoveExtraActionListener(listener);
	}

	template <typename TEnum, typename = std::enable_if_t<std::is_enum_v<TEnum>>>
	constexpr inline auto get_underlying_value_from_enum(TEnum enum_value)
	{
		return static_cast<std::underlying_type_t<TEnum>>(enum_value);
	}

	template <typename T>
	void see_type(T)
	{
		CryWarning(VALIDATOR_MODULE_GAME, VALIDATOR_WARNING, __FUNCSIG__);
	}
}

#endif // !DEVELOPMENT_TOOLS_H