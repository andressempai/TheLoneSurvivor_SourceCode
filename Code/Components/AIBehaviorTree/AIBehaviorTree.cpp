#include "AIBehaviorTree.h"
#include "../../BlackboardKeys/BlackboardKeys.h"
#include "../../EventSystem.h"
#include "../../MetaProgramming.h"
#include "../../DevelopmentTools.h"

#include <CrySchematyc/Env/Elements/EnvComponent.h>
#include <CrySchematyc/Env/IEnvRegistrar.h>
#include <CryCore/StaticInstanceList.h>

void register_behavior_tree_component(Schematyc::IEnvRegistrar& registrar)
{
	registrar.Scope(IEntity::GetEntityScopeGUID()).Register(SCHEMATYC_MAKE_ENV_COMPONENT(AIBehaviorTree));
}

CRY_STATIC_AUTO_REGISTER_FUNCTION(&register_behavior_tree_component);

template <typename BlackboardVariable>
void initialize_blackboard_variables_impl(IEntity& entity)
{
	if (const auto blackboard = DevelopmentTools::get_blackboard(entity.GetId()))
		blackboard->SetVariable(BlackboardVariable::name_.data(), BlackboardVariable::default_value_);
}

template <template <typename...> typename TypeListLike, typename... BlackboardVariable>
void initialize_blackboard_variables(IEntity& entity, TypeListLike<BlackboardVariable...>)
{
	(initialize_blackboard_variables_impl<BlackboardVariable>(entity), ...);
}

void ReflectType(Schematyc::CTypeDesc<AIBehaviorTree>& desc)
{
	desc.SetGUID("{61AA1668-BA59-4607-8DDB-7D9DE1917BFD}"_cry_guid);
	desc.SetEditorCategory("Game Components");
	desc.SetName(Schematyc::CTypeName("behaviortreecomponent"));
	desc.SetLabel("Behavior Tree");
	desc.SetDescription("Allow Entity to use any Behavior Tree");
	desc.SetComponentFlags(EEntityComponentFlags::Singleton);
}

void AIBehaviorTree::Initialize()
{
	const auto is_running = DevelopmentTools::start_behavior_tree_in_entity(*m_pEntity, name_.data());

	using BlackboardVariables = MP::type_list<
			BlackboardKey::EnemyIdBlackboardKey
		,	BlackboardKey::IsInAttackRangeBlackboardKey
		,	BlackboardKey::CooldownCompletedBlackboardKey
		,	BlackboardKey::CanSearchBlackboardKey
		,	BlackboardKey::IsStunnedBlackboardKey
	>;
	initialize_blackboard_variables(*m_pEntity, BlackboardVariables{});

	if (const auto blackboard = DevelopmentTools::get_blackboard(GetEntityId()))
		if (const auto male_arms = DevelopmentTools::search_entity_by_name("male_arms"))
			DevelopmentTools::set_value_in_blackboard("enemy_id", *blackboard, male_arms->GetId());

	BoomerShooterEventSystem::instance().subscribe<DeadEvent>(GetEntityId(), [this](const EventBase&)
	{
		if (not is_dead_) {
			is_dead_ = true;
			DevelopmentTools::stop_behavior_tree_in_entity(*m_pEntity);
		}
	});
}

void AIBehaviorTree::OnShutDown()
{
	DevelopmentTools::stop_behavior_tree_in_entity(*m_pEntity);
	BoomerShooterEventSystem::instance().unsubscribe<DeadEvent>(GetEntityId());
}