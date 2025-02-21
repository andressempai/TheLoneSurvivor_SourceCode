#ifndef SWITCH_WEAPON_H
#define SWITCH_WEAPON_H

#include "TimerManager.h"

#include <CryEntitySystem/IEntityComponent.h>
#include <CryAction/IActionMapManager.h>

#include <bitset>

class SwitchWeapon final : public IEntityComponent, public IActionListener {
	enum {
			next_gun
		,	previous_gun
		,	select_pistol
		,	select_shotgun
		,	total_actions
	};

public:
	friend void ReflectType(Schematyc::CTypeDesc<SwitchWeapon>& desc);

private:
	void Initialize() override;
	void OnShutDown() override;
	void ProcessEvent(const SEntityEvent& event) override;
	Cry::Entity::EventFlags GetEventMask() const override;
	void OnAction(const ActionId& action, int activation_mode, float value) override;

	TimerId parent_to_entity_timer_id_{};
	std::bitset<total_actions> input_actions_{};
	bool is_switching_weapon_{};
};

#endif // !SWITCH_WEAPON_H