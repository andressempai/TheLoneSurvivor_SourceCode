#include "SkeletalMesh.h"
#include "../../DevelopmentTools.h"

#include <CrySchematyc/Env/IEnvRegistrar.h>
#include <CrySchematyc/Env/Elements/EnvComponent.h>
#include <CryCore/StaticInstanceList.h>

void register_skeletal_mesh_component(Schematyc::IEnvRegistrar& registrar)
{
	registrar.Scope(IEntity::GetEntityScopeGUID()).Register(SCHEMATYC_MAKE_ENV_COMPONENT(SkeletalMesh));
}

CRY_STATIC_AUTO_REGISTER_FUNCTION(&register_skeletal_mesh_component);

void ReflectType(Schematyc::CTypeDesc<SkeletalMesh>& desc)
{
	desc.SetGUID("{A54EE02E-DD5A-4034-8378-777C4119E134}"_cry_guid);
	desc.SetEditorCategory("Game Components");
	desc.SetName(Schematyc::CTypeName("skeletalmeshcomponent"));
	desc.SetLabel("Skeletal Mesh");
	desc.SetDescription("Initialize Character Instance inside Entity");
	desc.SetComponentFlags({ EEntityComponentFlags::Singleton, EEntityComponentFlags::Transform });
	desc.AddMember(
			&SkeletalMesh::character_file_path_
		,	'char'
		,	"CharacterFile"
		,	"Character File"
		,	"The file path to the character file"
		,	Schematyc::CharacterFileName{}
	);

	desc.AddMember(
			&SkeletalMesh::material_file_path_
		,	'mat'
		,	"MaterialFile"
		,	"Material File"
		,	"The file path to the material file"
		,	Schematyc::MaterialFileName{}
	);
}

void SkeletalMesh::Initialize()
{
	calculate_slot();
	load_character_instance();
	shadow_casting();
	physicalize();
}

void SkeletalMesh::OnShutDown()
{
	DevelopmentTools::destroy_physics(*m_pEntity, slot_);
}

Cry::Entity::EventFlags SkeletalMesh::GetEventMask() const
{
	return Cry::Entity::EEvent::EditorPropertyChanged;
}

void SkeletalMesh::ProcessEvent(const SEntityEvent& event)
{
	Initialize();
}

void SkeletalMesh::calculate_slot()
{
	slot_ = GetOrMakeEntitySlotId();
}

void SkeletalMesh::load_character_instance()
{
	DevelopmentTools::create_character(*m_pEntity, slot_, character_file_path_);
	DevelopmentTools::load_material(*m_pEntity, slot_, material_file_path_);
	SetTransformMatrix(transform_offset_);
}

void SkeletalMesh::shadow_casting()
{
	DevelopmentTools::set_shadow_casting(*m_pEntity, can_cast_shadow_);
}

void SkeletalMesh::physicalize()
{
	OnShutDown();

	SEntityPhysicalizeParams params{};
	params.type = PE_STATIC;
	params.nSlot = slot_;
	params.mass = mass_;
	DevelopmentTools::create_physics(*m_pEntity, params);

	pe_simulation_params sim_params{};
	sim_params.maxTimeStep = 0.02f;
	sim_params.minEnergy = 0.004f;
	sim_params.mass = mass_;

	auto collision_class_params = DevelopmentTools::get_physics_params<pe_params_collision_class>(*m_pEntity);
	collision_class_params.collisionClassOR.ignore = collision_class_living;

	DevelopmentTools::add_complemetary_physics_params(*m_pEntity, sim_params, collision_class_params);
}