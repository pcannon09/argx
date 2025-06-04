/* tests/main.cpp
 * Owned and created by: pcannon09
 */

#include <iostream>

#include "../inc/macros.hpp"

#include "../inc/Argx.hpp"

int main(int argc, char *argv[])
{
	std::string docStr;
	
	argx::Argx mainArgx("main-args", argc, argv);

	// Setup mainArgx
	{
		argx::ARGXOptions helpOption;
		argx::ARGXOptions versionOption;

		helpOption.id = "help";
		helpOption.param = "--help";
		helpOption.sparam = "-h";
		helpOption.info = "Show this help";
		helpOption.hasSubParams = true;

		versionOption.id = "version";
		versionOption.param = "--version";
		versionOption.sparam = "-v";
		versionOption.info = "Show version message";
		versionOption.hasSubParams = true;
		
		argx::ARGXOptions versionSubOption;
		argx::ARGXOptions messageSubOption;
		
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
		
		helpOption.subParams.push_back(versionSubOption);
		helpOption.subParams.push_back(messageSubOption);

		versionOption.subParams.push_back(versionSubOption);

		mainArgx.add(helpOption);
		mainArgx.add(versionOption);
		
		docStr = mainArgx.createDocs(argx::ARGXStyle::Professional, "-- Docs ----", "This is a simple test for documentation using ARGX");
	}

	// Check if help exists
	if (mainArgx.getArgc() <= 1) std::cout << docStr << "\n";

	else if (mainArgx.getParam("help").exists)
	{
		argx::ARGXParam helpParam = mainArgx.getParam("help");

    	if (mainArgx.getSubParam(helpParam, "version"))
		{
			std::cout << "For more information, call the following parameter: `--version`\n";

			return 2;
		}

    	else if (mainArgx.getSubParam(helpParam, "message"))
		{
			if (mainArgx.getArgc() > 3) std::cout << mainArgx.getMainArgs()[3] << "\n";
			else std::cout << "Enter a message in the third parameter as a string" << "\n";

			return 0;
		}

		std::cout << docStr << "\n";

		return 0;
	}

	else if (mainArgx.getParam("version").exists)
	{
		std::cout << "ARGX Version information:\n";
		std::cout << "Version: " << std::to_string(ARGX_VERSION_MAJOR) << "." << std::to_string(ARGX_VERSION_MINOR) << "." << std::to_string(ARGX_VERSION_PATCH) << " " << ARGX_VERSION_STATE << "\n";
		std::cout << "Version Standard: " << std::to_string(ARGX_VERSION_STD) << "\n";
		std::cout << "Development Type: " << ARGX_DEV << "\n";
	}
	
	else
	{
		std::cout << "Argx: Unknown option `" + (mainArgx.getArgc() > 1 ? mainArgx.getMainArgs()[1] : "<UNKNOWN>") + "`\n";

		return 1;
	}

	return 0;
}

