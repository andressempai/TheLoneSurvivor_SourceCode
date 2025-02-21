#ifndef FOOTSTEP_H
#define FOOTSTEP_H

#include <CryEntitySystem/IEntityComponent.h>

class Footstep final : public IEntityComponent {
public:
	friend void ReflectType(Schematyc::CTypeDesc<Footstep>& desc);

private:
	Cry::Entity::EventFlags GetEventMask() const override;
	void ProcessEvent(const SEntityEvent& event) override;
};

#endif // !FOOTSTEP_H