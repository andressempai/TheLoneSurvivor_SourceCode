#ifndef LIVING_PHYSIC_H
#define LIVING_PHYSIC_H

#include "Utils/CharacterDimensions.h"
#include "Utils/CharacterDynamics.h"

#include <CryEntitySystem/IEntityComponent.h>

class LivingPhysic final : public IEntityComponent
#ifndef RELEASE
	,	public IEntityComponentPreviewer
#endif // !RELEASE
{
public:
	LivingPhysic() = default;

	template <typename Data>
	LivingPhysic(Data)
		:	dimensions_{
				SCharacterDimensionsControls(Data::stand_height_)
				.radius(Data::radius_)
				.height_eyes(Data::stand_height_eyes_)
				.ground_contact_epsilon(Data::ground_contact_epsilon_)
				.can_use_capsule(Data::can_use_capsule_)
			}
		,	dynamics_{
				SCharacterDynamicsControls(Vec3(0.0f, 0.0f, Data::gravity_))
				.mass(Data::mass_)
				.air_control(Data::air_control_)
				.air_resistance(Data::air_resistance_)
				.inertia(Data::inertia_)
				.inertia_acceleration(Data::inertia_acceleration_)
				.can_use_custom_gravity(Data::can_use_custom_gravity_)
			}
	{}

	friend void ReflectType(Schematyc::CTypeDesc<LivingPhysic>& desc);

private:
	void Initialize() override;
	void OnShutDown() override;
	Cry::Entity::EventFlags GetEventMask() const override;
	void ProcessEvent(const SEntityEvent& event) override;

#ifndef RELEASE
	IEntityComponentPreviewer* GetPreviewer() override
	{
		return this;
	}

	void SerializeProperties(Serialization::IArchive& archive) override {}
	void Render(const IEntity& entity, const IEntityComponent& component, SEntityPreviewContext& context) const override;
#endif // !RELEASE

	SCharacterDimensions dimensions_{};
	SCharacterDynamics dynamics_{};
	int slot_{ -1 };
};

#endif // !LIVING_PHYSIC_H