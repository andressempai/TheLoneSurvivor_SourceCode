#ifndef MUZZLE_FLASH_H
#define MUZZLE_FLASH_H

#include <CryEntitySystem/IEntityComponent.h>
#include <CrySchematyc/ResourceTypes.h>

#include <string_view>

class MuzzleFlash final : public IEntityComponent {
public:
	MuzzleFlash() = default;

	template <typename Data>
	MuzzleFlash(Data)
		:	particle_effect_name_{ Data::muzzle_flash_path_.data() }
		,	attachment_name_{ Data::muzzle_flash_attachment_name_ }
	{}

	friend void ReflectType(Schematyc::CTypeDesc<MuzzleFlash>& desc);

	void start_muzzle_flash();
	void stop_muzzle_flash();

private:
	void Initialize() override;
	void OnShutDown() override;
	Cry::Entity::EventFlags GetEventMask() const override;
	void ProcessEvent(const SEntityEvent& event) override;

	Schematyc::ParticleEffectName particle_effect_name_{};
	std::string_view attachment_name_{};
	int32 slot_{ -1 };
};

#endif // !MUZZLE_FLASH_H