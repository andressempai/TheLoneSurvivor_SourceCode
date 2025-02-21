#ifndef OBJECT_POINTER_H
#define OBJECT_POINTER_H

#include <type_traits>

template <typename T>
class ObjectPointer {
public:
	ObjectPointer() = default;

	template <typename U>
	ObjectPointer(U* ptr) : ptr_(ptr) {}

	T* get() const
	{
		return ptr_;
	}

	template <typename U, typename = std::enable_if_t<std::is_convertible_v<U, T>>>
	ObjectPointer& operator=(U* ptr)
	{
		ptr_ = ptr;
		return *this;
	}

	T* operator->() const
	{
		return ptr_;
	}

	T& operator*() const
	{
		return *ptr_;
	}

	operator T* () const
	{
		return ptr_;
	}

	operator bool() const
	{
		return ptr_;
	}

private:
	T* ptr_{};
};

template <typename U>
ObjectPointer(U*) -> ObjectPointer<U>;

#endif // !OBJECT_POINTER_H