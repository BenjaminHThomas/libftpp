#pragma once

#include <queue>
#include <memory>
#include <mutex>
#include <condition_variable>

template <typename T>
class thread_safe_queue
{
	private:
		mutable std::mutex mx;
		std::queue<T> data_queue;
		std::condition_variable data_cond;

	public:
		thread_safe_queue(){};
		thread_safe_queue(thread_safe_queue & other);
		
	public:
		void push(T new_val);
		void wait_and_pop(T & val);
		std::shared_ptr<T> wait_and_pop();
		bool try_pop(T & val);
		std::shared_ptr<T> try_pop();
		bool empty() const;
};

template <typename T>
thread_safe_queue<T>::thread_safe_queue(thread_safe_queue & other) {
	std::lock_guard<std::mutex> lk(other.mx);
	data_queue = other.data_queue;
}