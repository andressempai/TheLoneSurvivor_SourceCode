#ifndef AUDIO_TRIGGER_PARAMS_H
#define AUDIO_TRIGGER_PARAMS_H

#include <CryAudio/IAudioInterfacesCommonData.h>
#include <CrySchematyc/Reflection/TypeDesc.h>
#include <CrySerialization/Decorators/ResourcesAudio.h>

#include <string_view>

struct AudioTriggerParams {
	AudioTriggerParams() = default;
	AudioTriggerParams(std::string_view audio_trigger_name) : collected_audio_trigger_name_{ audio_trigger_name.data() } {}

	string collected_audio_trigger_name_{};
	CryAudio::ControlId audio_id_{ CryAudio::InvalidControlId };
};

inline void ReflectType(Schematyc::CTypeDesc<AudioTriggerParams>& desc) {
	desc.SetGUID("{0A245EDC-3B88-4A27-A910-2578C6BF27C1}"_cry_guid);
	desc.SetName(Schematyc::CTypeName{ "audiotrigger" });
	desc.SetLabel("Audio Trigger");
}

inline bool operator==(const AudioTriggerParams& lhs, const AudioTriggerParams& rhs) {
	return lhs.collected_audio_trigger_name_ == rhs.collected_audio_trigger_name_ and lhs.audio_id_ == rhs.audio_id_;
}

inline bool operator!=(const AudioTriggerParams& lhs, const AudioTriggerParams& rhs) {
	return not(lhs == rhs);
}

inline bool Serialize(Serialization::IArchive& archive, AudioTriggerParams& data, const char* name, const char* label)
{
	archive(Serialization::AudioTrigger(data.collected_audio_trigger_name_), name, label);
	archive.doc("Audio to Trigger");

	data.audio_id_ = CryAudio::StringToId(data.collected_audio_trigger_name_.c_str());

	return true;
}

#endif // !AUDIO_TRIGGER_PARAMS_H