#include "UIListeners/ReturnMainMenuListener.h"
#include "DevelopmentTools.h"

#include <CrySystem/IConsole.h>

namespace UIListeners {
	void ReturnMainMenuListener::OnUIEvent(IUIElement* pSender, const SUIEventDesc& event, const SUIArguments& args)
	{
		if (strcmp(event.sName, "ReturnMainMenu") == 0) {
			if (DevelopmentTools::is_game_pause())
				DevelopmentTools::resume_game();

			gEnv->pConsole->ExecuteString("map main_menu", false, true);
		}
	}
}	// namespace UIListeners