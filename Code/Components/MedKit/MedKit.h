#ifndef MEDKIT_H
#define MEDKIT_H

#include "Utils/HealthPack.h"

#include <CryEntitySystem/IEntityComponent.h>

class MedKit final : public IEntityComponent {
public:
	friend void ReflectType(Schematyc::CTypeDesc<MedKit>& desc);

private:
	void Initialize() override;
	void OnShutDown() override;
	Cry::Entity::EventFlags GetEventMask() const override;
	void ProcessEvent(const SEntityEvent& event) override;

	void collect(IEntity& other_entity);

	HealthPack health_pack_{ large_health_pack };
};

#endif // !MEDKIT_H