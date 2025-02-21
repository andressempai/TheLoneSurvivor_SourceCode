#ifndef SHOOT_H
#define SHOOT_H

#include <CryEntitySystem/IEntityComponent.h>
#include <CryAction/IActionMapManager.h>

class Shoot final : public IEntityComponent, public IActionListener {
public:
	friend void ReflectType(Schematyc::CTypeDesc<Shoot>& desc);

private:
	void Initialize() override;
	void OnShutDown() override;
	Cry::Entity::EventFlags GetEventMask() const override;
	void ProcessEvent(const SEntityEvent& event) override;
	void OnAction(const ActionId& action, int activation_mode, float value) override;

	EntityId weapon_id_{ INVALID_ENTITYID };
	bool is_switching_weapon_{};
};

#endif // !SHOOT_H