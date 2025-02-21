#include "MuzzleFlash.h"
#include "../../DevelopmentTools.h"

#include <CrySchematyc/Env/Elements/EnvComponent.h>
#include <CrySchematyc/Env/IEnvRegistrar.h>
#include <CryCore/StaticInstanceList.h>

void register_muzzle_flash_component(Schematyc::IEnvRegistrar& registrar)
{
	registrar.Scope(IEntity::GetEntityScopeGUID()).Register(SCHEMATYC_MAKE_ENV_COMPONENT(MuzzleFlash));
}

CRY_STATIC_AUTO_REGISTER_FUNCTION(&register_muzzle_flash_component);

void ReflectType(Schematyc::CTypeDesc<MuzzleFlash>& desc)
{
	desc.SetGUID("{6798C3DF-C205-4E12-BEE6-0A9D71720C07}"_cry_guid);
	desc.SetEditorCategory("Game Components");
	desc.SetName(Schematyc::CTypeName("muzzleflashcomponent"));
	desc.SetLabel("Muzzle Flash");
	desc.SetDescription("Muzzle Flash's Particle Emmiter");
	desc.SetComponentFlags({ EEntityComponentFlags::Singleton, EEntityComponentFlags::Transform });
	desc.AddMember(
			&MuzzleFlash::particle_effect_name_
		,	'pfxn'
		,	"particleeffectname"
		,	"Particle Effect Name"
		,	"Particle Effect Name"
		,	Schematyc::ParticleEffectName{}
	);
}

void MuzzleFlash::Initialize()
{
	if (const auto particle_effect = DevelopmentTools::find_particle_effect(particle_effect_name_))
		slot_ = DevelopmentTools::load_particle_emmiter(*m_pEntity, GetOrMakeEntitySlotId(), particle_effect);
}

void MuzzleFlash::OnShutDown()
{
}

Cry::Entity::EventFlags MuzzleFlash::GetEventMask() const
{
	return Cry::Entity::EEvent::Update;
}

void MuzzleFlash::ProcessEvent(const SEntityEvent& event)
{
	if (const auto particle_emmiter = DevelopmentTools::get_particle_emmiter(*m_pEntity, slot_)) {
		const auto character = DevelopmentTools::get_character_from_entity(*m_pEntity, 0);
		if (not character)
			return;

		if (const auto attachment = DevelopmentTools::get_attachment_from_character(*character, attachment_name_))
			particle_emmiter->SetLocation(attachment->GetAttWorldAbsolute());
	}
}

void MuzzleFlash::start_muzzle_flash()
{
	if (const auto particle_emmiter = DevelopmentTools::get_particle_emmiter(*m_pEntity, slot_))
		particle_emmiter->Activate(true);
}

void MuzzleFlash::stop_muzzle_flash()
{
	if (const auto particle_emmiter = DevelopmentTools::get_particle_emmiter(*m_pEntity, slot_))
		particle_emmiter->Activate(false);
}