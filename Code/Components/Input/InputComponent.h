#ifndef INPUT_COMPONENT_H
#define INPUT_COMPONENT_H

#include <CryEntitySystem/IEntityComponent.h>

class InputComponent final : public IEntityComponent {
public:
	friend void ReflectType(Schematyc::CTypeDesc<InputComponent>& desc);

private:
	void Initialize() override;
	void OnShutDown() override;

	struct IActionMapManager* action_map_manager_{};
};

#endif // !INPUT_COMPONENT_H