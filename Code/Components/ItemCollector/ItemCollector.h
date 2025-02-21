#ifndef ITEM_COLLECTOR_H
#define ITEM_COLLECTOR_H

#include <CryEntitySystem/IEntityComponent.h>

class ItemCollector final : public IEntityComponent
#ifndef RELEASE
	,	public IEntityComponentPreviewer
#endif // !RELEASE
{
public:
	ItemCollector() = default;

	template <typename Data>
	ItemCollector(Data)
		:	size_{ Data::trigger_size_x_, Data::trigger_size_y_, Data::trigger_size_z_ }
	{}

	friend void ReflectType(Schematyc::CTypeDesc<ItemCollector>& desc);

private:
	void Initialize() override;
	Cry::Entity::EventFlags GetEventMask() const override;
	void ProcessEvent(const SEntityEvent& event) override;

#ifndef RELEASE
	IEntityComponentPreviewer* GetPreviewer() override;
	void SerializeProperties(Serialization::IArchive& archive) override {}
	void Render(const IEntity& entity, const IEntityComponent& component, SEntityPreviewContext& context) const override;
#endif // !RELEASE

	IEntityTriggerComponent* trigger_{};
	Vec3 size_{ ZERO };
};

#endif	// !ITEM_COLLECTOR_H