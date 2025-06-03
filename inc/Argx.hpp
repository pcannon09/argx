/* inc/argx.hpp
 * Owned and created by: pcannon09
 */

#pragma once

#include "../inc/macros.hpp"

#if __cplusplus >= 201102L
#include <string>
#include <vector>

#include "../inc/types.hpp"

namespace argx
{
	class Argx
	{
	private:
		std::string id;

		static std::vector<ARGXOptions> options;
		static std::vector<std::string> *mainArgs;

	public:
		Argx(const std::string &id, int argc, char *argv[]);
		Argx();
		~Argx();

		void add(ARGXOptions option) const;
		ARGXParam getParam(const std::string& id);

		std::string createDocs(ARGXStyle style, const std::string &title, const std::string &mainInfo);

		std::vector<std::string> getMainArgs() const;
		std::vector<ARGXOptions> getOptions() const;
		std::string getID() const;
	};
}

#else
# 	error "Must compile with C++11 support or newer"
# 	pragma message("Current standard is " ARGX_TOSTRING(__cplusplus))
#endif
