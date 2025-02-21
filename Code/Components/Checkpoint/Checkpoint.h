#ifndef CHECKPOINT_H
#define CHECKPOINT_H

#include <CryEntitySystem/IEntityComponent.h>

class CCheckpoint final : public IEntityComponent {
public:
	friend void ReflectType(Schematyc::CTypeDesc<CCheckpoint>& desc);

private:
	Cry::Entity::EventFlags GetEventMask() const override;
	void ProcessEvent(const SEntityEvent& event) override;
};

#endif // !CHECKPOINT_H