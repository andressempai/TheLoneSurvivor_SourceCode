#ifndef STORE_TO_MAGAZINE_H
#define STORE_TO_MAGAZINE_H

#include "Utils/AudioTriggerParams.h"

#include <CryEntitySystem/IEntityComponent.h>

struct StoreToMagazine final : public IEntityComponent {
	StoreToMagazine() = default;

	template <typename Data>
	StoreToMagazine(Data) : audio_trigger_params_{ Data::collected_audio_trigger_name_.data() }
	{
		audio_trigger_params_.audio_id_ = CryAudio::StringToId(audio_trigger_params_.collected_audio_trigger_name_.c_str());
	}

	friend void ReflectType(Schematyc::CTypeDesc<StoreToMagazine>& desc);

	void Initialize() override;
	void OnShutDown() override;

	AudioTriggerParams audio_trigger_params_{};
};

#endif // !STORE_TO_MAGAZINE_H