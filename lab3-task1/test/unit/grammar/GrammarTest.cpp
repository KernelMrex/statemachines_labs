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
		{ "S", { "A" } },
		{ "A", { "a", "Bb" } },
		{ "B", { "b", "Bb" } },
	};
	ASSERT_EQ(rules, expected);
}

TEST(BuildLeftGrammarGraphTest, MustBuildCorrectGraph)
{
	std::map<std::string, std::set<std::string>> rules{
		{ "S", { "A" } },
		{ "A", { "a", "Bb" } },
		{ "B", { "b", "Bb" } },
	};

	auto graph = BuildLeftGrammarGraph(rules);

	std::set<std::string> expectedNodes({
		"Start",
		"A",
		"B",
		"S",
	});
	ASSERT_EQ(graph.GetNodes(), expectedNodes);

	{
		std::map<std::string, std::string> expectedTransitions;
		expectedTransitions.insert({ { "A", "a" }, { "B", "b" } });
		std::map<std::string, std::string> transitionsFromStart;
		bool ok;
		std::tie(transitionsFromStart, ok) = graph.GetTransitionsFromNode("Start");
		ASSERT_TRUE(ok);
		ASSERT_EQ(transitionsFromStart, expectedTransitions);
	}

	{
		std::map<std::string, std::string> expectedTransitions;
		expectedTransitions.insert({ { "S", "" } });
		std::map<std::string, std::string> transitionsFromA;
		std::tie(transitionsFromA, std::ignore) = graph.GetTransitionsFromNode("A");
		ASSERT_EQ(transitionsFromA, expectedTransitions);
	}

	{
		std::map<std::string, std::string> expectedTransitions;
		expectedTransitions.insert({ { "A", "b" }, { "B", "b" } });
		std::map<std::string, std::string> transitionsFromB;
		std::tie(transitionsFromB, std::ignore) = graph.GetTransitionsFromNode("B");
		ASSERT_EQ(transitionsFromB, expectedTransitions);
	}

	{
		std::map<std::string, std::string> expectedTransitions;
		std::map<std::string, std::string> transitionsFromS;
		bool ok;
		std::tie(transitionsFromS, ok) = graph.GetTransitionsFromNode("S");
		ASSERT_FALSE(ok);
		ASSERT_EQ(transitionsFromS, expectedTransitions);
	}
}