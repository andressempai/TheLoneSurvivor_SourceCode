#include "AIListener.h"
#include "../../DevelopmentTools.h"

#include <CrySchematyc/Env/Elements/EnvComponent.h>
#include <CrySchematyc/Env/IEnvRegistrar.h>
#include <CryCore/StaticInstanceList.h>
#include <CryAISystem/IAISystem.h>
#include <CryAISystem/IFactionMap.h>

void register_ai_listener(Schematyc::IEnvRegistrar& registrar)
{
	registrar.Scope(IEntity::GetEntityScopeGUID()).Register(SCHEMATYC_MAKE_ENV_COMPONENT(AIListener));
}

CRY_STATIC_AUTO_REGISTER_FUNCTION(&register_ai_listener);

void ReflectType(Schematyc::CTypeDesc<AIListener>& desc)
{
	desc.SetGUID("{64F6D85C-6870-491A-B38D-98B6D8DC266C}"_cry_guid);
	desc.SetEditorCategory("AI Components");
	desc.SetName(Schematyc::CTypeName("ailistener"));
	desc.SetLabel("AI Listener");
	desc.SetDescription("Allow Entity to listen Other Entities.");
	desc.SetComponentFlags(EEntityComponentFlags::Singleton);
}

void AIListener::Initialize()
{
	params_.listeningDistanceScale = 1.0f;
	params_.factionsToListenMask = 7;
	params_.onSoundHeardCallback = functor(*this, &AIListener::on_heard_sound);
	params_.ears.push_back(Perception::SListenerEarParams{ GetWorldTransformMatrix().GetColumn3() });

	gEnv->pAISystem->GetAuditionMap()->RegisterListener(GetEntityId(), params_);
}

Cry::Entity::EventFlags AIListener::GetEventMask() const
{
	return Cry::Entity::EEvent::Update;
}

void AIListener::ProcessEvent(const SEntityEvent& event)
{
	if (gEnv->IsEditing())
		return;
	
	params_.ears[0] = Perception::SListenerEarParams{ GetWorldTransformMatrix().GetColumn3() };

	gEnv->pAISystem->GetAuditionMap()->ListenerChanged(
			GetEntityId()
		,	params_
		,	Perception::ListenerParamsChangeOptions::ChangedAll
	);
}

void AIListener::on_heard_sound(const Perception::SSoundStimulusParams& stimulus)
{
	const auto loud_entity = DevelopmentTools::search_entity_by_id(stimulus.sourceEntityId);
	CryWarning(VALIDATOR_MODULE_3DENGINE, VALIDATOR_WARNING, "Loud Entity is: %s", loud_entity->GetName());
}