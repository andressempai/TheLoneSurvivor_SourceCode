#ifndef TIMER_MANAGER_H
#define TIMER_MANAGER_H

#include "Timer.h"

#include <memory>
#include <vector>
#include <utility>
#include <algorithm>

class TimerId {
	friend class TimerManager;

	bool operator==(const TimerId& other) const
	{
		return id_ == other.id_;
	}

	static inline uint64 timer_id_{ 0 };
	uint64 id_{ timer_id_++ };
};

class TimerWrapper {
public:
	template <typename T>
	TimerWrapper(T&& obj) : pimpl_{ std::make_shared<TimerModel<T>>(std::forward<T>(obj)) } {}

	void consume(float delta_time)
	{
		if (pimpl_)
			pimpl_->consume(delta_time);
	}

	void pause()
	{
		if (pimpl_)
			pimpl_->pause();
	}

	void stop()
	{
		if (pimpl_)
			pimpl_->stop();
	}

	void resume()
	{
		if (pimpl_)
			pimpl_->resume();
	}

	void finish_it()
	{
		if (pimpl_)
			pimpl_->finish_it();
	}

	bool is_time_up() const
	{
		if (pimpl_)
			return pimpl_->is_time_up();

		return false;
	}

private:
	struct TimerConcept {
		virtual ~TimerConcept() = default;

		virtual void consume(float delta_time) = 0;
		virtual void pause() = 0;
		virtual void resume() = 0;
		virtual void stop() = 0;
		virtual void finish_it() = 0;
		virtual bool is_time_up() const = 0;
	};
	
	template <typename T>
	struct TimerModel final : TimerConcept {
		template <typename Obj>
		TimerModel(Obj&& obj) : obj_{ std::forward<Obj>(obj) } {}

		void consume(float delta_time) override { obj_.consume(delta_time); }
		void pause() override { obj_.pause(); }
		void resume() override { obj_.resume(); }
		void stop() override { obj_.stop(); }
		void finish_it() override { obj_.finish_it(); }
		bool is_time_up() const override { return obj_.is_time_up(); }

		T obj_{};
	};

	std::shared_ptr<TimerConcept> pimpl_;
};

class TimerManager {
public:
	using TimerContainer = std::vector<std::pair<TimerId, TimerWrapper>>;
	using Iterator = TimerContainer::iterator;
	using ConstIterator = TimerContainer::const_iterator;

	TimerManager(const TimerManager&) = delete;
	TimerManager(TimerManager&&) = delete;
	TimerManager& operator=(const TimerManager&) = delete;
	TimerManager& operator=(TimerManager&&) = delete;

	static TimerManager& instance()
	{
		static TimerManager instance{};
		return instance;
	}

	void set_manager_capacity(std::size_t capacity)
	{
		active_timers_.reserve(capacity);
		temp_timers_.reserve(capacity);
	}

	template <typename Callable, typename... Params>
	auto& make_timer(TimerId timer_id, f32 time, bool can_loop, Callable callback, Params&&... params)
	{
		return active_timers_.emplace_back(std::make_pair(
			timer_id, TimerWrapper{ Temporizador<Params...>{ time, can_loop, callback, std::forward<Params>(params)... } }));
	}

	void consume_all_timers(f32 delta_seconds)
	{
		std::for_each(begin(active_timers_), end(active_timers_), [=](auto& pair) { pair.second.consume(delta_seconds); });
	}

	void pause_all_timers()
	{
		std::for_each(begin(active_timers_), end(active_timers_), [](auto& pair) { pair.second.pause(); });
	}

	void resume_all_timers()
	{
		std::for_each(begin(active_timers_), end(active_timers_), [](auto& pair) { pair.second.resume(); });
	}

	void finish_all_timers()
	{
		std::for_each(begin(active_timers_), end(active_timers_), [](auto& pair) { pair.second.finish_it(); });
	}

	void prepare_for_remove_finished_timers()
	{
		std::for_each(begin(active_timers_), end(active_timers_), [this](auto& pair)
		{
			if (not pair.second.is_time_up())
				temp_timers_.emplace_back(std::move(pair));
		});
	}

	void remove_finished_timers()
	{
		active_timers_.clear();
		active_timers_.swap(temp_timers_);
	}

	void remove_timer(TimerId timer_id)
	{
		std::vector<Iterator> client_timer_container{};
		client_timer_container.reserve(active_timers_.capacity());

		for (auto iter = begin(active_timers_); iter != end(active_timers_); ++iter)
			if (iter->first == timer_id)
				client_timer_container.emplace_back(iter);

		if (client_timer_container.size() == 1)
			client_timer_container[0]->second.stop();
		else {
			auto iter = std::find_if(begin(client_timer_container), end(client_timer_container), [](auto iter)
			{
				return not iter->second.is_time_up();
			});
			if (iter != std::end(client_timer_container))
				(*iter)->second.stop();
		}
	}

	void clear()
	{
		active_timers_.clear();
		temp_timers_.clear();
	}

	bool is_timer_active(TimerId timer_id) const
	{
		return get_active_timer(timer_id) != end(active_timers_);
	}

	Iterator get_active_timer(TimerId timer_id)
	{
		return std::find_if(begin(active_timers_), end(active_timers_), [timer_id](const auto& pair)
		{
			if (pair.first == timer_id)
				return not pair.second.is_time_up();

			return false;
		});
	}

	ConstIterator get_active_timer(TimerId timer_id) const
	{
		return std::find_if(begin(active_timers_), end(active_timers_), [timer_id](const auto& pair)
		{
			if (pair.first == timer_id)
				return not pair.second.is_time_up();

			return false;
		});
	}

private:
	explicit TimerManager(std::size_t capacity = 16)
	{
		active_timers_.reserve(capacity);
		temp_timers_.reserve(capacity);
	}

	std::vector<std::pair<TimerId, TimerWrapper>> active_timers_{};
	std::vector<std::pair<TimerId, TimerWrapper>> temp_timers_{};
};

#endif	// !TIMER_MANAGER_H