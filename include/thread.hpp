#pragma once

#include <functional>
#include <thread>

/*
	I'm not sure why a thread needs a wrapper,
	it seems like unnecessary abstraction,
	but it's a requirement of the subject.
*/

class Thread
{
	private:
		const std::string _name;
		std::function<void()> _func;
		std::thread _thread;

	public:
		Thread(
			const std::string & name,
			std::function<void()> funcToExecute
		);
		Thread() = delete;
		Thread(Thread & other) = delete;
		Thread & operator=(const &Thread) = delete;

	public:
		void start();
		void stop();
};