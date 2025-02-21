#include "Components/DynamicMesh/DynamicMesh.h"
#include "DevelopmentTools.h"

#include <CrySchematyc/Env/IEnvRegistrar.h>
#include <CrySchematyc/Env/Elements/EnvComponent.h>
#include <CryCore/StaticInstanceList.h>

void register_dynamic_mesh_component(Schematyc::IEnvRegistrar& registrar)
{
	registrar.Scope(IEntity::GetEntityScopeGUID()).Register(SCHEMATYC_MAKE_ENV_COMPONENT(DynamicMesh));
}

CRY_STATIC_AUTO_REGISTER_FUNCTION(&register_dynamic_mesh_component);

void ReflectType(Schematyc::CTypeDesc<DynamicMesh>& desc)
{
	desc.SetGUID("{A04B3B3A-7464-472F-802C-1918AE313082}"_cry_guid);
	desc.SetEditorCategory("Game Components");
	desc.SetName(Schematyc::CTypeName("dynamicmeshcomponent"));
	desc.SetLabel("Dynamic Mesh");
	desc.SetDescription("Dynamic Mesh like Static and Rigid Body Objects");
	desc.SetComponentFlags({ EEntityComponentFlags::Singleton, EEntityComponentFlags::Transform });
	desc.AddMember(
			&DynamicMesh::type_
		,	'dtyp'
		,	"dynamictype"
		,	"Dynamic Type"
		,	"Type of the Dynamic Mesh (Static or Rigid)"
		,	EDynamicMeshType::Static
	);

	desc.AddMember(
			&DynamicMesh::mesh_params_
		,	'dmmp'
		,	"meshparams"
		,	"Mesh Params"
		,	"Mesh Params"
		,	SMeshParams{}
	);

	desc.AddMember(
			&DynamicMesh::simulation_params_
		,	'dmsp'
		,	"simulationparams"
		,	"Simulation Params"
		,	"Simulation Params for this Dynamic Mesh"
		,	SSimulationParams{}
	);

	desc.AddMember(
			&DynamicMesh::can_cast_shadow_
		,	'dmcs'
		,	"cancastshadow"
		,	"Can Cast Shadow"
		,	"Allow Entity to Cast Shadow or not"
		,	true
	);
}

void DynamicMesh::Initialize()
{
	slot_ = GetOrMakeEntitySlotId();

	load_mesh();
}

Cry::Entity::EventFlags DynamicMesh::GetEventMask() const
{
	return Cry::Entity::EEvent::EditorPropertyChanged;
}

void DynamicMesh::ProcessEvent(const SEntityEvent& event)
{
	load_mesh();
}

void DynamicMesh::physicalize()
{
	DevelopmentTools::destroy_physics(*m_pEntity, slot_);

	auto physicalize_params = SEntityPhysicalizeParams{};
	physicalize_params.type = DevelopmentTools::get_underlying_value_from_enum(type_);
	physicalize_params.nSlot = slot_;
	physicalize_params.nFlagsOR = pef_ignore_areas;
	DevelopmentTools::create_physics(*m_pEntity, physicalize_params);

	auto simulation = static_cast<pe_simulation_params>(simulation_params_);
	DevelopmentTools::add_complemetary_physics_params(*m_pEntity, simulation);
}

void DynamicMesh::load_mesh()
{
	if (mesh_params_.geometry_file_path_.value.empty())
		return;

	DevelopmentTools::load_geometry(*m_pEntity, slot_, mesh_params_.geometry_file_path_);
	DevelopmentTools::set_shadow_casting(*m_pEntity, can_cast_shadow_);
	physicalize();

	if (const auto render_node = DevelopmentTools::get_render_node(*m_pEntity, slot_)) {
		m_pEntity->AddFlags(ENTITY_FLAG_CUSTOM_VIEWDIST_RATIO);
		render_node->SetLodRatio(mesh_params_.lod_ratio_.value);
		render_node->SetViewDistRatio(mesh_params_.view_distance_ratio_.value);
	}

	if (mesh_params_.material_file_path_.value.empty())
		return;

	DevelopmentTools::load_material(*m_pEntity, slot_, mesh_params_.material_file_path_);
}