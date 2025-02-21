#ifndef CHARACTER_DATA_H
#define CHARACTER_DATA_H

#include "DevelopmentTools.h"

using namespace std::string_view_literals;
using namespace DevelopmentTools;

namespace Data {
	struct MaleArms {
		static constexpr auto character_file_path_ = "objects/characters/male_arms/male_arms.cdf"sv;
		static constexpr auto material_file_path_ = "objects/characters/male_arms/male_arms.mtl"sv;
		static constexpr auto animation_data_base_file_path_ = "animations/mannequin/adb/male_arms.adb"sv;
		static constexpr auto controller_definition_file_path_ = "animations/mannequin/adb/male_arms_controller_definitions.xml"sv;
		static constexpr auto scope_contexts_ = make_array("FirstPersonContext"sv, "FirstPersonUpperBodyContext"sv);
		static constexpr auto animation_drive_motion_ = false;
		static constexpr auto can_cast_shadow_ = false;

		static constexpr auto mesh_position_x_ = 0.0f;
		static constexpr auto mesh_position_y_ = -0.1725f;
		static constexpr auto mesh_position_z_ = -1.675f;
		static constexpr auto mesh_rotation_angle_x_ = 0.0f;
		static constexpr auto mesh_rotation_angle_y_ = 0.0f;
		static constexpr auto mesh_rotation_angle_z_ = 0.0f;
		static constexpr auto mesh_scale_x_ = 1.0f;
		static constexpr auto mesh_scale_y_ = 1.0f;
		static constexpr auto mesh_scale_z_ = 1.0f;

		static constexpr float fov_ = 45.0f;
		static constexpr float near_plane_ = 0.025f;
		static constexpr float far_plane_ = 1024.0f;
		static constexpr float min_pitch_rotation_ = -75.0f;
		static constexpr float max_pitch_rotation_ = 80.0f;
		static constexpr float mouse_sensitivity_ = 0.5f;

		static constexpr auto stand_speed_ = 8.5f;
		static constexpr auto crouch_speed_ = 5.5f;
		static constexpr auto jump_force_ = 4.35f;
		static constexpr auto jump_buffer_ = 0.25f;

		static constexpr auto throw_force_ = 15.5f;

		static constexpr auto trigger_size_x_ = 0.85f;
		static constexpr auto trigger_size_y_ = 0.85f;
		static constexpr auto trigger_size_z_ = 1.70f;

		static constexpr auto gravity_ = -12.8f;
		static constexpr auto mass_ = 88.0f;
		static constexpr auto air_control_ = 0.85f;
		static constexpr auto air_resistance_ = 0.15f;
		static constexpr auto inertia_ = 5.55f;
		static constexpr auto inertia_acceleration_ = 5.55f;
		static constexpr auto can_use_custom_gravity_ = true;

		static constexpr auto stand_height_ = 1.73f;
		static constexpr auto crouch_height_ = 1.035f;
		static constexpr auto radius_ = 0.65f;
		static constexpr auto stand_height_eyes_ = 1.61f;
		static constexpr auto crouch_height_eyes_ = 0.835f;
		static constexpr auto ground_contact_epsilon_ = 0.004f;
		static constexpr auto can_use_capsule_ = true;
	};

	struct Zombie {
		static constexpr auto character_file_path_ = "objects/characters/zombie/zombie.cdf"sv;
		static constexpr auto material_file_path_ = "objects/characters/zombie/zombie.mtl"sv;
		static constexpr auto animation_data_base_file_path_ = "animations/mannequin/adb/zombie.adb"sv;
		static constexpr auto controller_definition_file_path_ = "animations/mannequin/adb/zombie_controller_definitions.xml"sv;
		static constexpr auto scope_contexts_ = make_array("ThirdPersonContext"sv);
		static constexpr bool animation_drive_motion_ = true;

