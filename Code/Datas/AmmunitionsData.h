#ifndef AMMUNITIONS_DATA_H
#define AMMUNITIONS_DATA_H

#include "../Utils/DynamicMeshType.h"

#include <string_view>

using namespace std::string_view_literals;

namespace Data {
	struct PistolAmmunition {
		static constexpr auto geometry_file_path_ = "objects/stage_elements/ammo/pistol/pistol_ammo_crate.cgf"sv;
		static constexpr auto material_file_path_ = "objects/stage_elements/ammo/pistol/pistol_ammo_box_crate.mtl"sv;
		static constexpr auto view_distance_ratio_ = 255;
		static constexpr auto lod_ratio_ = 100;
		static constexpr auto collected_audio_trigger_name_ = "pistol_ammo"sv;
		static constexpr auto gravity_x_ = 0.0f;
		static constexpr auto gravity_y_ = 0.0f;
		static constexpr auto gravity_z_ = -12.98f;
		static constexpr auto mass_ = 1.33f;
		static constexpr auto emesh_type_ = EDynamicMeshType::Rigid;
		static constexpr auto magazine_size_ = 10;
		static constexpr auto magazine_capacity_ = 50;
	};
	
	struct ShotgunAmmunition {
		static constexpr auto geometry_file_path_ = "objects/stage_elements/ammo/shotgun/shotgun_ammo_crate.cgf"sv;
		static constexpr auto material_file_path_ = "objects/stage_elements/ammo/shotgun/shotgun_ammo_box_crate.mtl"sv;
		static constexpr auto view_distance_ratio_ = 255;
		static constexpr auto lod_ratio_ = 100;
		static constexpr auto collected_audio_trigger_name_ = "shotgun_ammo"sv;
		static constexpr auto gravity_x_ = 0.0f;
		static constexpr auto gravity_y_ = 0.0f;
		static constexpr auto gravity_z_ = -12.98f;
		static constexpr auto mass_ = 1.33f;
		static constexpr auto emesh_type_ = EDynamicMeshType::Rigid;
		static constexpr auto magazine_size_ = 5;
		static constexpr auto magazine_capacity_ = 40;
	};
}

#endif // !AMMUNITIONS_DATA_H