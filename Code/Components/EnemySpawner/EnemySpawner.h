#ifndef ENEMY_SPAWNER_H
#define ENEMY_SPAWNER_H

#include <CryEntitySystem/IEntityComponent.h>
#include <CrySchematyc/MathTypes.h>

enum class EnemyType {
		zombie
	,	perderos
};

SERIALIZATION_ENUM_BEGIN(EnemyType, "Enemy Type")
SERIALIZATION_ENUM(EnemyType::zombie, "zombie", "Zombie")
SERIALIZATION_ENUM(EnemyType::perderos, "perderos", "Perderos")
SERIALIZATION_ENUM_END()

class EnemySpawner final : public IEntityComponent {
	using EnemyTypeVector = std::vector<EnemyType>;

	struct EnemyTypeContainer {
		friend void ReflectType(Schematyc::CTypeDesc<EnemyTypeContainer>& desc)
		{
			desc.SetGUID("{A78CED78-132D-4F2B-8674-FDC13A950436}"_cry_guid);
			desc.SetName(Schematyc::CTypeName("enemytypecontainer"));
			desc.SetLabel("Enemy Type Container");
			desc.SetDescription("Container for enemy types");
		}

		bool operator==(const EnemyTypeContainer& rhs) const
		{
			return 0 == memcmp(this, &rhs, sizeof(rhs));
		}

		EnemyTypeVector container_{};
	};

public:
	friend void ReflectType(Schematyc::CTypeDesc<EnemySpawner>& desc);
	friend bool Serialize(
			Serialization::IArchive& archive
		,	EnemySpawner::EnemyTypeContainer& data
		,	const char* name
		,	const char* label
	);

private:
	Cry::Entity::EventFlags GetEventMask() const override;
	void ProcessEvent(const SEntityEvent& event) override;

	EnemyTypeContainer enemy_to_spawn_{};
	Schematyc::Range<0, 600> spawn_interval_{};
	Schematyc::Range<0, 60> initial_delay_{};
	f32 time_since_spawn_{};
};

#endif // ENEMY_SPAWNER_H