#ifndef BEHAVIOR_TREE_NODES_REGISTRATOR_H
#define BEHAVIOR_TREE_NODES_REGISTRATOR_H

#include "../Types/BehaviorTreeNodesTypes.h"
#include "Cooldown.h"
#include "DistanceCheck.h"
#include "ForceFailure.h"
#include "ForceRunning.h"
#include "ForceSuccess.h"
#include "MeleeAttack.h"
#include "Search.h"
#include "SetFlag.h"
#include "StopSearch.h"
#include "TryStunned.h"
#include "TurnToEngageEnemy.h"

#include <CrySerialization/ClassFactory.h>

inline void register_behavior_tree_nodes()
{
	using namespace BehaviorTree;

	auto& manager = *gEnv->pAISystem->GetIBehaviorTreeManager();
	[[maybe_unused]] const char* COLOR_CORE = "0000ff";

	REGISTER_BEHAVIOR_TREE_NODE_WITH_SERIALIZATION(
			manager
		,	Cooldown
		,	"Custom\\Cooldown"
		,	COLOR_CORE
	);

	REGISTER_BEHAVIOR_TREE_NODE_WITH_SERIALIZATION(
			manager
		,	DistanceCheck
		,	"Custom\\Distance Check"
		,	COLOR_CORE
	);

	REGISTER_BEHAVIOR_TREE_NODE_WITH_SERIALIZATION(
			manager
		,	ForceFailure
		,	"Custom\\Force Failure"
		,	COLOR_CORE
	);

	REGISTER_BEHAVIOR_TREE_NODE_WITH_SERIALIZATION(
			manager
		,	ForceRunning
		,	"Custom\\Force Running"
		,	COLOR_CORE
	);

	REGISTER_BEHAVIOR_TREE_NODE_WITH_SERIALIZATION(
			manager
		,	ForceSuccess
		,	"Custom\\Force Success"
		,	COLOR_CORE
	);

	REGISTER_BEHAVIOR_TREE_NODE_WITH_SERIALIZATION(
			manager
		,	MeleeAttack
		,	"Custom\\Melee Attack"
		,	COLOR_CORE
	);

	REGISTER_BEHAVIOR_TREE_NODE_WITH_SERIALIZATION(
			manager
		,	CompareKeyWithValueEntityId
		,	"Custom\\Compare Key(EntityId)"
		,	COLOR_CORE
	);

	REGISTER_BEHAVIOR_TREE_NODE_WITH_SERIALIZATION(
			manager
		,	CompareKeyWithValueBool
		,	"Custom\\Compare Key(Bool)"
		,	COLOR_CORE
	);

	REGISTER_BEHAVIOR_TREE_NODE_WITH_SERIALIZATION(
			manager
		,	Search
		,	"Custom\\Search"
		,	COLOR_CORE
	);

	REGISTER_BEHAVIOR_TREE_NODE_WITH_SERIALIZATION(
			manager
		,	SetFlag
		,	"Custom\\Set Flag"
		,	COLOR_CORE
	);

	REGISTER_BEHAVIOR_TREE_NODE_WITH_SERIALIZATION(
			manager
		,	StopSearch
		,	"Custom\\Stop Search"
		,	COLOR_CORE
	);

	REGISTER_BEHAVIOR_TREE_NODE_WITH_SERIALIZATION(
			manager
		,	TryStunned
		,	"Custom\\Try Stunned"
		,	COLOR_CORE
	);

	REGISTER_BEHAVIOR_TREE_NODE_WITH_SERIALIZATION(
			manager
		,	TurnToEngageEnemy
		,	"Custom\\Turn to Engage Enemy"
		,	COLOR_CORE
	);
}

#endif // !BEHAVIOR_TREE_NODES_REGISTRATOR_H