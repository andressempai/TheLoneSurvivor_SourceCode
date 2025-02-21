#ifndef AI_LISTENER_H
#define AI_LISTENER_H

#include <CryEntitySystem/IEntityComponent.h>
#include <CryPhysics/physinterface.h>
#include <CryAISystem/IAuditionMap.h>

class AIListener final : public IEntityComponent {
public:
	friend void ReflectType(Schematyc::CTypeDesc<AIListener>& desc);

private:
	void Initialize() override;
	Cry::Entity::EventFlags GetEventMask() const override;
	void ProcessEvent(const SEntityEvent& event) override;

	void on_heard_sound(const Perception::SSoundStimulusParams& stimulus);

	Perception::SListenerParams params_{};
};

#endif // !AI_LISTENER_H