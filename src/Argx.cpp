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
	}
#endif

	Argx::Argx()
	{ }

	Argx::~Argx()
	{
		delete this->mainArgs; this->mainArgs = nullptr;
	}

	int Argx::getArgIDPos(const std::string &arg)
	{
		ARGXOptions option = this->getOption(arg);
		
		int argPos = this->getArgPos(option.param);
		int shortArgPos = this->getArgPos(option.sparam);

		if (argPos >= 0) return argPos;
		if (shortArgPos >= 0) return shortArgPos;

		return -1;
	}

	std::string Argx::paramToID(const std::string &param)
	{
		std::string id;

		for (const auto &option : this->options)
		{
			if (option.param == param || option.sparam == param)
				return option.id;
		}

		return id;
	}

	int Argx::getArgPos(const std::string &arg)
	{
		if (!this->mainArgs)
			return -2;

		for (size_t i = 0; i < this->mainArgs->size(); ++i)
		{
			if (this->mainArgs->at(i) == arg)
				return i;
		}

		return -1;
	}

	void Argx::add(ARGXOptions option) const
	{ this->options.emplace_back(option); }

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
		if (this->findParam(id) >= 0) return true;

		return false;
	}

	bool Argx::subParamExists(const std::string &id)
	{
		for (const auto &p : this->options)
		{
			if (p.sparam == id) return true;
		}
		
		return false;
	}

	bool Argx::hasTag(const std::string &id, const std::string &tag)
	{
		int paramID = this->findParam(id);

		if (paramID < 0) return false;
		
		// Validate if tag from options is equal to this tag from function param
		if (this->options[paramID].tag == tag) return true;

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
						for (size_t i = parentPos + 1 ; i < this->mainArgs->size(); ++i)
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

	// int Argx::getWrongArgs(const std::vector<std::string> &argv)
	// {
	// 	int pos = 1;
	// 	bool isNormalParam = true;
	//
	// 	for (unsigned int i = 1 ; i < argv.size() ; ++i)
	// 	{
	// 		const auto arg = argv[pos];
	//
	// 		pos = i;
	//
	// 		if (this->options.size() > i)
	// 		{
	// 			if (this->options[i].param == arg || this->options[i].sparam == arg)
	// 			{
	// 				if (this->options[i].hasSubParams || this->options[i].hasAnySubParams)
	// 				{
	// 					isNormalParam = false;
	//
	// 					break;
	// 				}
	// 			}
	//
	// 			else break;
	// 		}
	// 	}
	//
	// 	// if (this->mainArgs->size() > 2)
	// 	// 	if (isNormalParam) return 1;
	//
	// 	return (isNormalParam ? pos : -pos);
	// }

	// Replace your existing getWrongArgs(...) with this (signature unchanged)
	int Argx::getWrongArgs(const std::vector<std::string> &argv)
	{
    	int pos = 1; // skip program name
    	bool isNormalParam = true;

    	while (pos < (int)argv.size())
    	{
        	const std::string &arg = argv[pos];

        	// Find matching top-level option
        	const ARGXOptions *matched = nullptr;

        	for (const auto &opt : this->options)
        	{
            	if (arg == opt.param || arg == opt.sparam)
            	{
                	matched = &opt;
                	break;
            	}
        	}
            
            // unknown top-level arg
        	if (!matched) return (isNormalParam ? pos : -pos);

        	// matched a top-level option
        	isNormalParam = true;

        	if (matched->hasSubParams || matched->hasAnySubParams)
        	{
            	isNormalParam = false;

            	int scanPos = pos + 1;

            	while (scanPos < (int)argv.size())
            	{
                	const std::string &nextArg = argv[scanPos];

                	// Is it a declared subparam for this option?
                	bool isSub = false;

                	for (const auto &subOpt : matched->subParams)
                	{
                    	if (nextArg == subOpt.param || nextArg == subOpt.sparam)
                    	{
                        	isSub = true;
                        	break;
                    	}
                	}

                	if (isSub)
                	{
                    	// Consume that subparam and continue scanning
                    	pos = scanPos;
                    	++scanPos;

                    	continue;
                	}

                	// If not a subparam, is it a global option? (do **NOT** consume it)
                	bool isGlobalOpt = false;

                	for (const auto &globalOpt : this->options)
                	{
                    	if (nextArg == globalOpt.param || nextArg == globalOpt.sparam)
                    	{
                        	isGlobalOpt = true;

                        	break;
                    	}
                	}

                    // valid global option follows; stop subparam scan and let outer loop handle it
                	if (isGlobalOpt) break;

                	// neither subparam nor global option; it's an invalid sub-parameter token
                	return -scanPos;
            	}
        	}

        	++pos;
    	}

    	// If nothing wrong found; return your existing success codes
    	if (isNormalParam) return (this->mainArgs->size() > 2 ? 2 : 1);
    	else return (this->mainArgs->size() > 2 ? -2 : -1);
	}

	int Argx::formatWrongArgs(const int &_int)
	{
		if (_int < 0) return -_int; // Convert to unsigned SAFELY

		return _int;
	}

	bool Argx::compareArgs(std::vector<ARGXOptions> options, std::vector<std::string> argv)
	{
    	// iterate over argv and skip program name
    	for (size_t i = 1; i < argv.size(); ++i)
    	{
        	const std::string &arg = argv[i];

        	// find a matching top-level option
        	const ARGXOptions *matched = nullptr;

        	for (const auto &opt : options)
        	{
            	if (opt.sparam == arg || opt.param == arg)
            	{
                	matched = &opt;

                	break;
            	}
        	}

			// No matched top-level option
        	if (!matched) return false;

        	// if option supports subparams, try to get them
        	if (matched->hasSubParams || matched->hasAnySubParams)
        	{
            	size_t j = i + 1;

            	while (j < argv.size())
            	{
                	const std::string &next = argv[j];

                	// Check the next declared subparam of `matched`
                	bool isSub = false;

                	for (const auto &sub : matched->subParams)
                	{
                    	if (next == sub.param || next == sub.sparam)
                    	{
                        	isSub = true;

                        	break;
                    	}
                	}

                    // Get it and continue scanning for more subparams
                	if (isSub)
                	{
                    	++j;

                    	continue;
                	}

                	// not a subparam then check if it's a known top-level option
                	bool isGlobal = false;

                	for (const auto &g : options)
                	{
                    	if (next == g.param || next == g.sparam)
                    	{
                        	isGlobal = true;
                        	break;
                    	}
                	}


                    // stop scanning subparams; outer loop will handle this global option
                	if (isGlobal) break;

                	// neither a subparam nor a global option to an invalid sequence
                	return false;
            	}

            	// advance outer index to the last consumed token ( j - 1 ).
            	// outer for-loop will increment i, so set `i = j - 1` to continue at j
            	if (j > i + 1)
                	i = j - 1;
        	}
    	}

    	return true;
	}

	ARGXOptions Argx::getOption(const std::string &id)
	{
		for (const auto &x : this->options)
			if (x.id == id) return x;

		return {};
	}

	std::vector<std::string> Argx::getSubValue(const std::string &id)
	{
		// Use `Argx::getArgPos()` function for sub-params
		size_t idPos = this->getArgPos(id) + 1;

		if (idPos < 0 || idPos == std::string::npos)
			return {this->getOption(id).defaultValue};

		std::vector<std::string> values;

		for (size_t i = idPos ; i < this->getMainArgs().size() ; i++)
		{
			// End of the sub-parameter finding
			// Assume the search is done due to an existsing sub-param
			if (i != idPos && this->subParamExists(this->getMainArgs()[i]))
				break;

			values.emplace_back(this->getMainArgs()[i]);
		}

		std::string defaultValue;

		{
			bool breakOut = false;

			for (size_t i = 0; i < this->mainArgs->size(); ++i)
			{
				for (size_t j = 0 ; j < this->options.size() ; ++j)
				{
					if (this->options[i].subParams.size() > j)
					{
						if (this->options[i].subParams[j].id == id)
						{
							defaultValue = this->options[i].subParams[j].defaultValue;


							breakOut = true;

							break;
						}
					}
				}

				if (breakOut) break;
			}
		}

		if (values.empty())
			values.emplace_back(defaultValue);

		return values;
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

