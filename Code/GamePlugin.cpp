#include "StdAfx.h"
#include "GamePlugin.h"
#include "TimerManager.h"
#include "Types/WeaponTypes.h"
#include "Types/InventoryTypes.h"
#include "Types/AmmunitionTypes.h"
#include "Types/BodyProxyTypes.h"
#include "Types/AnimationType.h"
#include "BehaviorTreeNodes/BehaviorTreeNodesRegistrator.h"
#include "Types/UserInterfaceType.h"

#include <IGameObjectSystem.h>
#include <IGameObject.h>
#include <CrySchematyc/Env/IEnvRegistry.h>
#include <CrySchematyc/Env/EnvPackage.h>
#include <CrySchematyc/Utils/SharedString.h>
#include <CryCore/Platform/platform_impl.inl>
#include <CrySystem/ConsoleRegistration.h>

CGamePlugin::~CGamePlugin()
{
	gEnv->pSystem->GetISystemEventDispatcher()->RemoveListener(this);

	if (gEnv->pSchematyc)
		gEnv->pSchematyc->GetEnvRegistry().DeregisterPackage(CGamePlugin::GetCID());

	gEnv->pConsole->UnregisterVariable("dv_DebugRaycast");
	gEnv->pConsole->UnregisterVariable("dv_DebugTrigger");
}

bool CGamePlugin::Initialize(SSystemGlobalEnvironment& env, const SSystemInitParams& initParams)
{
	gEnv->pSystem->GetISystemEventDispatcher()->RegisterListener(this, "CGamePlugin");

	EnableUpdate(EUpdateStep::BeforeRender, true);

	REGISTER_INT("dv_DebugRaycast", 0, VF_NULL, "Debug Raycast");
	REGISTER_INT("dv_DebugTrigger", 0, VF_NULL, "Debug Triggers");

	return true;
}

void CGamePlugin::UpdateBeforeRender()
{
	const auto frame_time = gEnv->pTimer->GetFrameTime();
	auto& timer_manager = TimerManager::instance();
	
	timer_manager.consume_all_timers(frame_time);
	
	static constexpr auto remover_timer_interval = 2.0;
	static auto time_before_remove_finished_timer = remover_timer_interval;
	if (time_before_remove_finished_timer -= frame_time; time_before_remove_finished_timer <= 0.0) {
		time_before_remove_finished_timer = remover_timer_interval;
	
		timer_manager.prepare_for_remove_finished_timers();
		timer_manager.remove_finished_timers();
	}
}

void CGamePlugin::OnSystemEvent(ESystemEvent event, UINT_PTR wparam, UINT_PTR lparam)
{
	switch (event) {
		case ESYSTEM_EVENT_GAME_POST_INIT:
			if (not gEnv->IsEditor())
				gEnv->pConsole->ExecuteString("map main_menu", false, true);
		break;

		case ESYSTEM_EVENT_REGISTER_SCHEMATYC_ENV:
		{
			auto staticAutoRegisterLambda = [](Schematyc::IEnvRegistrar& registrar)
			{
				Detail::CStaticAutoRegistrar<Schematyc::IEnvRegistrar&>::InvokeStaticCallbacks(registrar);

				auto scope = registrar.Scope(IEntity::GetEntityScopeGUID());
				
				Types::Components::PistolComponent::register_component(scope);
				Types::Components::AmericanShotgunComponent::register_component(scope);

				Types::Components::WeaponInventoryComponent::register_component(scope);

				Types::Components::PistolAmmunitionComponent::register_component(scope);
				Types::Components::ShotgunAmmunitionComponent::register_component(scope);

				Types::Components::PlayerBodyProxyComponent::register_component(scope);
				Types::Components::ZombieBodyProxyComponent::register_component(scope);
				Types::Components::PerderosBodyProxyComponent::register_component(scope);

				Types::Components::PlayerAnimationComponent::register_component(scope);
				Types::Components::PistolAnimationComponent::register_component(scope);
				Types::Components::ShotgunAnimationComponent::register_component(scope);

				Types::Components::MainMenu::register_component(scope);
				Types::Components::PlayerHUD::register_component(scope);
				Types::Components::PauseMenu::register_component(scope);
				Types::Components::DeadMenu::register_component(scope);

				register_behavior_tree_nodes();
			};

			if (gEnv->pSchematyc) {
				gEnv->pSchematyc->GetEnvRegistry().RegisterPackage(
					stl::make_unique<Schematyc::CEnvPackage>(
							CGamePlugin::GetCID()
						,	"EntityComponents"
						,	"Crytek GmbH"
						,	"Components"
						,	staticAutoRegisterLambda
					)
				);
			}
		}
		break;
		
		case ESYSTEM_EVENT_LEVEL_UNLOAD_START:
		{
			TimerManager::instance().clear();
		}
		break;
	}
}

CRYREGISTER_SINGLETON_CLASS(CGamePlugin)