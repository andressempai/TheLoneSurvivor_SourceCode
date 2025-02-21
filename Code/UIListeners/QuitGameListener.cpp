#include "UIListeners/QuitGameListener.h"

#include <CrySystem/IConsole.h>

namespace UIListeners {
	void QuitGameListener::OnUIEvent(IUIElement* pSender, const SUIEventDesc& event, const SUIArguments& args)
	{
		if (strcmp(event.sName, "QuitGame") == 0)
			gEnv->pConsole->ExecuteString("quit");
	}
}	// namespace UIListeners