#ifndef DYNAMIC_MESH_TYPE_H
#define DYNAMIC_MESH_TYPE_H

#include <CrySchematyc/Reflection/TypeDesc.h>

enum struct EDynamicMeshType {
		Static = 1
	,	Rigid
};

static void ReflectType(Schematyc::CTypeDesc<EDynamicMeshType>& desc)
{
	desc.SetGUID("{B61EA16A-A670-48D0-9603-AC62679929BF}"_cry_guid);
	desc.SetLabel("Mesh Types");
	desc.AddConstant(EDynamicMeshType::Static, "static", "Static");
	desc.AddConstant(EDynamicMeshType::Rigid, "rigid", "Rigid");
}

#endif // !DYNAMIC_MESH_TYPE_H