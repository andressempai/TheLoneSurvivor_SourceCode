#include "AIObserver.h"
#include "../../EventSystem.h"
#include "../../DevelopmentTools.h"

#include <CrySchematyc/Env/Elements/EnvComponent.h>
#include <CrySchematyc/Env/IEnvRegistrar.h>
#include <CryCore/StaticInstanceList.h>
#include <CryAISystem/IAISystem.h>
#include <CryAISystem/VisionMapTypes.h>
#include <CryAISystem/IFactionMap.h>
#include <CryAISystem/BehaviorTree/IBehaviorTree.h>

void register_ai_observer_component(Schematyc::IEnvRegistrar& registrar)
{
	registrar.Scope(IEntity::GetEntityScopeGUID()).Register(SCHEMATYC_MAKE_ENV_COMPONENT(AIObserver));
}

CRY_STATIC_AUTO_REGISTER_FUNCTION(&register_ai_observer_component);

void ReflectType(Schematyc::CTypeDesc<AIObserver>& desc)
{
	desc.SetGUID("{6A605C68-47E3-4AE6-A4F4-6841C8D25FFB}"_cry_guid);
	desc.SetEditorCategory("AI Components");
	desc.SetName(Schematyc::CTypeName("aiobserver"));
	desc.SetLabel("AI Observer");
	desc.SetDescription("Allow Entity to obserb other Entities inside world.");
	desc.SetComponentFlags(EEntityComponentFlags::Singleton);
}

void AIObserver::Initialize()
{
	string vision_name = m_pEntity->GetName();
	vision_name.Append(".Observer");

	id_ = gEnv->pAISystem->GetVisionMap()->CreateVisionID(vision_name.c_str());

	params_.callback = functor(*this, &AIObserver::visibility_status_change);
	params_.skipListSize = 1;
	params_.skipList[0] = m_pEntity->GetPhysicalEntity();
	params_.faction = gEnv->pAISystem->GetFactionMap().GetFactionID("HostileOnlyWithPlayers");
	params_.typeMask = General | AliveAgent;
	params_.factionsToObserveMask = 3;
	params_.typesToObserveMask = General | AliveAgent | Player;
	params_.sightRange = 100.0f;
	params_.fovCos = crymath::cos(DEG2RAD(80.0f));
	params_.entityId = GetEntityId();
	params_.raycastFlags = AI_VISION_RAY_CAST_FLAG_BLOCKED_BY_SOLID_COVER;
	gEnv->pAISystem->GetVisionMap()->RegisterObserver(id_, params_);
}

Cry::Entity::EventFlags AIObserver::GetEventMask() const
{
	return Cry::Entity::EEvent::Update;
}

void AIObserver::ProcessEvent(const SEntityEvent& event)
{
	params_.eyePosition = m_pEntity->GetWorldPos();
	params_.eyeDirection = m_pEntity->GetForwardDir();
	gEnv->pAISystem->GetVisionMap()->ObserverChanged(id_, params_, EChangeHint::eChangedAll);
}

void AIObserver::visibility_status_change(
		const VisionID& observer_id
	,	const ObserverParams& observer_params
	,	const VisionID& observable_id
	,	const ObservableParams& observable_params
	,	bool is_visible
)
{
	if (const auto blackboard = DevelopmentTools::get_blackboard(GetEntityId())) {
		if (is_visible) {
			DevelopmentTools::set_value_in_blackboard("enemy_id", *blackboard, observable_params.entityId);
			DevelopmentTools::set_value_in_blackboard("can_see_enemy", *blackboard, true);
			BoomerShooterEventSystem::instance().send(GetEntityId(), AlertEvent{});
		}
		else
			DevelopmentTools::set_value_in_blackboard("can_see_enemy", *blackboard, false);
	}
}