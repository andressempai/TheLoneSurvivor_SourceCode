#ifndef RETURN_TO_MAIN_MENU_LISTENER_H
#define RETURN_TO_MAIN_MENU_LISTENER_H

#include <CrySystem/Scaleform/IFlashUI.h>

#include <string_view>

using namespace std::string_view_literals;

namespace UIListeners {
	struct ReturnMainMenuListener final : public IUIElementEventListener
	{
		void OnUIEvent(IUIElement* pSender, const SUIEventDesc& event, const SUIArguments& args) override;
	
		static constexpr auto event_name_ = "return_main_menu_listener"sv;
	};
}	// namespace UIListeners

#endif // !RETURN_TO_MAIN_MENU_LISTENER_H