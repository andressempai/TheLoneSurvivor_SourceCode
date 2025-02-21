#ifndef BEHAVIOR_TREE_NODES_DATAS_H
#define BEHAVIOR_TREE_NODES_DATAS_H

#include <string_view>

using namespace std::string_view_literals;

struct CompareKeyWithValueEntityIdData {
	static constexpr auto node_name_ = "CompareKeyWithValueEntityId"sv;
};

struct CompareKeyWithValueBoolData {
	static constexpr auto node_name_ = "CompareKeyWithValueBool"sv;
};

#endif // !BEHAVIOR_TREE_NODES_DATAS_H