#ifndef RESUME_GAME_LISTENER_H
#define RESUME_GAME_LISTENER_H

#include <CrySystem/Scaleform/IFlashUI.h>

#include <string_view>

using namespace std::string_view_literals;

namespace UIListeners {
	struct ResumeGameListener final : public IUIElementEventListener
	{
		void OnUIEvent(IUIElement* pSender, const SUIEventDesc& event, const SUIArguments& args) override;
	
		static constexpr auto event_name_ = "resume_game_listener"sv;
	};
}	// namespace UIListeners

#endif // !RESUME_GAME_LISTENER_H