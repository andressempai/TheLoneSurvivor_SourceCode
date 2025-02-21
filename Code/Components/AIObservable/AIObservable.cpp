#include "AIObservable.h"
#include "../../DevelopmentTools.h"

#include <CrySchematyc/Env/Elements/EnvComponent.h>
#include <CrySchematyc/Env/IEnvRegistrar.h>
#include <CryCore/StaticInstanceList.h>
#include <CryAISystem/IAISystem.h>
#include <CryAISystem/VisionMapTypes.h>
#include <CryAISystem/IFactionMap.h>

void register_ai_observable_component(Schematyc::IEnvRegistrar& registrar)
{
	registrar.Scope(IEntity::GetEntityScopeGUID()).Register(SCHEMATYC_MAKE_ENV_COMPONENT(AIObservable));
}

CRY_STATIC_AUTO_REGISTER_FUNCTION(&register_ai_observable_component);

void ReflectType(Schematyc::CTypeDesc<AIObservable>& desc)
{
	desc.SetGUID("{78979341-023D-416F-89F1-605F170D7D0E}"_cry_guid);
	desc.SetEditorCategory("AI Components");
	desc.SetName(Schematyc::CTypeName("aiobservablecomponent"));
	desc.SetLabel("AI Observable");
	desc.SetDescription("Allow Entity to be observer by others AI Observer Entities.");
	desc.SetComponentFlags(EEntityComponentFlags::Singleton);
}

void AIObservable::Initialize()
{
	string vision_name = m_pEntity->GetName();
	vision_name.Append(".Observable");

	params_.typeMask = VisionMapTypes::Player;
	params_.faction = gEnv->pAISystem->GetFactionMap().GetFactionID("Players");
	params_.entityId = GetEntityId();
	params_.mode = EObservableMode::Default;
	
	id_ = gEnv->pAISystem->GetVisionMap()->CreateVisionID(vision_name.c_str());
	gEnv->pAISystem->GetVisionMap()->RegisterObservable(id_, params_);
}

Cry::Entity::EventFlags AIObservable::GetEventMask() const
{
	return Cry::Entity::EEvent::Update;
}

void AIObservable::ProcessEvent(const SEntityEvent& event)
{
	params_.observablePositions[0] = m_pEntity->GetWorldPos();
	params_.observablePositionsCount = 1;
	gEnv->pAISystem->GetVisionMap()->ObservableChanged(id_, params_, EChangeHint::eChangedAll);
}