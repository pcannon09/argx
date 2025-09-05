/* src/argx.cpp
 * Owned and created by: pcannon09
 */

#include <string>
#include <codecvt>
#include <locale>
#include <iostream>

#include "../inc/Argx.hpp"
#include "../inc/types.hpp"

namespace argx
{
	// PRIVATE
	std::vector<ARGXOptions> Argx::options;
	std::vector<std::string> *Argx::mainArgs = nullptr;

	unsigned int Argx::mainArgc;

	// PUBLIC:
#if defined(ARGX_AS_PYTHON_PACKAGE)
	Argx::Argx(const std::string &id, const std::vector<std::string> &args)
		: id(id)
	{
		this->mainArgs = new std::vector<std::string>(args);
		this->mainArgc = args.size();
	}

	void Argx::destroy()
	{ this->~Argx(); }

#else
	Argx::Argx(const std::string &id, int argc, char *argv[])
		: id(id)
	{
        this->mainArgs = new std::vector<std::string>(argv, argv + argc);
        this->mainArgc = argc;

        if (!this->mainArgs)
			std::cerr << "`Argx::mainArgs` for ID of " + id + " does not exist (NULL value)";
	}
#endif

	Argx::Argx()
	{ }

	Argx::~Argx()
	{
		if (this->mainArgs) delete this->mainArgs; this->mainArgs = nullptr;
	}

	int Argx::getArgPos(const std::string &arg)
	{
		if (!this->mainArgs)
			return -1;

		for (size_t i = 0; i < this->mainArgs->size(); ++i)
		{
			if (this->mainArgs->at(i) == arg)
				return static_cast<int>(i);
		}

		return -1;
	}

	void Argx::add(ARGXOptions option) const
	{
		ARGXError error;
		error.type = "success";
		error.code = 0;

		this->options.emplace_back(option);
	}

	int Argx::findParam(const std::string &id)
	{
    	// First check if it's a main parameter
    	for (size_t i = 0; i < this->options.size(); i++)
    	{
        	if (this->options[i].id == id)
        	{
            	// Check if this main parameter exists in arguments
            	for (const std::string &arg : *this->mainArgs)
            	{
                	if (arg == this->options[i].param || arg == this->options[i].sparam)
                	{
                    	return static_cast<int>(i);
                	}
            	}
        	}
    	}

    	// Then look for sub-parameters
    	for (const auto &opt : this->options)
    	{
        	// Check if the parent option exists in the arguments
        	bool parentExists = false;

        	for (const std::string &arg : *this->mainArgs)
        	{
            	if (arg == opt.param || arg == opt.sparam)
            	{
                	parentExists = true;
                	break;
            	}
        	}

        	if (parentExists)
        	{
            	// Find the index of the requested sub-parameter
            	for (size_t i = 0; i < opt.subParams.size(); i++)
            	{
                	if (opt.subParams[i].id == id) return static_cast<int>(i);
            	}
        	}
    	}

    	return -1; // Not found
	}

	bool Argx::paramExists(const std::string &id)
	{
		if (this->findParam(id) >= 0) return true;;

		return false;
	}


	
	ARGXParam Argx::getParam(const std::string &id)
	{
		if (this->mainArgc <= 1) return {};

		ARGXParam result;

		// First, check if this is a top-level option
		for (const auto &opt : this->options)
		{
			if (opt.id == id)
			{
				// Find the position of the main option in arguments
				int mainOptionPos = -1;

				for (size_t i = 0; i < this->mainArgs->size(); ++i)
				{
					if ((*this->mainArgs)[i] == opt.param || (*this->mainArgs)[i] == opt.sparam)
					{
						result.exists = true;
						mainOptionPos = i;
						break;
					}
				}

				if (result.exists)
				{
					if (opt.hasSubParams || opt.hasAnySubParams)
					{
						// Check each sub-parameter
						for (const auto &sub : opt.subParams)
						{
							bool subMatched = false;

							// Look for sub-parameters after the main option
							for (size_t i = mainOptionPos + 1; i < this->mainArgs->size(); ++i)
							{
								if ((*this->mainArgs)[i] == sub.param || (*this->mainArgs)[i] == sub.sparam)
								{
									subMatched = true;
									break;
								}
							}

							result.subExists.push_back(subMatched);
						}
					}

					return result;
				}
			}
		}

		// If not found as top-level, check if it's a sub-parameter
		for (const auto &opt : this->options)
		{
			// Find if the parent option exists and get its position
			size_t parentPos = -1;

			for (size_t i = 0; i < this->mainArgs->size(); ++i)
			{
				if ((*this->mainArgs)[i] == opt.param || (*this->mainArgs)[i] == opt.sparam)
				{
					parentPos = i;
					break;
				}
			}

			if (parentPos > -1 && (opt.hasSubParams || opt.hasAnySubParams))
			{
				// Check if the requested sub-parameter exists after the parent
				for (const auto &sub : opt.subParams)
				{
					if (sub.id == id)
					{
						for (size_t i = parentPos + 1; i < this->mainArgs->size(); ++i)
						{
							if ((*this->mainArgs)[i] == sub.param || (*this->mainArgs)[i] == sub.sparam)
							{
								result.exists = true;
								break;
							}
						}

						if (!result.exists && parentPos + 1 < this->mainArgs->size())
						{
							std::string nextArg = (*this->mainArgs)[parentPos + 1];

							if (nextArg == sub.param || nextArg == sub.sparam) result.exists = true;
						}

						// Handle any sub-sub-parameters if they exist
						if (result.exists && (sub.hasSubParams || sub.hasAnySubParams))
						{
							for (const auto &subsub : sub.subParams)
							{
								bool subsubMatched = false;

								for (size_t i = 0; i < this->mainArgs->size(); ++i)
								{
									if ((*this->mainArgs)[i] == subsub.param || (*this->mainArgs)[i] == subsub.sparam)
									{
										subsubMatched = true;
										break;
									}
								}

								result.subExists.push_back(subsubMatched);
							}
						}

						return result;
					}
				}
			}
		}

		return result;
	}

