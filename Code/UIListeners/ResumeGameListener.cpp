#include "UIListeners/ResumeGameListener.h"
#include "DevelopmentTools.h"

#include <CrySystem/IConsole.h>

namespace UIListeners {
	void ResumeGameListener::OnUIEvent(IUIElement* pSender, const SUIEventDesc& event, const SUIArguments& args)
	{
		if (strcmp(event.sName, "ResumeGame") == 0) {
			DevelopmentTools::resume_game();
			DevelopmentTools::enable_action_map("player");
			DevelopmentTools::set_ui_element_visible(*pSender, false);
		}
	}
}	// namespace UIListeners