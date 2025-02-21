#ifndef AMMO_BOX_H
#define AMMO_BOX_H

#include <string_view>
#include <algorithm>

template<typename Tag>
class AmmoBox {
	using Thumbnail = std::string_view;

public:
	AmmoBox() : AmmoBox("", 0, 0) {}
	AmmoBox(Thumbnail thumbnail, int32_t value, int32_t capacity)
		:	thumbnail_{ std::move(thumbnail) }
		,	size_{ std::move(value) }
		,	capacity_{ std::move(capacity) }
	{}

	int32_t size() const
	{
		return size_;
	}

	int32_t capacity() const
	{
		return capacity_;
	}

	bool full() const
	{
		return size_ >= capacity_;
	}

	bool is_empty() const
	{
		return size_ <= 0;
	}

	int32_t remove(int32_t amount)
	{
		const auto temp = size_ > amount ? amount : size_;
		size_ = std::clamp(size_ - amount, 0, capacity_);

		return temp;
	}

	void restore(int32_t amount)
	{
		size_ = std::clamp(size_ + amount, 0, capacity_);
	}

private:
	Thumbnail thumbnail_;
	int32_t size_;
	int32_t capacity_;
};

#endif // !AMMO_BOX_H