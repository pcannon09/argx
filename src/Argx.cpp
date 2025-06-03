/* src/argx.cpp
 * Owned and created by: pcannon09
 */

#include <string>
#include <iostream>

#include "../inc/Argx.hpp"
#include "../inc/types.hpp"

namespace argx
{
	// PRIVATE
	std::vector<ARGXOptions> Argx::options;
	std::vector<std::string> *Argx::mainArgs = nullptr;

	// PUBLIC:
	Argx::Argx(const std::string &id, int argc, char *argv[])
		: id(id)
	{
        this->mainArgs = new std::vector<std::string>(argv, argv + argc);
	}

	Argx::Argx()
	{ }

	Argx::~Argx()
	{
		delete this->mainArgs; this->mainArgs = nullptr;
	}

	void Argx::add(ARGXOptions option) const
	{
		ARGXError error = {
			.type = "success",
			.code = 0
		};

		this->options.emplace_back(option);
	}

	ARGXParam Argx::getParam(const std::string& id)
	{
		ARGXParam result;

		for (const auto& opt : this->options)
		{
			bool subMatched = false;

			// Check if the main option matches
			for (const std::string& arg : *this->mainArgs)
			{
				if (arg == opt.param || arg == opt.sparam)
				{
					result.exists = true;
					break;
				}
			}

			// Check sub-options
			for (const auto& sub : opt.subParams)
			{
				for (const std::string& arg : *this->mainArgs)
				{
					if (arg == sub.param || arg == sub.sparam)
					{
						subMatched = true;
						break;
					}
				}
				if (subMatched) break;
			}

			result.subExists.push_back(subMatched);
		}

		return result;
	}

	std::string Argx::createDoc(ARGXStyle style, std::string title, std::string mainInfo)
	{
		std::string startStr;
		std::string contentStr;
		std::string endStr;

		contentStr += mainInfo;

		if (style == ARGXStyle::Professional)
		{
			unsigned int i = 0;

			for (const auto &x : this->options)
			{
				++i;

				std::string subParamSpacing;

				contentStr += "ID: " + x.id + "\n";
				contentStr += "[ " + std::string(x.sparam) + " | " + std::string(x.param) + 
               		(x.hasSubParams ? x.subParams[i].param : std::string("")) + "] " + 
               		std::string(x.info);
			}
		}
		
		return "";
	}

	std::vector<std::string> Argx::getMainArgs() const
	{ return *this->mainArgs; }

	std::vector<ARGXOptions> Argx::getOptions() const
	{ return this->options; }

	std::string Argx::getID() const
	{ return this->id; }
}

