#ifndef AI_OBSERVABLE_H
#define AI_OBSERVABLE_H

#include <CryEntitySystem/IEntityComponent.h>
#include <CryAISystem/IVisionMap.h>

class AIObservable final : public IEntityComponent {
public:
	friend void ReflectType(Schematyc::CTypeDesc<AIObservable>& desc);

private:
	void Initialize() override;
	Cry::Entity::EventFlags GetEventMask() const override;
	void ProcessEvent(const SEntityEvent& event) override;

	ObservableParams params_{};
	ObservableID id_{};
};

#endif // !AI_OBSERVABLE_H