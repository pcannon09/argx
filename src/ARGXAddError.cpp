#include <utility>
#include <string>

#include "../inc/ARGXAddError.hpp"

namespace argx
{
	ARGXAddError::ARGXAddError(const std::string &msg, const std::string &help)
		: msg(msg), help(help)
	{ }

	std::pair<const char* /*Error*/, const char* /*Help*/> ARGXAddError::what() const noexcept
	{ return { this->msg.c_str(), this->help.c_str() }; }
}

