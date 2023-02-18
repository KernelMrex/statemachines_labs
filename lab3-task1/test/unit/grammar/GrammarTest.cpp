#include "../../../src/lib/grammar/Grammar.h"
#include <gtest/gtest.h>

TEST(ParseRulesTest, MustParseRules)
{
	std::string data = "S -> A\n"
					   "A -> a | Bb\n"
					   "B -> b | Bb\n";

	std::istringstream iss(data);

	auto rules = ParseRules(iss);
	std::map<std::string, std::set<std::string>> expected{
		{ "S", {"A"} },
		{ "A", {"a", "Bb"} },
		{ "B", {"b", "Bb"} },
	};
	ASSERT_EQ(rules, expected);
}