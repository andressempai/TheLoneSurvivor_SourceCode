#ifndef USER_INTERFACE_TYPE_H
#define USER_INTERFACE_TYPE_H

#include "Components/UserInterface.h"
#include "GameEvents/UI/SubscribeToMagazineUpdateEvent.h"
#include "GameEvents/UI/SubscribeToHealthUpdateEvent.h"
#include "GameEvents/UI/SubscribeToArmorUpdateEvent.h"
#include "UIListeners/QuitGameListener.h"
#include "UIListeners/ResumeGameListener.h"
#include "UIListeners/RetryGameListener.h"
#include "UIListeners/ReturnMainMenuListener.h"
#include "UIListeners/StartGameListener.h"
#include "Datas/UserInterfaceData.h"
#include "Datas/ComponentsData.h"

namespace Types::Components {
	using ComponentSubscribeToNothing = MP::type_list<>;
	using EmptyUIListener = MP::type_list<>;
	using MainMenuUIEventListeners = MP::type_list<
			UIListeners::StartGameListener
		,	UIListeners::QuitGameListener
	>;
	using MainMenu = UserInterface<
			Data::MainMenu
		,	ComponentData::MainMenu
		,	ComponentSubscribeToNothing
		,	MainMenuUIEventListeners
	>;

	using PlayerHUDEventsToSubscribeList = MP::type_list<
			GameEvents::UI::SubscribeToMagazineUpdateEvent
		,	GameEvents::UI::SubscribeToHealthUpdateEvent
		,	GameEvents::UI::SubscribeToArmorUpdateEvent
	>;
	using PlayerHUD = UserInterface<
			Data::PlayerHUD
		,	ComponentData::PlayerHUD
		,	PlayerHUDEventsToSubscribeList
		,	EmptyUIListener
	>;

	using PauseMenuEventListener = MP::type_list<
			UIListeners::ResumeGameListener
		,	UIListeners::ReturnMainMenuListener
		,	UIListeners::QuitGameListener
	>;
	using PauseMenu = UserInterface<
			Data::PauseMenu
		,	ComponentData::PauseMenu
		,	ComponentSubscribeToNothing
		,	PauseMenuEventListener
	>;

	using DeadMenuEventListener = MP::type_list<
			UIListeners::RetryGameListener
		,	UIListeners::ReturnMainMenuListener
		,	UIListeners::QuitGameListener
	>;
	using DeadMenu = UserInterface<
			Data::DeadMenu
		,	ComponentData::DeadMenu
		,	ComponentSubscribeToNothing
		,	DeadMenuEventListener
	>;
}	// namespace Types::Components

#endif // !USER_INTERFACE_TYPE_H