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

static std::tuple<std::string, std::string> ParseGrammarRightSide(const std::string& rightSide)
{
	std::string to;
	std::string signal;

	for (char ch : rightSide)
	{
		if (ch >= 'A' && ch <= 'Z')
		{
			to = ch;
		}
		else
		{
			signal = ch;
		}
	}

	return { to, signal };
}

CGraph<std::string, std::string> BuildLeftGrammarGraph(const std::map<std::string, std::set<std::string>>& rules)
{
	CGraph<std::string, std::string> graph;

	static const std::string startNodeName = "Start";

	for (const auto& rule : rules)
	{
		const auto& to = rule.first;
		for (const auto& rightSide : rule.second)
		{
			std::string from;
			std::string signal;
			std::tie(from, signal) = ParseGrammarRightSide(rightSide);
			if (from.empty())
			{
				from = startNodeName;
			}
			graph.AddTransition(from, to, signal);
		}
	}

	return graph;
}

CGraph<std::string, std::string> BuildRightGrammarGraph(const std::map<std::string, std::set<std::string>>& rules)
{
	CGraph<std::string, std::string> graph;

	static const std::string lastNodeName = "Last";

	for (const auto& rule : rules)
	{
		const auto& from = rule.first;
		for (const auto& rightSide : rule.second)
		{
			std::string to;
			std::string signal;
			std::tie(to, signal) = ParseGrammarRightSide(rightSide);
			if (to.empty())
			{
				to = lastNodeName;
			}
			graph.AddTransition(from, to, signal);
		}
	}

	return graph;
}
