#ifndef MAGAZINE_H
#define MAGAZINE_H

#include "../../MetaProgramming.h"
#include "../../TimerManager.h"

#include <CryEntitySystem/IEntityComponent.h>
#include <CrySchematyc/MathTypes.h>

template <typename T, typename = void>
struct try_get_cooldown_time_value {
	static constexpr auto value = 0.0f;
};

template <typename T>
struct try_get_cooldown_time_value<T, std::void_t<decltype(T::cooldown_time_)>> {
	static constexpr auto value = T::cooldown_time_;
};

class Magazine final : public IEntityComponent {
	using PositiveInt = Schematyc::Range<0, 150, 0, 150, int32>;

public:
	Magazine() = default;
	
	template <typename Data>
	Magazine(Data)
		:	size_{ Data::magazine_size_ }
		,	capacity_{ Data::magazine_capacity_ }
		,	cooldown_time_{ try_get_cooldown_time_value<Data>::value }
	{}

	friend void ReflectType(Schematyc::CTypeDesc<Magazine>& desc);

	int32 size() const
	{
		return size_.value;
	}

	int32 capacity() const
	{
		return capacity_.value;
	}

	bool is_full() const
	{
		return size_.value >= capacity_.value;
	}

	bool is_empty() const
	{
		return size_.value <= 0;
	}

	int32 remove(const int32 amount);
	void restore(const int32 amount);

private:
	void Initialize() override;
	void OnShutDown() override;
	Cry::Entity::EventFlags GetEventMask() const override;
	void ProcessEvent(const SEntityEvent& event) override;
	ComponentEventPriority GetEventPriority() const override;

	TimerId cooldown_timer_id_{};
	EntityId parent_id_{};

	PositiveInt size_{};
	PositiveInt capacity_{};
	f32 cooldown_time_{};
	bool can_remove_bullet_{};
};

#endif // !MAGAZINE_H