	bool Argx::getSubParam(const argx::ARGXParam &param, const std::string &id)
	{ return this->paramExists(id) && param.subExists[this->findParam(id)]; }

	std::string Argx::createDocs(ARGXStyle style, const std::string &title, const std::string &mainInfo)
	{
		std::string contentStr;

		if (style == ARGXStyle::Professional)
		{
			for (const auto &x : this->options)
			{
				// Main option header line
				contentStr += "ID: " + x.id + "\n";
				contentStr += "[ " + x.sparam + " | " + x.param;

				if (x.hasSubParams && !x.subParams.empty())
				{
					contentStr += " [ ";

					for (size_t i = 0; i < x.subParams.size(); ++i)
					{
						const auto &sub = x.subParams[i];

						contentStr += sub.param;

						if (i < x.subParams.size() - 1) contentStr += " | ";
						else if (i <= x.subParams.size()) contentStr += ' ';
					}

					contentStr += "] ] ";
				}

				else contentStr += " ] ";

				contentStr += x.info + "\n";

				// Print all sub-options with sparam and param, aligned with ideographic spaces if there are
				if (x.hasSubParams && !x.subParams.empty())
				{
					for (const auto &sub : x.subParams)
					{
						// Create ideographic spaces matching the length of main param for alignment
						std::wstring wideSpaces(x.param.size(), L'\u3000');
						std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
						std::string spacing = converter.to_bytes(wideSpaces);

						contentStr += spacing + "  [ " + sub.sparam + " | " + sub.param + " ] " + sub.info + "\n";
					}
				}
			}
		}

		else if (style == ARGXStyle::Simple)
		{
			for (const auto &x : this->options)
			{
				contentStr += x.sparam + ", " + x.param + " - " + x.info + "\n";

				if (x.hasSubParams && !x.subParams.empty())
				{
					for (const auto &sub : x.subParams)
					{
						contentStr += "  " + sub.sparam + ", " + sub.param + " - " + sub.info + "\n";
					}
				}
			}
		} // ARGXStyle

		return title + "\n" + mainInfo + "\n" + contentStr;
	}

	
	bool Argx::compareArgs(std::vector<ARGXOptions> options, std::vector<std::string> argv)
	{
    	for (size_t i = 1; i < argv.size(); ++i)
    	{
        	const std::string &arg = argv[i];

        	bool found = false;
        	bool hasSubParams = false;
        	bool hasAnySubParams = false;

        	ARGXOptions matchedOption;

        	// Find the matching option
        	for (const auto &option : options)
        	{
            	if (option.sparam == arg || option.param == arg)
            	{
                	found = true;
                	hasSubParams = option.hasSubParams;
					hasAnySubParams = option.hasAnySubParams;
                	matchedOption = option;

                	break;
            	}
        	}

        	if (!found) return false;

        	if (hasSubParams || hasAnySubParams)
        	{
            	// Check if there's a next argument
            	if (i + 1 < argv.size())
            	{
                	const std::string &nextArg = argv[i + 1];

                	// Check if next argument is a sub-parameter
                	bool isSubParam = false;

                	if (!nextArg.empty() && nextArg[0] != '-')
                	{
                    	// Validate if it's a valid sub-parameter
                    	for (const auto &subOption : matchedOption.subParams)
                    	{
                        	if (subOption.param == nextArg || subOption.sparam == nextArg)
                        	{
                            	isSubParam = true;

                            	break;
                        	}
                    	}

                    	if (isSubParam) ++i; // Get the sub-parameter
                   		else if (hasSubParams || hasAnySubParams) return false;
                	}

                	else if (hasSubParams || hasAnySubParams) return false;
				}
        	}
    	}

    	return true;
	}

	std::vector<std::string> Argx::getMainArgs() const
	{ return *this->mainArgs; }

	int Argx::getArgc() const
	{ return this->mainArgc; }

	std::vector<ARGXOptions> Argx::getOptions() const
	{ return this->options; }

	std::string Argx::getID() const
	{ return this->id; }
}

