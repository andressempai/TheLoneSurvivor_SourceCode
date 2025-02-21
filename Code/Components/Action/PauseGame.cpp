#include "PauseGame.h"
#include "Types/UserInterfaceType.h"
#include "DevelopmentTools.h"

#include <CrySchematyc/Env/Elements/EnvComponent.h>
#include <CrySchematyc/Env/IEnvRegistrar.h>
#include <CryCore/StaticInstanceList.h>

void register_pause_game_action_component(Schematyc::IEnvRegistrar& registrar)
{
	registrar.Scope(IEntity::GetEntityScopeGUID()).Register(SCHEMATYC_MAKE_ENV_COMPONENT(PauseGame));
}

CRY_STATIC_AUTO_REGISTER_FUNCTION(&register_pause_game_action_component);

void ReflectType(Schematyc::CTypeDesc<PauseGame>& desc)
{
	desc.SetGUID("{A2A1F566-F7E6-44FA-AE99-D84860121273}"_cry_guid);
	desc.SetEditorCategory("Game Actions");
	desc.SetName(Schematyc::CTypeName("pausegameactioncomponent"));
	desc.SetLabel("Pause Game");
	desc.SetDescription("Allow Entity to Pause Game");
	desc.SetComponentFlags(EEntityComponentFlags::Singleton);
}

void PauseGame::Initialize()
{
	DevelopmentTools::add_action_listener(this, "player");
}

void PauseGame::OnAction(const ActionId& action, int activation_mode, float value)
{
	using namespace DevelopmentTools;

	if (activation_mode != eAAM_OnPress)
		return;

	if ((strcmp(action.c_str(), "pause_game") == 0)) {
		if (const auto pause_entity = search_entity_by_name("pause_entity")) {
			if (const auto pause_menu = get_component_from_entity<Types::Components::PauseMenu>(*pause_entity)) {
				pause_menu->show_ui_element();
				pause_game();
				disable_action_map("player");
			}
		}
	}
}