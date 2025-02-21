#ifndef WEAPONS_DATA_H
#define WEAPONS_DATA_H

#include "../DevelopmentTools.h"

using namespace std::string_view_literals;

namespace Data {
	struct Pistol {
		static constexpr auto character_file_path_ = "objects/characters/pistol/pistol.cdf"sv;
		static constexpr auto material_file_path_ = "objects/characters/pistol/pistol.mtl"sv;
		static constexpr auto can_cast_shadow_ = false;
		static constexpr auto controller_definition_file_path_ = "animations/mannequin/adb/male_arms_controller_definitions.xml"sv;
		static constexpr auto animation_data_base_file_path_ = "animations/mannequin/adb/pistol.adb"sv;
		static constexpr auto scope_contexts_ = DevelopmentTools::make_array("PistolContext"sv);
		static constexpr auto animation_drive_motion_ = false;
		static constexpr auto animation_tag_name_ = "Pistol"sv;

		static constexpr auto mesh_position_x_ = 0.0f;
		static constexpr auto mesh_position_y_ = 0.0f;
		static constexpr auto mesh_position_z_ = 0.0f;
		static constexpr auto mesh_rotation_angle_x_ = 0.0f;
		static constexpr auto mesh_rotation_angle_y_ = 0.0f;
		static constexpr auto mesh_rotation_angle_z_ = 0.0f;
		static constexpr auto mesh_scale_x_ = 1.0f;
		static constexpr auto mesh_scale_y_ = 1.0f;
		static constexpr auto mesh_scale_z_ = 1.0f;

		static constexpr auto grip_name_ = "pistol_attachment"sv;
		static constexpr auto collected_audio_trigger_name_ = ""sv;
		static constexpr auto thumbnail_ = ""sv;

		static constexpr auto mass_ = 1.0f;

		static constexpr auto pos_shake_frequency_ = 8.5f;
		static constexpr auto rot_shake_frequency_ = 8.5f;
		static constexpr auto pos_shake_amplitude_ = 0.091f;
		static constexpr auto rot_shake_amplitude_ = 0.045f;
		static constexpr auto shake_duration_ = 0.135f;
		static constexpr auto damage_ = 13;
		static constexpr auto magazine_size_ = 30;
		static constexpr auto magazine_capacity_ = 60;
		static constexpr auto bullets_per_reload_ = magazine_capacity_;
		static constexpr auto cooldown_time_ = 0.43f;
		static constexpr auto reload_cooldown_ = 2.44f;
		static constexpr auto spread_ = 0.005f;
		static constexpr auto number_of_projectiles_ = 1;
		static constexpr auto max_distance_ = 100.0f;
		static constexpr auto is_automatic_ = false;
		static constexpr auto can_it_be_reloaded_in_loop_ = false;

		static constexpr auto muzzle_flash_path_ = "objects/effects/muzzle_flash/muzzle_flash.pfx"sv;
		static constexpr auto muzzle_flash_attachment_name_ = "muzzle_flash_attachment"sv;
	};

	struct AmericanShotgun {
		static constexpr auto character_file_path_ = "objects/characters/american_shotgun/american_shotgun.cdf"sv;
		static constexpr auto material_file_path_ = "objects/characters/american_shotgun/american_shotgun.mtl"sv;
		static constexpr auto can_cast_shadow_ = false;
		static constexpr auto animation_data_base_file_path_ = "animations/mannequin/adb/american_shotgun.adb"sv;
		static constexpr auto controller_definition_file_path_ = "animations/mannequin/adb/male_arms_controller_definitions.xml"sv;
		static constexpr auto scope_contexts_ = DevelopmentTools::make_array("AmericanShotgunContext"sv);
		static constexpr auto animation_drive_motion_ = false;
		static constexpr auto animation_tag_name_ = "AmericanShotgun"sv;

		static constexpr auto mesh_position_x_ = 0.0f;
		static constexpr auto mesh_position_y_ = 0.0f;
		static constexpr auto mesh_position_z_ = 0.0f;
		static constexpr auto mesh_rotation_angle_x_ = 0.0f;
		static constexpr auto mesh_rotation_angle_y_ = 0.0f;
		static constexpr auto mesh_rotation_angle_z_ = 0.0f;
		static constexpr auto mesh_scale_x_ = 1.0f;
		static constexpr auto mesh_scale_y_ = 1.0f;
		static constexpr auto mesh_scale_z_ = 1.0f;

		static constexpr auto grip_name_ = "american_shotgun_attachment"sv;
		static constexpr auto collected_audio_trigger_name_ = ""sv;
		static constexpr auto thumbnail_ = ""sv;

		static constexpr auto mass_ = 2.4f;

		static constexpr auto pos_shake_frequency_ = 6.55f;
		static constexpr auto rot_shake_frequency_ = 12.55f;
		static constexpr auto pos_shake_amplitude_ = 0.095f;
		static constexpr auto rot_shake_amplitude_ = 0.075f;
		static constexpr auto shake_duration_ = 0.19f;
		static constexpr auto damage_ = 90;
		static constexpr auto magazine_size_ = 15;
		static constexpr auto magazine_capacity_ = 35;
		static constexpr auto bullets_per_reload_ = 1;
		static constexpr auto cooldown_time_ = 0.74f;
		static constexpr auto reload_cooldown_ = 0.77f;
		static constexpr auto spread_ = 0.07f;
		static constexpr auto number_of_projectiles_ = 6;
		static constexpr auto max_distance_ = 100.0f;
		static constexpr auto is_automatic_ = false;
		static constexpr auto can_it_be_reloaded_in_loop_ = true;

		static constexpr auto muzzle_flash_path_ = "objects/effects/muzzle_flash/muzzle_flash.pfx"sv;
		static constexpr auto muzzle_flash_attachment_name_ = "muzzle_flash_attachment"sv;
	};
}

#endif // !WEAPONS_DATA_H