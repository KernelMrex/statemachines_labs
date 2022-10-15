#ifndef MEALYSTATEMACHINEBUILDER_H
#define MEALYSTATEMACHINEBUILDER_H

#include "MealyStateMachine.h"
#include <string>
#include <unordered_set>
#include <utility>
#include <vector>

class MealyStateMachineBuilder
{
	struct Transition
	{
		Transition(std::string from, std::string terminal, std::string to, std::string func)
			: from(std::move(from))
			, terminal(std::move(terminal))
			, to(std::move(to))
			, func(std::move(func)){};

		std::string from;
		std::string to;
		std::string func;
		std::string terminal;
	};

public:
	MealyStateMachineBuilder()
		: m_states(std::unordered_set<std::string>())
		, m_terminals(std::unordered_set<std::string>())
		, m_transitions(std::vector<Transition>())
	{
	}

	void AddTransition(const std::string& fromState, const std::string& terminal, const std::string& toState, const std::string& transFunc)
	{
		m_states.insert(fromState);
		m_states.insert(toState);
		m_terminals.insert(terminal);
		m_transitions.emplace_back(fromState, terminal, toState, transFunc);
	}

	MealyStateMachine Build() const
	{
		MealyStateMachine sm(m_states, m_terminals);

		for (const auto& transition : m_transitions)
		{
			sm.SetTransition(transition.from, transition.terminal, transition.to, transition.func);
		}

		return sm;
	}

private:
	std::unordered_set<std::string> m_states;
	std::unordered_set<std::string> m_terminals;
	std::vector<Transition> m_transitions;
};

#endif // MEALYSTATEMACHINEBUILDER_H
