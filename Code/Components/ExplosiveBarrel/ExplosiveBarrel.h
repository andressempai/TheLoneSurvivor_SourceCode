#ifndef EXPLOSIVE_BARREL_H
#define EXPLOSIVE_BARREL_H

#include "../../Mixin/Explosive.h"

#include <CryEntitySystem/IEntityComponent.h>
#include <CrySchematyc/MathTypes.h>

class ExplosiveBarrel final : public IEntityComponent, public Explosive {
public:
	friend void ReflectType(Schematyc::CTypeDesc<ExplosiveBarrel>& desc);

	ExplosiveBarrel();

private:
	void Initialize() override;
	void OnShutDown() override;
	Cry::Entity::EventFlags GetEventMask() const override;
	void ProcessEvent(const SEntityEvent& event) override;

	void impact_damage(const std::vector<Vec3>& impact_points, const Vec3& impact_direction, int32 damage);
	void explosion_damage(int damage);

	Schematyc::Range<1, 100, 1, 100, int32> hardness_;
	Schematyc::Range<1, 10000, 1, 10000, int32> time_before_explosion_;
	Schematyc::PositiveFloat damage_;
};

#endif	// !EXPLOSIVE_BARREL_H