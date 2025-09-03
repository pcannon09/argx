#pragma once

#if __cplusplus >= 201402L || defined(ARGX_AS_PYTHON_PACKAGE)
#include <string>
#include <vector>

namespace argx
{
	enum class ARGXStyle
	{
		Professional,
		Simple,
	};

	typedef struct [[deprecated("ARGXError is deprecated, there is no direct replacement, this type definition is going to be removed in version 1.4.0")]]
		ARGXError
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

		/**
		 * @brief Param (param). EXAMPLE:
		 * --help, --version
		 */
		std::string param;

		/**
		 * @brief Short Param (sparam):
		 */
		std::string sparam;
		std::string info;

		/**
		 * @brief Set default tag to categorize arguments
		 **/
		std::string tag = "default";
		std::string defaultValue;

		bool hasSubParams = false;
		bool hasAnySubParams = false;
		bool required = false;

		std::vector<ARGXOptions> subParams;
	} ARGXOptions;

	typedef struct ARGXParam
	{
		bool exists = false;
		std::vector<bool> subExists;
	} ARGXParam;
}

#else
# 	error "Must compile with C++14 support or newer"
# 	pragma message("Current standard is " ARGX_TOSTRING(__cplusplus))
#endif

