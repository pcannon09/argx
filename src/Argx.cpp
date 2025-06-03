/* src/argx.cpp
 * Owned and created by: pcannon09
 */

#include <string>
#include <iostream>
#include <codecvt>
#include <locale>

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


	
	std::string Argx::createDoc(ARGXStyle style, const std::string &title, const std::string &mainInfo)
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

						if (i < x.subParams.size())
							contentStr += " | ";
					}

					contentStr += "] ] ";
				}

				else contentStr += " ] ] ";

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

		return title + "\n" + mainInfo + "\n" + contentStr;
	}

	std::vector<std::string> Argx::getMainArgs() const
	{ return *this->mainArgs; }

	std::vector<ARGXOptions> Argx::getOptions() const
	{ return this->options; }

	std::string Argx::getID() const
	{ return this->id; }
}

