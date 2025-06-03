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
		versionSubOption.info = "Show this message \"Hello world\"";
		randomSubOption.hasSubParams = true;

		mainOptions.subParams.push_back(versionSubOption);
		mainOptions.subParams.push_back(randomSubOption);

		mainArgx.add(mainOptions);
		docStr = mainArgx.createDocs(argx::ARGXStyle::Professional, "-- Docs ----", "This is the main documentation for ARGX");
	}

	if (mainArgx.getParam("help").exists)
	{
		std::cout << docStr << "\n";
	}

	return 0;
}

