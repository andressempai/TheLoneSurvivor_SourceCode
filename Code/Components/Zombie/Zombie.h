#ifndef ZOMBIE_H
#define ZOMBIE_H

#include <CryEntitySystem/IEntityComponent.h>

class Zombie final : public IEntityComponent {
public:
	friend void ReflectType(Schematyc::CTypeDesc<Zombie>& desc);

private:
	void Initialize() override;
	void ProcessEvent(const SEntityEvent& event) override;
	Cry::Entity::EventFlags GetEventMask() const override;

	Vec3 requested_velocity_{ ZERO };
	bool apply_root_motion_{};
};

#endif // !ZOMBIE_H