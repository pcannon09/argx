#pragma once

// NOTE:
// Left behind with the old C++ standard (C++11) as it is not being maintained anymore
#if __cplusplus >= 201103L || defined(ARGX_AS_PYTHON_PACKAGE)

#include <string>
#include <utility>

namespace argx
{
	class [[deprecated("ARGXAddError is deprecated, there is no direct replacement, this function is going to be removed in version 1.4.0")]]
		ARGXAddError
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

