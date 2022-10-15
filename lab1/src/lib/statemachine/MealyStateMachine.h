#ifndef MEALYSTATEMACHINE_H
#define MEALYSTATEMACHINE_H

#include <map>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>

class MealyStateMachine
{
	struct Transition
	{
		Transition() = default;

		Transition(std::string to, std::string func)
			: to(std::move(to))
			, func(std::move(func)){};

		std::string to;
		std::string func;
	};

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

	using TransitionMap = std::unordered_map<TransitionsMapKey, Transition, TransitionsMapKey::Hash, TransitionsMapKey::Hash>;

public:
	MealyStateMachine(const std::unordered_set<std::string>& states, const std::unordered_set<std::string>& terminals)
		: m_transitions(TransitionMap())
		, m_states(states)
		, m_terminals(terminals)
	{
		for (const auto& state : states)
		{
			for (const auto& terminal : terminals)
			{
				m_transitions.insert(std::make_pair(TransitionsMapKey(state, terminal), Transition()));
			}
		}
	}

	[[nodiscard]] Transition GetTransition(const std::string& state, const std::string& terminal) const
	{
		return m_transitions.at(TransitionsMapKey(state, terminal));
	}

	void SetTransition(const std::string& fromState, const std::string& terminal, const std::string& toState, const std::string& transFunc)
	{
		Transition newTrans(toState, transFunc);
		auto transIt = m_transitions.find(TransitionsMapKey(fromState, terminal));
		if (transIt == m_transitions.end())
		{
			m_transitions.insert(std::make_pair(TransitionsMapKey(fromState, terminal), newTrans));
		}
		transIt->second = newTrans;
	}

	[[nodiscard]] std::unordered_set<std::string> GetStates() const
	{
		return m_states;
	}

	[[nodiscard]] std::unordered_set<std::string> GetTerminals() const
	{
		return m_terminals;
	}

private:
	TransitionMap m_transitions;
	std::unordered_set<std::string> m_states;
	std::unordered_set<std::string> m_terminals;
};

#endif // MEALYSTATEMACHINE_H
