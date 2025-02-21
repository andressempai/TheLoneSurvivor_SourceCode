#ifndef SKELETAL_MESH_H
#define SKELETAL_MESH_H

#include <CryEntitySystem/IEntityComponent.h>
#include <CrySchematyc/CoreAPI.h>

namespace MP {
	template <typename T, typename = void>
	struct has_can_cast_shadow {
		static constexpr auto value = true;
	};
	
	template <typename T>
	struct has_can_cast_shadow<T, std::void_t<decltype(T::can_cast_shadow_)>> {
		static constexpr auto value = T::can_cast_shadow_;
	};
}

class SkeletalMesh final : public IEntityComponent {
public:
	SkeletalMesh() = default;

	template <typename Data>
	SkeletalMesh(Data)
		:	transform_offset_{
				Matrix34::Create(
						Vec3(Data::mesh_scale_x_, Data::mesh_scale_y_, Data::mesh_scale_z_)
					,	Quat::CreateRotationZ(Data::mesh_rotation_angle_z_)
					,	Vec3(Data::mesh_position_x_, Data::mesh_position_y_, Data::mesh_position_z_)
				)
			}
		,	character_file_path_{ Data::character_file_path_.data() }
		,	material_file_path_{ Data::material_file_path_.data() }
		,	mass_{ Data::mass_ }
		,	slot_{ -1 }
		,	can_cast_shadow_{ MP::has_can_cast_shadow<Data>::value }
	{}

	friend void ReflectType(Schematyc::CTypeDesc<SkeletalMesh>& desc);

private:
	void Initialize() override;
	void OnShutDown() override;
	Cry::Entity::EventFlags GetEventMask() const override;
	void ProcessEvent(const SEntityEvent& event) override;

	void calculate_slot();
	void load_character_instance();
	void shadow_casting();
	void physicalize();
	
	Matrix34 transform_offset_{ IDENTITY };
	Schematyc::CharacterFileName character_file_path_{};
	Schematyc::MaterialFileName material_file_path_{};
	f32 mass_{ 90.0f };
	int slot_{ -1 };
	bool can_cast_shadow_{ true };
};

#endif // !WSKELETAL_MESH_H