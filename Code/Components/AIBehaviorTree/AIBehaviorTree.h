#ifndef AI_BEHAVIOR_TREE_H
#define AI_BEHAVIOR_TREE_H

#include <CryEntitySystem/IEntityComponent.h>

#include <string_view>

class AIBehaviorTree final : public IEntityComponent {
public:
	AIBehaviorTree() = default;

	template <typename Data>
	AIBehaviorTree(Data) : name_{ Data::behavior_tree_name_ } {}

	friend void ReflectType(Schematyc::CTypeDesc<AIBehaviorTree>& desc);

private:
	void Initialize() override;
	void OnShutDown() override;

	std::string_view name_{};
	bool is_dead_{};
};

#endif // !AI_BEHAVIOR_TREE_H