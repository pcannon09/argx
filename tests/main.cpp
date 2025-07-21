/* tests/main.cpp
 * Owned and created by: pcannon09
 */

#include <iostream>

#include "../inc/macros.hpp"
#include "../inc/Argx.hpp"

int main(int argc, char *argv[])
{
#if defined(AS_PYTHON_PACKAGE)
	std::cout << "FATAL: YOU SHOULD NOT RUN THIS EXECUTABLE AS `AS_PYTHON_PACKAGE`, DO NOT USE THE MACRO IF YOU ARE WILLING TO EXECUTE ARGX\n";

	return -1;

#else
	std::string docStr;
	
	argx::Argx mainArgx("main-args", argc, argv);

	// Setup mainArgx
	{
		argx::ARGXOptions helpOption;
		argx::ARGXOptions versionOption;
		argx::ARGXOptions styleOption;

		helpOption.id = "help";
		helpOption.param = "--help";
		helpOption.sparam = "-h";
		helpOption.info = "Show this help";
		helpOption.hasSubParams = true;

		versionOption.id = "version";
		versionOption.param = "--version";
		versionOption.sparam = "-v";
		versionOption.info = "Show version message";
		versionOption.hasSubParams = false;

		styleOption.id = "style";
		styleOption.param = "--style";
		styleOption.sparam = "-s";
		styleOption.info = "Set the style of the documentation (simple OR professional)";
		styleOption.hasSubParams = true;
		
		argx::ARGXOptions versionSubOption;
		argx::ARGXOptions messageSubOption;
		argx::ARGXOptions styleSubOption;
		
		versionSubOption.id = "version";
		versionSubOption.param = "version";
		versionSubOption.sparam = "v";
		versionSubOption.info = "Show version help";
		versionSubOption.hasSubParams = false;
		
		messageSubOption.id = "message";
		messageSubOption.param = "message";
		messageSubOption.sparam = "m";
		messageSubOption.info = "Show a specific message";
		messageSubOption.hasSubParams = false;

 		styleSubOption.id = "simple";
 		styleSubOption.param = "simple";
 		styleSubOption.info = "Set simple documentation";
 		styleSubOption.hasSubParams = false;

		styleOption.subParams.push_back(styleSubOption);

 		styleSubOption.id = "simple";
 		styleSubOption.param = "professional";
 		styleSubOption.sparam = "pro";
 		styleSubOption.info = "Set professional documentation";
 		styleSubOption.hasSubParams = false;

		styleOption.subParams.push_back(styleSubOption);

		helpOption.subParams.push_back(versionSubOption);
		helpOption.subParams.push_back(messageSubOption);

		versionOption.subParams.push_back(versionSubOption);

		mainArgx.add(helpOption);
		mainArgx.add(versionOption);
		mainArgx.add(styleOption);
		
		std::string msg = "Simple documentation on how to use the ARGX test";

		std::cout << "HELP PARAM POSITION: " << (mainArgx.getArgPos(helpOption.param) < 0 ? mainArgx.getArgPos(helpOption.sparam) : mainArgx.getArgPos(helpOption.param)) << "\n";
		std::cout << "STYLE PARAM POSITION: " << (mainArgx.getArgPos(styleOption.param) < 0 ? mainArgx.getArgPos(styleOption.sparam) : mainArgx.getArgPos(styleOption.param)) << "\n";
		std::cout << "VERSION PARAM POSITION: " << (mainArgx.getArgPos(versionOption.param) < 0 ? mainArgx.getArgPos(versionOption.sparam) : mainArgx.getArgPos(versionOption.param)) << "\n";

		if (mainArgx.getParam("style").exists)
		{
			if (mainArgx.getArgc() > 2)
			{
				if (mainArgx.getMainArgs()[2] == "simple") docStr = mainArgx.createDocs(argx::ARGXStyle::Simple, "-- Docs ----", msg);
				else docStr = mainArgx.createDocs(argx::ARGXStyle::Professional, "-- Docs ----", msg);
			}

			else
			{
				std::cout << "Set one of those two values:\n";
				std::cout << "* simple\n";
				std::cout << "* professional\n";
				std::cout << "NOTE: You can code your own documentation by overriding the Argx::createDocs() function\n";

				return 1;
			}
		}
		
		else docStr = mainArgx.createDocs(argx::ARGXStyle::Professional, "-- Docs ----", msg);
	}

	// Check if help exists
	if (mainArgx.getArgc() <= 1)
	{
		std::cout << docStr << "\n";

		return 0;
	}

	if (mainArgx.getParam("help").exists)
	{
		argx::ARGXParam helpParam = mainArgx.getParam("help");

    	if (mainArgx.getSubParam(helpParam, "version"))
    		std::cout << "For more information, call the following parameter: `--version`\n";

    	else if (mainArgx.getSubParam(helpParam, "message"))
		{
			if (mainArgx.getArgc() > 3) std::cout << mainArgx.getMainArgs()[3] << "\n";
			else std::cout << "Enter a message in the third parameter as a string" << "\n";
		}

		std::cout << docStr << "\n";
	}

	if (mainArgx.getParam("version").exists)
	{
		std::cout << "ARGX Version information:\n";
		std::cout << "Version: " << std::to_string(ARGX_VERSION_MAJOR) << "." << std::to_string(ARGX_VERSION_MINOR) << "." << std::to_string(ARGX_VERSION_PATCH) << " " << ARGX_VERSION_STATE << "\n";
		std::cout << "Version Standard: " << std::to_string(ARGX_VERSION_STD) << "\n";
		std::cout << "Development Type: " << (ARGX_DEV ? "DEV" : "PRODUCTION") << "\n";
	}

	if (!mainArgx.compareArgs(mainArgx.getOptions(), mainArgx.getMainArgs()))
	{
		std::cout << "Argx: Unknown option `" + (mainArgx.getArgc() > 1 ? mainArgx.getMainArgs()[1] : "<UNKNOWN>") + "`\n";

		return 1;
	}

	return 0;
#endif
}

