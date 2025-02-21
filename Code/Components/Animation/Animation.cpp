#include "Animation.h"
#include "../../DevelopmentTools.h"

#include <CrySchematyc/Env/IEnvRegistrar.h>
#include <CrySchematyc/Env/Elements/EnvComponent.h>
#include <CryCore/StaticInstanceList.h>

void register_animation_component(Schematyc::IEnvRegistrar& registrar)
{
	registrar.Scope(IEntity::GetEntityScopeGUID()).Register(SCHEMATYC_MAKE_ENV_COMPONENT(Animation));
}

CRY_STATIC_AUTO_REGISTER_FUNCTION(&register_animation_component);

void ReflectType(Schematyc::CTypeDesc<Animation>& desc)
{
	desc.SetGUID("{BBD9AC12-5670-413A-B589-9639E87E26AE}"_cry_guid);
	desc.SetEditorCategory("Game Components");
	desc.SetName(Schematyc::CTypeName("animationcomponent"));
	desc.SetLabel("Animation");
	desc.SetDescription("This component is used to animate entities.");
	desc.SetComponentFlags({ EEntityComponentFlags::Singleton });
}

bool Serialize(
		Serialization::IArchive& archive
	,	Animation::AnimationData& data
	,	const char* name
	,	const char* label
)
{
	archive(
			Serialization::MannequinControllerDefinitionPath(data.controller_definition_path_)
		,	"controllerdefinitionpath"
		,	"Controller Definition Path"
	);
	archive.doc("The path to the controller definition file.");

	archive(
			Serialization::MannequinAnimationDatabasePath(data.animation_database_path_)
		,	"animationdatabasepath"
		,	"Animation Database Path"
	);
	archive.doc("The path to the animation database file.");

	auto resource_params = std::make_shared<Serialization::SMannequinControllerDefResourceParams>();

	if (archive.isEdit()) {
		if (data.controller_definition_path_.size()) {
			using namespace DevelopmentTools;
			resource_params->pControllerDef = load_controller_definition(data.controller_definition_path_.c_str());
		}
	}

	std::Serialize(archive, data.scope_contexts_selector_, "scopecontexts", "Scope Contexts");
	archive.doc("Scope Context Container");

	for (auto& value : data.scope_contexts_selector_)
		value.pCustomParams = resource_params;

	return true;
}

void Animation::Initialize()
{
	using namespace DevelopmentTools;

	OnShutDown();

	if (animation_data_.controller_definition_path_.empty() or animation_data_.animation_database_path_.empty())
		return;

	controller_definition_ = load_controller_definition(animation_data_.controller_definition_path_.c_str());
	if (not controller_definition_)
		return;

	animation_database_ = load_animation_data_base(animation_data_.animation_database_path_.c_str());
	if (not animation_database_)
		return;

	animation_context_ = create_animation_context(*controller_definition_);
	if (not animation_context_)
		return;

	action_controller_ = create_action_controller(m_pEntity, *animation_context_);
	if (not action_controller_)
		return;

	for (const auto& scope_context : animation_data_.scope_contexts_selector_) {
		if (scope_context.value_.empty())
			break;

		const auto scope_context_id = find_scope_context(*controller_definition_, scope_context.value_.c_str());
		action_controller_->SetScopeContext(
				scope_context_id
			,	*m_pEntity
			,	get_character_from_entity(*m_pEntity, 0)
			,	animation_database_
		);
	}

	drive_motion_by_animation(*m_pEntity, 0, animation_data_.animation_drive_motion_);
	initialize_fragment();
}

void Animation::OnShutDown()
{
	if (animation_context_)
		DevelopmentTools::destroy_animation_context(animation_context_);

	if (action_controller_)
		DevelopmentTools::destroy_action_controller(action_controller_);
}

Cry::Entity::EventFlags Animation::GetEventMask() const
{
	return Cry::Entity::EEvent::EditorPropertyChanged | Cry::Entity::EEvent::Update;
}

void Animation::ProcessEvent(const SEntityEvent& event)
{
	switch (event.event) {
	case Cry::Entity::EEvent::EditorPropertyChanged:
		Initialize();
		break;
	case Cry::Entity::EEvent::Update:
		if (action_controller_)
			action_controller_->Update(event.fParam[0]);
		break;
	}
}