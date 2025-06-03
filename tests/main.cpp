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
		argx::ARGXOptions mainOptions;

		mainOptions.id = "help";
		mainOptions.param = "--help";
		mainOptions.sparam = "-h";
		mainOptions.info = "Show this help";
		mainOptions.hasSubParams = true;

		argx::ARGXOptions versionSubOption;
		argx::ARGXOptions randomSubOption;

		versionSubOption.id = "version";
		versionSubOption.param = "version";
		versionSubOption.sparam = "v";
		versionSubOption.info = "Show version help";
		versionSubOption.hasSubParams = true;

		randomSubOption.id = "message";
		randomSubOption.param = "msg";
		randomSubOption.sparam = "m";
		randomSubOption.info = "Show this message \"Hello world\"";
		randomSubOption.hasSubParams = true;

		randomSubOption.id = "other";
		randomSubOption.param = "other";
		randomSubOption.sparam = "o";
		randomSubOption.info = "Show this message \"Goodbye world\"";
		randomSubOption.hasSubParams = false;

		mainOptions.subParams.push_back(versionSubOption);
		mainOptions.subParams.push_back(randomSubOption);

		mainArgx.add(mainOptions);
		docStr = mainArgx.createDocs(argx::ARGXStyle::Professional, "-- Docs ----", "This is a simple test for documentation using ARGX");
	}

	if (mainArgx.getParam("help").exists && mainArgx.findParam("help"))
	{
		argx::ARGXParam helpParam = mainArgx.getParam("help");

    	if (mainArgx.paramExists("version") && helpParam.subExists[mainArgx.findParam("version")])
    	{
			std::cout << "VERSION PARAM CALLED\n";

        	return 0;
    	}

    	if (mainArgx.paramExists("message") && helpParam.subExists[mainArgx.findParam("message")])
    	{
    		if (helpParam.subExists[mainArgx.findParam("message") + 1])
    		{
				std::cout << "NEXT\n";

				return 0;
    		}

			std::cout << "This is a random hard coded message string\n";

			return 0;
    	}

    	std::cout << docStr << "\n";
	}

	else if (mainArgx.getParam("version").exists && mainArgx.findParam("help"))
	{
		argx::ARGXParam versionParam = mainArgx.getParam("version");

		// if (mainArgx.paramExists("version") && versionParam.subExists[mainArgx.findParam("version") + 1])
		// {
		//
		// }

		std::cout << "Check the source code for the version...\n";

		return 0;
	}

	return 0;
}

