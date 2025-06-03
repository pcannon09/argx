#pragma once

#if __cplusplus >= 201102L
#include <string>
#include <vector>

namespace argx
{
	enum class ARGXStyle
	{
		Professional,
		Simple,
		Stylish,
		Custom,
	};

	typedef struct ARGXError
	{
		std::string type;
		std::string error;
		std::string help;

		int code;
	} ARGXError;

	typedef struct ARGXOptions
	{
		/** ID (id)
		 * @brief ID of the option
		 */
		std::string id;

		/** Param (param):
		 * @brief EXAMPLE:
		 * --help, --version
		 */
		std::string param;

		/** Short Param (sparam):
		 * @brief
		 */
		std::string sparam;
		std::string info;

		bool hasSubParams = false;

		std::vector<ARGXOptions> subParams;
	} ARGXOptions;

	typedef struct ARGXParam
	{
		bool exists = false;
		std::vector<bool> subExists;
	} ARGXParam;
}

#else
# 	error "Must compile with C++11 support or newer"
# 	pragma message("Current standard is " ARGX_TOSTRING(__cplusplus))
#endif

