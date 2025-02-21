#ifndef PAUSE_GAME_H
#define PAUSE_GAME_H

#include <CryEntitySystem/IEntityComponent.h>
#include <CryAction/IActionMapManager.h>

class PauseGame final : public IEntityComponent, public IActionListener {
public:
	friend void ReflectType(Schematyc::CTypeDesc<PauseGame>& desc);

private:
	void Initialize() override;
	void OnAction(const ActionId& action, int activation_mode, float value) override;
};

#endif // !PAUSE_GAME_H