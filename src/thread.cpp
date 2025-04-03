#include "thread.hpp"

Thread::Thread(const std::string & name, std::function<void()> funcToExecute) 
	:
	_name(name),
	_func(funcToExecute)
{
};

void Thread::start()
{
	if (!_thread.joinable())
		_thread = std::thread(_func);
}

void Thread::stop()
{
	if (_thread.joinable())
		_thread.join();
}