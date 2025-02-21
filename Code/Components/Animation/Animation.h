#ifndef ANIMATION_H
#define ANIMATION_H

#include <CryEntitySystem/IEntityComponent.h>
#include <CryAnimation/ICryMannequin.h>
#include <CrySchematyc/CoreAPI.h>

struct ScopeContextSelector : Serialization::IResourceSelector
{
	ScopeContextSelector() : ScopeContextSelector("") {}
	explicit ScopeContextSelector(std::string_view value) : value_{ value.data() }
	{
		resourceType = "MannequinScopeContextName";
		pCustomParams = Serialization::ICustomResourceParamsPtr();
	}

	const char* GetValue() const override
	{
		return value_.c_str();
	}

	void SetValue(const char* s) override
	{
		value_ = s;
	}

	const void* GetHandle() const override
	{
		return &value_;
	}

	Serialization::TypeID GetType() const override
	{
		return Serialization::TypeID::get<string>();
	}

	string value_{};
};

inline bool Serialize(Serialization::IArchive& archive, ScopeContextSelector& value, const char* name, const char* label)
{
	if (archive.isEdit())
		return archive(Serialization::SStruct::forEdit(static_cast<Serialization::IResourceSelector&>(value)), name, label);
	else
		return archive(value.value_, name, label);
}

class Animation : public IEntityComponent {
	struct AnimationData {
		AnimationData() = default;

		template <typename Data>
		AnimationData(Data)
			:	scope_contexts_selector_{ Data::scope_contexts_.begin(), Data::scope_contexts_.end() }
			,	controller_definition_path_{ Data::controller_definition_file_path_.data() }
			,	animation_database_path_{ Data::animation_data_base_file_path_.data() }
			,	animation_drive_motion_{ Data::animation_drive_motion_ }
		{}

		friend void ReflectType(Schematyc::CTypeDesc<AnimationData>& desc) {
			desc.SetGUID("{C06B1231-8BE8-41F0-8800-AFF246E54706}"_cry_guid);
			desc.SetLabel("AnimationScopeContext");
			desc.SetDescription("This structure is used to store the name of the scope context.");
		}

		bool operator==(const AnimationData& rhs) const {
			return 0 == memcmp(this, &rhs, sizeof(rhs));
		}

		std::vector<ScopeContextSelector> scope_contexts_selector_{};
		string controller_definition_path_{};
		string animation_database_path_{};
		bool animation_drive_motion_{};
	};

public:
	template <typename Data>
	Animation(Data) : animation_data_{ Data{} } {}

	Animation() = default;
	virtual ~Animation() = default;

	friend void ReflectType(Schematyc::CTypeDesc<Animation>& desc);
	friend void ReflectType(Schematyc::CTypeDesc<class WeaponAnimation>& desc);
	friend void ReflectType(Schematyc::CTypeDesc<class LivingAnimation>& desc);
	friend void ReflectType(Schematyc::CTypeDesc<class ZombieAnimation>& desc);
	friend void ReflectType(Schematyc::CTypeDesc<class PerderosAnimation>& desc);
	friend bool Serialize(
			Serialization::IArchive& archive
		,	Animation::AnimationData& data
		,	const char* name
		,	const char* label
	);

protected:
	struct priority {
		static constexpr std::size_t very_low = 0;
		static constexpr std::size_t low = 10;
		static constexpr std::size_t medium = 20;
		static constexpr std::size_t high = 30;
		static constexpr std::size_t very_high = 40;
	};

	void Initialize() override;
	void OnShutDown() override;
	Cry::Entity::EventFlags GetEventMask() const override;
	void ProcessEvent(const SEntityEvent& event) override;

	virtual void initialize_fragment() {}

	AnimationData animation_data_{};

	const SControllerDef* controller_definition_{};
	const IAnimationDatabase* animation_database_{};
	std::unique_ptr<SAnimationContext> animation_context_{};
	IActionController* action_controller_{};
};

#endif // !ANIMATION_H