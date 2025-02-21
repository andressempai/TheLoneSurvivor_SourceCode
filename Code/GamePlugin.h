#ifndef GAME_PLUGIN_H
#define GAME_PLUGIN_H

#include <CrySystem/ICryPlugin.h>
#include <CryGame/IGameFramework.h>
#include <CryAction/IActionMapManager.h>

class CGamePlugin : public Cry::IEnginePlugin, public ISystemEventListener {
public:
	CRYINTERFACE_SIMPLE(Cry::IEnginePlugin)
	CRYGENERATE_SINGLETONCLASS_GUID(CGamePlugin, "Blank", "f01244b0-a4e7-4dc6-91e1-0ed18906fe7c"_cry_guid)

	virtual ~CGamePlugin();
	
	// Cry::IEnginePlugin
	const char* GetCategory() const override { return "Game"; }
	bool Initialize(SSystemGlobalEnvironment& env, const SSystemInitParams& initParams) override;
	void UpdateBeforeRender() final;
	// ~Cry::IEnginePlugin

	// ISystemEventListener
	virtual void OnSystemEvent(ESystemEvent event, UINT_PTR wparam, UINT_PTR lparam) override;
	// ~ISystemEventListener

	static CGamePlugin* GetInstance()
	{
		return cryinterface_cast<CGamePlugin>(CGamePlugin::s_factory.CreateClassInstance().get());
	}
};

#endif // !GAME_PLUGIN_H