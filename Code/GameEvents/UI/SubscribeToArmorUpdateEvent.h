#ifndef SUBSCRIBE_TO_ARMOR_UPDATE_EVENT_H
#define SUBSCRIBE_TO_ARMOR_UPDATE_EVENT_H

#include <functional>

struct IUIElement;

using EntityId = unsigned int;

namespace GameEvents::UI {
	struct SubscribeToArmorUpdateEvent {
		explicit SubscribeToArmorUpdateEvent(IUIElement& ui_element);
		void subcribe_to_event(EntityId id);
		void unscribe_to_event(EntityId id);

		std::reference_wrapper<IUIElement> ui_element_;
	};
}	// namespace GameEvents::UI

#endif // !SUBSCRIBE_TO_ARMOR_UPDATE_EVENT_H