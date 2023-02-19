#ifndef LAB3_TASK1_CGRAPH_H
#define LAB3_TASK1_CGRAPH_H

#include <map>
#include <set>
#include <string>
#include <unordered_map>

template <typename NodeT, typename SignalT>
class CGraph
{
public:
	CGraph()
		: m_nodes(std::set<NodeT>())
		, m_signals(std::set<SignalT>())
		, m_transitions(std::map<NodeT, std::map<NodeT, SignalT>>()){};

	bool AddTransition(const NodeT& from, const NodeT& to, const SignalT& signal)
	{
		m_nodes.insert(from);
		m_nodes.insert(to);
		m_signals.insert(signal);

		auto it = m_transitions.find(from);
		if (it == m_transitions.end())
		{
			m_transitions.insert(std::pair(from, std::map{std::pair(to, signal)}));
			return true;
		}

		auto& innerMap = it->second;
		auto innerMapIt = innerMap.find(to);
		if (innerMapIt == innerMap.end())
		{
			innerMap.insert(std::pair(to, signal));
			return true;
		}

		return false;
	}

	std::tuple<std::map<NodeT, SignalT>, bool> GetTransitionsFromNode(const NodeT& from) const
	{
		auto it = m_transitions.find(from);
		if (it == m_transitions.end())
		{
			return std::tuple(std::map<NodeT, SignalT>(), false);
		}

		return std::tuple(it->second, true);
	}

	std::set<NodeT> const& GetNodes() const
	{
		return m_nodes;
	}

	std::set<SignalT> const& GetSignals() const
	{
		return m_signals;
	}

private:
	std::set<NodeT> m_nodes;
	std::set<SignalT> m_signals;
	std::map<NodeT, std::map<NodeT, SignalT>> m_transitions;
};

#endif // LAB3_TASK1_CGRAPH_H
