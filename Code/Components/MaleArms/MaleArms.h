#ifndef MALE_ARMS_H
#define MALE_ARMS_H

#include "Components/Shooter.h"

class MaleArms final : public Shooter {
public:
	friend void ReflectType(Schematyc::CTypeDesc<MaleArms>& desc);

private:
	Shooter::ViewPoint get_view_point() const override;
	void Initialize() override;
};

#endif // !MALE_ARMS_H