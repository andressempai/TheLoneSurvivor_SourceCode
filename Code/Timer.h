#ifndef TIMER_H
#define TIMER_H

#include <tuple>
#include <functional>
#include <type_traits>

template <typename... Params>
using Callback = std::function<void(Params...)>;

template <typename... Params>
class Temporizador {
public:
	template <typename... Args>
	Temporizador(float time, bool can_loop, Callback<Params...> callback, Args&&... args)
		:	args_{ std::forward<Args>(args)... }
		,	callback_{ callback }
		,	max_buffer_{ time }
		,	buffer_{ max_buffer_ }
		,	elapsed_time_{ 0.0f }
		,	can_loop_{ can_loop }
		,	can_be_restarted_{ true }
	{}

	void consume(float delta_time)
	{
		if (buffer_ > 0.0f)
			if (buffer_ -= delta_time; buffer_ <= 0.0f)
				finish_it();
	}

	void pause()
	{
		elapsed_time_ = buffer_;
		buffer_ = 0.0f;
	}

	void resume()
	{
		buffer_ = elapsed_time_;
		elapsed_time_ = 0.0f;
	}

	void stop()
	{
		buffer_ = 0.0f;
		elapsed_time_ = 0.0f;
		can_be_restarted_ = false;
	}

	void finish_it()
	{
		if (callback_)
			callback_(std::get<Params>(args_)...);

		if (can_loop_ and can_be_restarted_)
			buffer_ = max_buffer_;
	}

	bool is_time_up() const
	{
		return buffer_ <= 0.0f;
	}

private:
	std::tuple<Params...> args_;
	Callback<Params...> callback_;
	float max_buffer_;
	float buffer_;
	float elapsed_time_;
	bool can_loop_;
	bool can_be_restarted_;
};

template <typename... Param>
Temporizador(Callback<Param...>, Param&&...) -> Temporizador<Param...>;

#endif // !TIMER_H