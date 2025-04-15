#pragma once

#include <deque>
#include <memory>
#include <mutex>
#include <condition_variable>

template <typename T>
class ThreadSafeQueue
{
	private:
		mutable std::mutex mx;
		std::deque<T> deque;
		std::condition_variable data_cond;

	public:
		ThreadSafeQueue(){};
		ThreadSafeQueue(ThreadSafeQueue & other);
		ThreadSafeQueue & operator=(
			const ThreadSafeQueue&) = delete;
		
	public:
		void push_back(const T & newVal);
		void push_front(const T & newVal);

		T pop_back(); // I prefer passing references, but this is a subject requirement
		T pop_front();

		void wait_and_pop_back(T & val);
		std::shared_ptr<T> wait_and_pop_back();
		void wait_and_pop_front(T & val);
		std::shared_ptr<T> wait_and_pop_front();

		bool empty() const;

		// non-blocking pops
		bool try_pop_back(T & val);
		bool try_pop_front(T & val);
		std::shared_ptr<T> try_pop_back();
		std::shared_ptr<T> try_pop_front();
};

template <typename T>
ThreadSafeQueue<T>::ThreadSafeQueue(ThreadSafeQueue & other) {
	std::lock_guard<std::mutex> lk(other.mx);
	deque = other.deque;
}

template <typename T>
void ThreadSafeQueue<T>::push_back(const T & newVal) {
	std::lock_guard<std::mutex> lk(mx);
	deque.push_back(newVal);
};

template <typename T>
void ThreadSafeQueue<T>::push_front(const T & newVal) {
	std::lock_guard<std::mutex> lk(mx);
	deque.push_front(newVal);
}

template <typename T>
T ThreadSafeQueue<T>::pop_back() {
	std::lock_guard<std::mutex> lk(mx);
	T val = deque.back();
	deque.pop_back();
	return val;
}

template <typename T>
T ThreadSafeQueue<T>::pop_front() {
	std::lock_guard<std::mutex> lk(mx);
	T val = deque.front();
	deque.pop_front();
	return val;
}

template <typename T>
void ThreadSafeQueue<T>::wait_and_pop_back(T & val) {
	std::unique_lock<std::mutex> lk(mx);
	data_cond.wait(lk, [this] {return !deque.empty();});
	val = deque.back();
	deque.pop_back();
}

template <typename T>
std::shared_ptr<T> ThreadSafeQueue<T>::wait_and_pop_back() {
	std::unique_lock<std::mutex> lk(mx);
	data_cond.wait(lk, [this] {return !deque.empty();});
	std::shared_ptr<T> res(std::make_shared<T> (deque.back()));
	deque.pop_back();
	return res;
}

template <typename T>
void ThreadSafeQueue<T>::wait_and_pop_front(T & val) {
	std::unique_lock<std::mutex> lk(mx);
	data_cond.wait(lk, [this] {return !deque.empty();});
	val = deque.front();
	deque.pop_front();
}

template <typename T>
std::shared_ptr<T> ThreadSafeQueue<T>::wait_and_pop_front() {
	std::unique_lock<std::mutex> lk(mx);
	data_cond.wait(lk, [this] {return !deque.empty();});
	std::shared_ptr<T> res(std::make_shared<T> (deque.front()));
	deque.pop_front();
	return res;
}

template <typename T>
bool ThreadSafeQueue<T>::empty() const {
	std::lock_guard<std::mutex> lk(mx);
	return deque.empty();
}

template <typename T>
bool ThreadSafeQueue<T>::try_pop_back(T & val) {
	std::lock_guard<std::mutex> lk(mx);
	if (deque.empty())
		return false;
	val = deque.pop_back();
	deque.pop_back();
	return true;
}

template <typename T>
std::shared_ptr<T> ThreadSafeQueue<T>::try_pop_back() {
	std::lock_guard<std::mutex> lk(mx);
	if (deque.empty())
		return std::shared_ptr<T>();
	std::shared_ptr<T> res(std::make_shared<T>(deque.back()));
	deque.pop_back();
	return res;
}

template <typename T>
bool ThreadSafeQueue<T>::try_pop_front(T & val) {
	std::lock_guard<std::mutex> lk(mx);
	if (deque.empty())
		return false;
	val = deque.pop_front();
	deque.pop_front();
	return true;
}

template <typename T>
std::shared_ptr<T> ThreadSafeQueue<T>::try_pop_front() {
	std::lock_guard<std::mutex> lk(mx);
	if (deque.empty())
		return std::shared_ptr<T>();
	std::shared_ptr<T> res(std::make_shared<T>(deque.front()));
	deque.pop_front();
	return res;
}
