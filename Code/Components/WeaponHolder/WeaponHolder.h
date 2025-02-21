#ifndef WWEAPON_HOLDER_H
#define WWEAPON_HOLDER_H

#include <CryEntitySystem/IEntityComponent.h>

class WeaponHolder final : public IEntityComponent {
public:
	WeaponHolder() = default;

	template <typename Data>
	WeaponHolder(Data) {}

	friend struct FindAttachmentAndSendEquipEvent;
	friend void ReflectType(Schematyc::CTypeDesc<WeaponHolder>& desc);

private:
	Cry::Entity::EventFlags GetEventMask() const override;
	void ProcessEvent(const SEntityEvent& event) override;

	std::string_view equipped_weapon_grip_{};
	std::string_view animation_tag_{};
	IEntity* current_weapon_{};
};

#endif // !WWEAPON_HOLDER_H