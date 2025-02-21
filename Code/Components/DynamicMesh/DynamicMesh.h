#ifndef DYNAMIC_MESH_H
#define DYNAMIC_MESH_H

#include "Utils/SimulationParams.h"
#include "Utils/MeshParams.h"
#include "Utils/DynamicMeshType.h"

#include <CryEntitySystem/IEntityComponent.h>

template <typename T, typename = void>
struct has_can_cast_shadow {
	static constexpr auto value = true;
};

template <typename T>
struct has_can_cast_shadow<T, std::void_t<decltype(T::can_cast_shadow_)>> {
	static constexpr auto value = T::can_cast_shadow_;
};

class DynamicMesh final : public IEntityComponent {
public:
	friend void ReflectType(Schematyc::CTypeDesc<DynamicMesh>& desc);

	DynamicMesh() = default;

	template <typename Data>
	DynamicMesh(Data)
		:	simulation_params_{
				CSimulationParamsControls{ Vec3{ Data::gravity_x_, Data::gravity_y_, Data::gravity_z_} }
				.mass(Data::mass_)
			}
		,	mesh_params_{ 
					Data::geometry_file_path_.data()
				,	Data::material_file_path_.data()
				,	Data::view_distance_ratio_
				,	Data::lod_ratio_
			}
		,	type_{ Data::emesh_type_ }
		,	can_cast_shadow_{ has_can_cast_shadow<Data>::value }
	{}

	void set_simulation_params(const SSimulationParams& simulation_params)
	{
		simulation_params_ = simulation_params;
	}

	void set_mesh_params(const SMeshParams& mesh_params)
	{
		mesh_params_ = mesh_params;
	}

	void set_type(EDynamicMeshType type)
	{
		type_ = type;
	}

	void load_mesh();

private:
	void Initialize() override;
	Cry::Entity::EventFlags GetEventMask() const override;
	void ProcessEvent(const SEntityEvent& event) override;

	void physicalize();

	SSimulationParams simulation_params_{};
	SMeshParams mesh_params_{};
	EDynamicMeshType type_{ EDynamicMeshType::Static };
	int32 slot_{ -1 };
	bool can_cast_shadow_{ true };
};

#endif // !DYNAMIC_MESH_H