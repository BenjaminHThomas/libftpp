#pragma once

#include <mutex>
#include <string>
#include <sstream>

class ThreadSafeIOStream
{
	private:
		static std::mutex _m;
		std::string _prefix;
		std::ostringstream _buffer;

	public: // ostream
		template <typename T>
		ThreadSafeIOStream & operator <<(T & obj);
		ThreadSafeIOStream & operator <<(std::ostream& (*pf)(std::ostream&));

	public: // istream
		template <typename T>
		ThreadSafeIOStream & operator >>(T & obj);
		ThreadSafeIOStream & operator >>(std::istream& (*pf)(std::istream&));

	public: // misc
		template <typename T>
		void prompt(const std::string & question, T & dest);
		void setPrefix(const std::string & prefix);

};

extern ThreadSafeIOStream threadSafeCout;