#include "UIListeners/RetryGameListener.h"

#include <CrySystem/IConsole.h>

namespace UIListeners {
	void RetryGameListener::OnUIEvent(IUIElement* pSender, const SUIEventDesc& event, const SUIArguments& args)
	{
		if (strcmp(event.sName, "RetryGame") == 0)
			gEnv->pConsole->ExecuteString("map village", false, true);
	}
}	// namespace UIListeners