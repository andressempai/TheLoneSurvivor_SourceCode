#include "InputComponent.h"

#include <CrySchematyc/Env/Elements/EnvComponent.h>
#include <CrySchematyc/Env/IEnvRegistrar.h>
#include <CryCore/StaticInstanceList.h>
#include <CryGame/IGameFramework.h>
#include <IActionMapManager.h>

void register_input_component(Schematyc::IEnvRegistrar& registrar)
{
	registrar.Scope(IEntity::GetEntityScopeGUID()).Register(SCHEMATYC_MAKE_ENV_COMPONENT(InputComponent));
}

CRY_STATIC_AUTO_REGISTER_FUNCTION(&register_input_component);

void ReflectType(Schematyc::CTypeDesc<InputComponent>& desc)
{
	desc.SetGUID("{63745130-150A-42B4-BE06-D9B2F31807E6}"_cry_guid);
	desc.SetEditorCategory("Game Components");
	desc.SetName(Schematyc::CTypeName("inputcomponent"));
	desc.SetLabel("Input Component");
	desc.SetDescription("Use this component only to Initialize the Action Map");
	desc.SetComponentFlags(EEntityComponentFlags::Singleton);
}

void InputComponent::Initialize()
{
	action_map_manager_ = gEnv->pGameFramework->GetIActionMapManager();
	action_map_manager_->InitActionMaps("libs/config/defaultprofile.xml");
	action_map_manager_->Enable(true);
	action_map_manager_->EnableActionMap("player", true);
}

void InputComponent::OnShutDown()
{
	if (action_map_manager_)
		action_map_manager_->Clear();
}