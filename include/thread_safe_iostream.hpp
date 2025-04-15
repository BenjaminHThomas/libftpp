#pragma once

#include <mutex>
#include <string>
#include <sstream>
#include <iostream>

class ThreadSafeIOStream
{
	private:
		static std::mutex _mx;
		std::string _prefix;
		std::ostringstream _buffer;
		void _flush(void);

	public: // ostream
		template <typename T>
		ThreadSafeIOStream & operator <<(const T & obj) {
      const std::lock_guard<std::mutex> lk(_mx);
      _buffer << obj;
      return *this;
    }
		ThreadSafeIOStream & operator <<(std::ostream& (*pf)(std::ostream&));

	public: // istream
		template <typename T>
		ThreadSafeIOStream & operator >>(T & obj) {
      const std::lock_guard<std::mutex> lk(_mx);
      std::cout << _prefix << std::flush;
      std::cin >> obj;
      return *this;
    }
		ThreadSafeIOStream & operator >>(std::istream& (*pf)(std::istream&));

	public: // misc
		template <typename T>
		void prompt(const std::string & question, T & dest) {
	    const std::lock_guard<std::mutex> lk(_mx);
	    dest << _prefix << question;
    }
		void setPrefix(const std::string & prefix);
};

extern thread_local ThreadSafeIOStream threadSafeCout;
