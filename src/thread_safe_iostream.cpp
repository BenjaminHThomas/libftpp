#include "thread_safe_iostream.hpp"

thread_local ThreadSafeIOStream threadSafeCout;
std::mutex ThreadSafeIOStream::_mx;

void ThreadSafeIOStream::setPrefix(const std::string & prefix)
{
	_prefix = prefix;
}

template <typename T>
void ThreadSafeIOStream::prompt(const std::string & question, T & dest)
{
	const std::lock_guard<std::mutex> lk(_mx);
	dest << _prefix << " ";
	dest << question << "\n";
}

template <typename T>
ThreadSafeIOStream & ThreadSafeIOStream::operator<<(const T & obj)
{
	const std::lock_guard<std::mutex> lk(_mx);
	_buffer << obj;
	return *this;
}

ThreadSafeIOStream & ThreadSafeIOStream::operator<<(std::ostream& (*pf)(std::ostream&))
{
	const std::lock_guard<std::mutex> lk(_mx);
	pf(_buffer);
	return *this;
}

template <typename T>
ThreadSafeIOStream & ThreadSafeIOStream::operator>>(T & obj)
{
	const std::lock_guard<std::mutex> lk(_mx);
	_buffer >> obj;
	return *this;
}

ThreadSafeIOStream & ThreadSafeIOStream::operator>>(std::istream& (*pf)(std::istream&))
{
	const std::lock_guard<std::mutex> lk(_mx);
	_istream.str(_buffer.str());
	_istream.clear(); // clear err flags
	_istream.seekg(0, std::ios::beg); // reset read pos to start
	pf(_istream);
	return *this;
}

void ThreadSafeIOStream::flush(void)
{
	const std::lock_guard<std::mutex> lk(_mx);
	std::cout << _prefix << " ";
	std::cout << _buffer.str() << "\n";
	_buffer.str("");
	_buffer.clear();
}
