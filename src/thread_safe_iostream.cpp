#include "thread_safe_iostream.hpp"

thread_local ThreadSafeIOStream threadSafeCout;
std::mutex ThreadSafeIOStream::_mx;

void ThreadSafeIOStream::setPrefix(const std::string & prefix)
{
  const std::lock_guard<std::mutex> lk(_mx);
	_prefix = prefix;
}

ThreadSafeIOStream & ThreadSafeIOStream::operator<<(std::ostream& (*pf)(std::ostream&))
{
	const std::lock_guard<std::mutex> lk(_mx);
	pf(_buffer);
  _flush();
	return *this;
}

ThreadSafeIOStream & ThreadSafeIOStream::operator>>(std::istream& (*pf)(std::istream&))
{
	const std::lock_guard<std::mutex> lk(_mx);
	pf(std::cin);
	return *this;
}

void ThreadSafeIOStream::_flush(void)
{
	std::cout << _prefix << _buffer.str();
	_buffer.str("");
	_buffer.clear();
}
