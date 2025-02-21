#include "Footstep.h"
#include "../../DevelopmentTools.h"

#include <CrySchematyc/Env/Elements/EnvComponent.h>
#include <CrySchematyc/Env/IEnvRegistrar.h>
#include <CryCore/StaticInstanceList.h>

static void register_footstep_component(Schematyc::IEnvRegistrar& registrar)
{
	registrar.Scope(IEntity::GetEntityScopeGUID()).Register(SCHEMATYC_MAKE_ENV_COMPONENT(Footstep));
}

CRY_STATIC_AUTO_REGISTER_FUNCTION(&register_footstep_component)

void ReflectType(Schematyc::CTypeDesc<Footstep>& desc)
{
	desc.SetGUID("{D3972428-70AF-40E2-B839-E8A0319EEAB8}"_cry_guid);
	desc.SetEditorCategory("Game Components");
	desc.SetName(Schematyc::CTypeName("footstepcomponent"));
	desc.SetLabel("Footstep Component");
	desc.SetDescription("Use this component to start footstep effects (Sound, Particles, Decals, etc)");
	desc.SetComponentFlags(EEntityComponentFlags::Singleton);
}

Cry::Entity::EventFlags Footstep::GetEventMask() const
{
	return Cry::Entity::EEvent::AnimationEvent;
}

void Footstep::ProcessEvent(const SEntityEvent& event)
{
	if (gEnv->IsEditing() or m_pEntity->IsGarbage())
		return;

	const auto* const animation_event = reinterpret_cast<const AnimEventInstance*>(event.nParam[0]);
	
	if (stricmp(animation_event->m_EventName, "footstep") == 0) {
		auto runtime_effects_params = SMFXRunTimeEffectParams{};
		runtime_effects_params.pos = m_pEntity->GetWorldPos();
		runtime_effects_params.audioProxyEntityId = GetEntityId();
		
		const auto living_status = DevelopmentTools::get_physics_status<pe_status_living>(*m_pEntity);

		if (const auto effect_id = DevelopmentTools::get_runtime_effect_id("footstep", living_status.groundSurfaceIdx))
			DevelopmentTools::execute_runtime_effect_id(effect_id, runtime_effects_params);
	}
}