#include "UIListeners/StartGameListener.h"

#include <CrySystem/IConsole.h>

namespace UIListeners {
	void StartGameListener::OnUIEvent(IUIElement* pSender, const SUIEventDesc& event, const SUIArguments& args)
	{
		if (strcmp(event.sName, "StartGame") == 0)
			gEnv->pConsole->ExecuteString("map village", false, true);
	}
}	// namespace UIListeners