#ifndef SHOOTER_H
#define SHOOTER_H

#include <CryEntitySystem/IEntityComponent.h>

struct Shooter : public IEntityComponent {
	struct ViewPoint {
		Vec3 position{ ZERO };
		Vec3 direction{ ZERO };
	};

	friend void ReflectType(Schematyc::CTypeDesc<Shooter>& desc)
	{
		desc.SetGUID("{19E38BDC-B3F1-4390-A30B-7BDBABE1274B}"_cry_guid);
		desc.SetName(Schematyc::CTypeName("shooter"));
	}

	virtual ~Shooter() = default;

	virtual ViewPoint get_view_point() const = 0;
};

#endif // !SHOOTER_H