#ifndef RETRY_GAME_LISTENER_H
#define RETRY_GAME_LISTENER_H

#include <CrySystem/Scaleform/IFlashUI.h>

#include <string_view>

using namespace std::string_view_literals;

namespace UIListeners {
	struct RetryGameListener final : public IUIElementEventListener
	{
		void OnUIEvent(IUIElement* pSender, const SUIEventDesc& event, const SUIArguments& args) override;
	
		static constexpr auto event_name_ = "retry_game_listener"sv;
	};
}	// namespace UIListeners

#endif // !RETRY_GAME_LISTENER_H