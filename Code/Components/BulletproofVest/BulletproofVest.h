#ifndef BULLETPROOF_VEST_H
#define BULLETPROOF_VEST_H

#include "Utils/Armor.h"

#include <CryEntitySystem/IEntityComponent.h>

class BulletproofVest final : public IEntityComponent {
public:
	friend void ReflectType(Schematyc::CTypeDesc<BulletproofVest>& desc);

private:
	void Initialize() override;
	void OnShutDown() override;
	Cry::Entity::EventFlags GetEventMask() const override;
	void ProcessEvent(const SEntityEvent& event) override;

	void collect(IEntity& other_entity);

	Armor armor_{ LargeArmor };
};

#endif // !BULLETPROOF_VEST_H