#ifndef BEHAVIOR_TREE_NODES_TYPES_H
#define BEHAVIOR_TREE_NODES_TYPES_H

#include "BehaviorTreeNodes/CompareKeyWithValue.h"
#include "Datas/BehaviorTreeNodesDatas.h"

using CompareKeyWithValueEntityId = BehaviorTree::CompareKeyWithValue<EntityId, CompareKeyWithValueEntityIdData>;
using CompareKeyWithValueBool = BehaviorTree::CompareKeyWithValue<bool, CompareKeyWithValueBoolData>;

#endif // !BEHAVIOR_TREE_NODES_TYPES_H