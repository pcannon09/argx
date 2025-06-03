/* tests/main.cpp
 * Owned and created by: pcannon09
 */

#include <iostream>

#include "../inc/Argx.hpp"

int main(int argc, char *argv[])
{
	std::string docStr;
	
	argx::Argx mainArgx("main-args", argc, argv);
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
		versionOption.info = "Show fake version message";
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
		messageSubOption.info = "Show this message \"Hello world\"";
		messageSubOption.hasSubParams = false;
		
		helpOption.subParams.push_back(versionSubOption);
		helpOption.subParams.push_back(messageSubOption);

		versionOption.subParams.push_back(versionSubOption);

		mainArgx.add(helpOption);
		mainArgx.add(versionOption);
		
		docStr = mainArgx.createDocs(argx::ARGXStyle::Professional, "-- Docs ----", "This is a simple test for documentation using ARGX");
	}

	auto a = mainArgx.getOptions();

	for (const auto &x : a)
	{
		std::cout << x.id << "\n";
	}
	
	// Check if help exists
	if (mainArgx.getParam("help").exists)
	{
		argx::ARGXParam helpParam = mainArgx.getParam("help");

    	if (mainArgx.getSubParam(helpParam, "version"))
		{
			std::cout << "VERSION PARAM CALLED\n";
			return 0;
		}

    	else if (mainArgx.getSubParam(helpParam, "message"))
		{
			std::cout << "This is a random hard coded message string\n";
			
			if (mainArgx.getArgc() > 3) std::cout << "And this is your message: " << mainArgx.getMainArgs()[3] << "\n";

			return 0;
		}

		// Default help output
		std::cout << docStr << "\n";

		return 0;
	}

	else if (mainArgx.getParam("version").exists)
	{
		std::cout << "Fake version, WOMP WOMP\n";
	}

	return 0;
}

