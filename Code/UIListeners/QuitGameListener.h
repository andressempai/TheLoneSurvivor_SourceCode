#ifndef QUIT_GAME_LISTENER_H
#define QUIT_GAME_LISTENER_H

#include <CrySystem/Scaleform/IFlashUI.h>

#include <string_view>

using namespace std::string_view_literals;

namespace UIListeners {
	struct QuitGameListener final : public IUIElementEventListener
	{
		void OnUIEvent(IUIElement* pSender, const SUIEventDesc& event, const SUIArguments& args) override;
	
		static constexpr auto event_name_ = "quit_game_listener"sv;
	};
}	// namespace UIListeners

#endif // !QUIT_GAME_LISTENER_H