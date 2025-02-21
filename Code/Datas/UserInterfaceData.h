#ifndef UI_ELEMENT_DATA_H
#define UI_ELEMENT_DATA_H

#include <string_view>

using namespace std::string_view_literals;

namespace Data {
	struct MainMenu {
		static constexpr auto ui_element_name_ = "main_menu"sv;
	};

	struct PlayerHUD {
		static constexpr auto ui_element_name_ = "player_hud"sv;
	};

	struct PauseMenu {
		static constexpr auto ui_element_name_ = "pause_menu"sv;
	};

	struct DeadMenu {
		static constexpr auto ui_element_name_ = "dead_menu"sv;
	};
}

#endif // !UI_ELEMENT_DATA_H