		static constexpr auto mesh_position_x_ = 0.0f;
		static constexpr auto mesh_position_y_ = 0.0f;
		static constexpr auto mesh_position_z_ = 0.0f;
		static constexpr auto mesh_rotation_angle_x_ = 0.0f;
		static constexpr auto mesh_rotation_angle_y_ = 0.0f;
		static constexpr auto mesh_rotation_angle_z_ = 0.0f;
		static constexpr auto mesh_scale_x_ = 1.0f;
		static constexpr auto mesh_scale_y_ = 1.0f;
		static constexpr auto mesh_scale_z_ = 1.0f;

		static constexpr auto stand_speed_ = 3.35f;
		static constexpr auto crouch_speed_ = 0.0f;
		static constexpr auto jump_force_ = 5.35f;

		static constexpr auto gravity_ = -12.8f;
		static constexpr auto mass_ = 65.0f;
		static constexpr auto air_control_ = 0.85f;
		static constexpr auto air_resistance_ = 0.15f;
		static constexpr auto inertia_ = 5.55f;
		static constexpr auto inertia_acceleration_ = 5.55f;
		static constexpr auto can_use_custom_gravity_ = true;

		static constexpr auto stand_height_ = 2.37f;
		static constexpr auto crouch_height_ = 1.17f;
		static constexpr auto radius_ = 0.6f;
		static constexpr auto stand_height_eyes_ = 2.17f;
		static constexpr auto crouch_height_eyes_ = 0.97f;
		static constexpr auto ground_contact_epsilon_ = 0.004f;
		static constexpr auto can_use_capsule_ = true;

		static constexpr auto behavior_tree_name_ = "zombie"sv;
	};

	struct Perderos {
		static constexpr auto character_file_path_ = "objects/characters/perderos/perderos.cdf"sv;
		static constexpr auto material_file_path_ = "objects/characters/perderos/perderos.mtl"sv;
		static constexpr auto animation_data_base_file_path_ = "animations/mannequin/adb/perderos.adb"sv;
		static constexpr auto controller_definition_file_path_ = "animations/mannequin/adb/perderos_controller_definitions.xml"sv;
		static constexpr auto scope_contexts_ = make_array("ThirdPersonContext"sv, "ThirdPersonUpperBodyContext"sv);
		static constexpr bool animation_drive_motion_ = false;

		static constexpr auto mesh_position_x_ = 0.0f;
		static constexpr auto mesh_position_y_ = 0.0f;
		static constexpr auto mesh_position_z_ = 0.0f;
		static constexpr auto mesh_rotation_angle_x_ = 0.0f;
		static constexpr auto mesh_rotation_angle_y_ = 0.0f;
		static constexpr auto mesh_rotation_angle_z_ = 0.0f;
		static constexpr auto mesh_scale_x_ = 1.0f;
		static constexpr auto mesh_scale_y_ = 1.0f;
		static constexpr auto mesh_scale_z_ = 1.0f;

		static constexpr auto stand_speed_ = 5.35f;
		static constexpr auto crouch_speed_ = 0.0f;
		static constexpr auto jump_force_ = 5.35f;

		static constexpr auto gravity_ = -15.98f;
		static constexpr auto mass_ = 145.0f;
		static constexpr auto air_control_ = 0.85f;
		static constexpr auto air_resistance_ = 0.15f;
		static constexpr auto inertia_ = 5.55f;
		static constexpr auto inertia_acceleration_ = 5.55f;
		static constexpr auto can_use_custom_gravity_ = true;

		static constexpr auto stand_height_ = 1.77f;
		static constexpr auto crouch_height_ = 1.17f;
		static constexpr auto radius_ = 1.45f;
		static constexpr auto stand_height_eyes_ = 2.17f;
		static constexpr auto crouch_height_eyes_ = 0.97f;
		static constexpr auto ground_contact_epsilon_ = 0.004f;
		static constexpr auto can_use_capsule_ = true;

		static constexpr auto behavior_tree_name_ = "perderos"sv;
	};
}

#endif // !CHARACTER_DATA_H