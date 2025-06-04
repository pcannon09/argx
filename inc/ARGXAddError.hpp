#pragma once

#if __cplusplus >= 201103L

#include <string>
#include <utility>

namespace argx
{
	class ARGXAddError
	{
	private:
		std::string msg;
		std::string help;

	public:
		ARGXAddError(const std::string &msg, const std::string &help = "");

		std::pair<const char* /*Error*/, const char* /*Help*/> what() const noexcept;
	};
}

#else
# 	error "Must compile with C++11 support or newer"
# 	pragma message("Current standard is " ARGX_TOSTRING(__cplusplus))
#endif

