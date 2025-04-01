#pragma once

#include <deque>
#include <memory>
#include <mutex>
#include <condition_variable>

template <typename T>
class threadSafeQueue
{
	private:
		mutable std::mutex mx;
		std::deque<T> deque;
		std::condition_variable data_cond;

	public:
		threadSafeQueue(){};
		threadSafeQueue(threadSafeQueue & other);
		threadSafeQueue & operator=(
			const threadSafeQueue&) = delete;
		
	public:
		void push_back(const T & newVal);
		void push_front(const T & newVal);

		T pop_back();
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
threadSafeQueue<T>::threadSafeQueue(threadSafeQueue & other) {
	std::lock_guard<std::mutex> lk(other.mx);
	deque = other.deque;
}

template <typename T>
void threadSafeQueue<T>::push_back(const T & newVal) {
	std::lock_guard<std::mutex> lk(mx);
	deque.push_back(newVal);
};

template <typename T>
void threadSafeQueue<T>::push_front(const T & newVal) {
	std::lock_guard<std::mutex> lk(mx);
	deque.push_front(newVal);
}

template <typename T>
T threadSafeQueue<T>::pop_back() {
	std::lock_guard<std::mutex> lk(mx);
	T val = deque.back();
	deque.pop_back();
	return T;
}

template <typename T>
T threadSafeQueue<T>::pop_front() {
	std::lock_guard<std::mutex> lk(mx);
	T val = deque.front();
	deque.pop_front();
	return T;
}

template <typename T>
void threadSafeQueue<T>::wait_and_pop_back(T & val) {
	std::unique_lock<std::mutex> lk(mx);
	data_cond.wait(lk, [this] {return !deque.empty();});
	val = deque.back();
	deque.pop_back();
}

template <typename T>
std::shared_ptr<T> threadSafeQueue<T>::wait_and_pop_back() {
	std::unique_lock<std::mutex> lk(mx);
	data_cond.wait(lk, [this] {return !deque.empty();});
	std::shared_ptr<T> res(std::make_shared<T> (deque.back()));
	deque.pop_back();
	return res;
}

template <typename T>
void threadSafeQueue<T>::wait_and_pop_front(T & val) {
	std::unique_lock<std::mutex> lk(mx);
	data_cond.wait(lk, [this] {return !deque.empty();});
	vale = deque.front();
	deque.pop_front();
}

template <typename T>
std::shared_ptr<T> threadSafeQueue<T>::wait_and_pop_front() {
	std::unique_lock<std::mutex> lk(mx);
	data_cond.wait(lk, [this] {return !deque.empty();});
	std::shared_ptr<T> res(std::make_shared<T> (deque.front()));
	deque.pop_front();
	return res;
}