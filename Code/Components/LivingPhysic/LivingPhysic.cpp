#include "Components/LivingPhysic/LivingPhysic.h"
#include "DevelopmentTools.h"

#include <CrySchematyc/Env/Elements/EnvComponent.h>
#include <CrySchematyc/Env/IEnvRegistrar.h>
#include <CryCore/StaticInstanceList.h>

void register_living_physic_component(Schematyc::IEnvRegistrar& registrar)
{
	registrar.Scope(IEntity::GetEntityScopeGUID()).Register(SCHEMATYC_MAKE_ENV_COMPONENT(LivingPhysic));
}

CRY_STATIC_AUTO_REGISTER_FUNCTION(&register_living_physic_component)

void ReflectType(Schematyc::CTypeDesc<LivingPhysic>& desc)
{
	desc.SetGUID("{2EBF40E2-D604-428D-AD97-02B582CCAD6A}"_cry_guid);
	desc.SetEditorCategory("Game Components");
	desc.SetName(Schematyc::CTypeName("livingphysiccomponent"));
	desc.SetLabel("Living Physic");
	desc.SetDescription("Physical Entity for Living type Entities");
	desc.SetComponentFlags({ EEntityComponentFlags::Singleton, EEntityComponentFlags::Transform });
	desc.AddMember(
			&LivingPhysic::dimensions_
		,	'dime'
		,	"dimensions"
		,	"Dimensions"
		,	"Character Dimensions"
		,	SCharacterDimensions{}
	);

	desc.AddMember(
			&LivingPhysic::dynamics_
		,	'dyna'
		,	"dynamics"
		,	"Dynamics"
		,	"Character Dynamics"
		,	SCharacterDynamics{}
	);
}

void LivingPhysic::Initialize()
{
	slot_ = GetOrMakeEntitySlotId();

	OnShutDown();

	auto dimensions = static_cast<pe_player_dimensions>(dimensions_);
	auto dynamics = static_cast<pe_player_dynamics>(dynamics_);

	auto physicalize_params = SEntityPhysicalizeParams{};
	physicalize_params.type = PE_LIVING;
	physicalize_params.nSlot = slot_;
	physicalize_params.mass = dynamics.mass;
	physicalize_params.nLod = 1;
	physicalize_params.pPlayerDimensions = &dimensions;
	physicalize_params.pPlayerDynamics = &dynamics;
	physicalize_params.nFlagsOR = std::invoke([this]()
	{
		int flag = pef_monitor_poststep;

		if (dynamics_.can_use_custom_gravity_)
			flag |= pef_ignore_areas;

		return flag;
	});

	DevelopmentTools::create_physics(*m_pEntity, physicalize_params);
}

void LivingPhysic::OnShutDown()
{
	DevelopmentTools::destroy_physics(*m_pEntity, slot_);
}

Cry::Entity::EventFlags LivingPhysic::GetEventMask() const
{
	return Cry::Entity::EEvent::EditorPropertyChanged;
}

void LivingPhysic::ProcessEvent(const SEntityEvent& event)
{
	Initialize();
}

#ifndef RELEASE
void LivingPhysic::Render(const IEntity& entity, const IEntityComponent& component, SEntityPreviewContext& context) const
{
	if (const auto physical_entity = DevelopmentTools::get_physics_from_entity(entity)) {
		pe_params_part part_params{};
		part_params.ipart = 0;

		physical_entity->GetParams(&part_params);

		const auto& transform_matrix = entity.GetWorldTM();
		
		geom_world_data geom_data{};
		geom_data.R = Matrix33{ Quat(transform_matrix) * part_params.q };
		geom_data.scale = transform_matrix.GetUniformScale() * part_params.scale;
		geom_data.offset = transform_matrix.GetTranslation() + transform_matrix.TransformVector(part_params.pos);

		gEnv->pSystem->GetIPhysRenderer()->DrawGeometry(
				part_params.pPhysGeom->pGeom
			,	&geom_data
			,	-1
			,	0
			,	ZERO
			,	context.debugDrawInfo.color
		);
	}
}
#endif // !RELEASE