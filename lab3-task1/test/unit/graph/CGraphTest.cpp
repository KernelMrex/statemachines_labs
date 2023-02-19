#include "../../../src/lib/graph/CGraph.h"
#include <gtest/gtest.h>

TEST(CGraphTest, MustStoreAndProvideNodesWithSignals)
{
	CGraph<std::string, std::string> graph;

	graph.AddTransition("A", "B", "SignalAB");
	graph.AddTransition("A", "C", "SignalAC");
	graph.AddTransition("B", "C", "SignalBC");
	graph.AddTransition("C", "A", "SignalCA");

	{
		auto [transitions, ok] = graph.GetTransitionsFromNode("A");
		ASSERT_TRUE(ok);
		std::map<std::string, std::string> expected{ { "B", "SignalAB" }, { "C", "SignalAC" } };
		ASSERT_EQ(transitions, expected);
	}

	{
		auto [transitions, ok] = graph.GetTransitionsFromNode("B");
		ASSERT_TRUE(ok);
		std::map<std::string, std::string> expected{ { "C", "SignalBC" } };
		ASSERT_EQ(transitions, expected);
	}

	{
		auto [transitions, ok] = graph.GetTransitionsFromNode("C");
		ASSERT_TRUE(ok);
		std::map<std::string, std::string> expected{ { "A", "SignalCA" } };
		ASSERT_EQ(transitions, expected);
	}

	{
		auto [transitions, ok] = graph.GetTransitionsFromNode("D");
		ASSERT_FALSE(ok);
		std::map<std::string, std::string> expected{};
		ASSERT_EQ(transitions, expected);
	}

	{
		std::set<std::string> expected{ "A", "B", "C" };
		ASSERT_EQ(graph.GetNodes(), expected);
	}

	{
		std::set<std::string> expected{ "SignalAB", "SignalAC", "SignalBC", "SignalCA" };
		ASSERT_EQ(graph.GetSignals(), expected);
	}
}