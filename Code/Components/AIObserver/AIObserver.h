#ifndef AI_OBSERVER_H
#define AI_OBSERVER_H

#include <CryEntitySystem/IEntityComponent.h>
#include <CryAISystem/IVisionMap.h>

class AIObserver final : public IEntityComponent {
public:
	friend void ReflectType(Schematyc::CTypeDesc<AIObserver>& desc);

private:
	void Initialize() override;
	Cry::Entity::EventFlags GetEventMask() const override;
	void ProcessEvent(const SEntityEvent& event) override;

	void visibility_status_change(
			const VisionID& observer_id
		,	const ObserverParams& observer_params
		,	const VisionID& observable_id
		,	const ObservableParams& observable_params
		,	bool is_visible
	);

	ObserverParams params_{};
	ObserverID id_{};
};

#endif // !AI_OBSERVER_H