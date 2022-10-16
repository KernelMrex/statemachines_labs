#ifndef MOORSTATEMACHINEBUILDER_H
#define MOORSTATEMACHINEBUILDER_H

#include "MoorStateMachine.h"
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

class MoorStateMachineBuilder
{
	struct Transition
	{
		Transition(std::string from, std::string terminal, std::string to)
			: from(std::move(from))
			, terminal(std::move(terminal))
			, to(std::move(to)){};

		std::string from;
		std::string to;
		std::string terminal;
	};

public:
	MoorStateMachineBuilder()
		: m_states(std::unordered_map<std::string, std::string>())
		, m_terminals(std::unordered_set<std::string>())
		, m_transitions(std::vector<Transition>())
	{
	}

	void AddTransition(const std::string& fromState, const std::string& terminal, const std::string& toState, const std::string& toStateFunc)
	{
		m_states.emplace(toState, toStateFunc);
		m_terminals.emplace(terminal);
		m_transitions.emplace_back(fromState, terminal, toState);
	}

	MoorStateMachine Build() const
	{
		MoorStateMachine sm(m_states, m_terminals);

		for (const auto& transition : m_transitions)
		{
			sm.SetTransition(transition.from, transition.to, transition.terminal);
		}

		return sm;
	}

private:
	std::unordered_map<std::string, std::string> m_states;
	std::unordered_set<std::string> m_terminals;
	std::vector<Transition> m_transitions;
};

#endif // MOORSTATEMACHINEBUILDER_H
