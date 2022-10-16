#ifndef MOORSTATEMACHINE_H
#define MOORSTATEMACHINE_H

#include <set>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>

class MoorStateMachine
{
	struct TransitionsMapKey
	{
		TransitionsMapKey(std::string from, std::string terminal)
			: from(std::move(from))
			, terminal(std::move(terminal))
		{
		}

		struct Hash
		{
			size_t operator()(const TransitionsMapKey& key) const
			{
				return std::hash<std::string>()(key.from) ^ std::hash<std::string>()(key.terminal);
			}

			bool operator()(const TransitionsMapKey& key1, const TransitionsMapKey& key2) const
			{
				return key1.from == key2.from && key1.terminal == key2.terminal;
			}
		};

		std::string from;
		std::string terminal;
	};

	using TransitionsMap = std::unordered_map<TransitionsMapKey, std::string, TransitionsMapKey::Hash, TransitionsMapKey::Hash>;

public:
	MoorStateMachine(std::unordered_map<std::string, std::string> states, std::unordered_set<std::string> terminals)
		: m_transitions(TransitionsMap())
		, m_states(std::move(states))
		, m_terminals(std::move(terminals))
	{
	}

	void SetTransition(const std::string& fromStateAlias, const std::string& toStateAlias, const std::string& terminal)
	{
		auto stateIt = m_states.find(fromStateAlias);
		if (stateIt == m_states.end())
		{
			throw std::invalid_argument("Cannot find state " + fromStateAlias);
		}

		m_transitions.emplace(TransitionsMapKey(fromStateAlias, terminal), toStateAlias);
	}

	std::pair<std::string, std::string> GetTransition(const std::string& fromStateAlias, const std::string& terminal) const
	{
		auto transIt = m_transitions.find(TransitionsMapKey(fromStateAlias, terminal));
		if (transIt == m_transitions.end())
		{
			throw std::invalid_argument("Cannot find transition from (" + fromStateAlias + ", " + terminal + ")");
		}

		auto stateFuncIt = m_states.find(transIt->second);
		if (stateFuncIt == m_states.end())
		{
			throw std::invalid_argument("Cannot find state func for " + transIt->second);
		}

		return std::make_pair(transIt->second, stateFuncIt->second);
	}

	const std::unordered_set<std::string>& GetTerminals() const
	{
		return m_terminals;
	}

	const std::unordered_map<std::string, std::string>& GetStates() const
	{
		return m_states;
	}

private:
	TransitionsMap m_transitions;
	std::unordered_set<std::string> m_terminals;
	std::unordered_map<std::string, std::string> m_states;
};

#endif // MOORSTATEMACHINE_H
