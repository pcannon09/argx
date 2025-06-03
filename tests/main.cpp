/* tests/main.cpp
 * Owned and created by: pcannon09
 */

#include <iostream>

#include "../inc/Argx.hpp"

int main(int argc, char *argv[])
{
	argx::Argx mainArgx("main-args", argc, argv);

	{
		argx::ARGXOptions mainOptions;

		mainOptions.id = "help";
		mainOptions.param = "--help";
		mainOptions.sparam = "-h";
		mainOptions.info = "Show this help";
		mainOptions.hasSubParams = true;

		argx::ARGXOptions subOption;

		subOption.id = "version";
		subOption.param = "version";
		subOption.sparam = "v";
		subOption.hasSubParams = true;

		mainOptions.subParams.push_back(subOption);

		subOption.id = "this";
		subOption.param = "this";
		subOption.sparam = "t";
		subOption.hasSubParams = true;

		mainOptions.subParams.push_back(subOption);

		mainArgx.add(mainOptions);
		mainArgx.createDoc(argx::ARGXStyle::Professional, "", "");
	}

	if (mainArgx.getParam("help").exists)
	{
		std::cout << "-- HELP -----------------------------------------------------------------------------\n";
		std::cout << "| * This library is a simple parameter parser for C++                               |\n";
		std::cout << "| * NOTE: For more information on how this program is used, check the source code   |\n";
		std::cout << "| * [-h | --help [version]] Get this help                                           |\n";
		std::cout << "| * [            [version]] Get version help                                        |\n";
		std::cout << "-------------------------------------------------------------------------------------\n";

		if (mainArgx.getParam("version").subExists[0])
		{
			std::cout << "VERSION\n";

			if (mainArgx.getParam("this").subExists[0])
			{
				std::cout << "THIS\n";
			}
		}
	}

	return 0;
}

