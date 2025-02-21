#ifndef MESH_PARAMS_H
#define MESH_PARAMS_H

#include <CrySchematyc/ResourceTypes.h>

struct SMeshParams {
	using Ratio = Schematyc::Range<0, 255, 0, 255, int32>;

	Schematyc::GeomFileName geometry_file_path_{};
	Schematyc::MaterialFileName material_file_path_{};
	Ratio view_distance_ratio_{ 100 };
	Ratio lod_ratio_{ 100 };
};

static void ReflectType(Schematyc::CTypeDesc<SMeshParams>& desc)
{
	desc.SetGUID("{77D4720C-3B04-4B7B-823A-A5274C0F6C3F}"_cry_guid);
	desc.SetLabel("Mesh Params");
	desc.AddMember(
			&SMeshParams::geometry_file_path_
		,	'mgeo'
		,	"geometry"
		,	"Geometry"
		,	"Geometry File Path"
		,	Schematyc::GeomFileName{}
	);

	desc.AddMember(
			&SMeshParams::material_file_path_
		,	'mmat'
		,	"material"
		,	"Material"
		,	"Material File Path"
		,	Schematyc::MaterialFileName{}
	);

	desc.AddMember(
			&SMeshParams::view_distance_ratio_
		,	'vrat'
		,	"viewdistanceratio"
		,	"View Distance Ratio"
		,	"View Distance Ratio"
		,	SMeshParams::Ratio{ 100 }
	);

	desc.AddMember(
			&SMeshParams::lod_ratio_
		,	'lrat'
		,	"lodratio"
		,	"LOD Ratio"
		,	"Level of Detail Ratio"
		,	SMeshParams::Ratio{ 100 }
	);
}

inline bool operator==(const SMeshParams& lhs, const SMeshParams& rhs)
{
	return memcmp(&lhs, &rhs, sizeof(lhs)) == 0;
}

inline bool operator!=(const SMeshParams& lhs, const SMeshParams& rhs)
{
	return !(operator==(lhs, rhs));
}

#endif // !MESH_PARAMS_H