#include "Grammar.h"
#include <sstream>

std::map<std::string, std::set<std::string>> ParseRules(std::istream& in)
{
	std::map<std::string, std::set<std::string>> rules;
	for (std::string line; std::getline(in, line);)
	{
		std::istringstream iss(line);
		std::string source;
		std::set<std::string> destinations;
		iss >> source;
		while (!iss.eof())
		{
			std::string separator;
			std::string destination;
			iss >> separator;
			iss >> destination;
			destinations.insert(destination);
		}
		rules.emplace(source, destinations);
	}

	return rules;
}
