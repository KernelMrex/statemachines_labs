#ifndef MOORTOMEALYCONVERTINGSTRATEGY_H
#define MOORTOMEALYCONVERTINGSTRATEGY_H

#include "../statemachine/MoorStateMachine.h"
#include "../statemachine/MoorStateMachineBuilder.h"
#include "ConvertingStrategy.h"

class MoorToMealyConvertingStrategy : public ConvertingStrategy
{
	struct PairHash
	{
		template <class T1, class T2>
		std::size_t operator()(const std::pair<T1, T2>& p) const
		{
			auto h1 = std::hash<T1>{}(p.first);
			auto h2 = std::hash<T2>{}(p.second);

			return h1 ^ h2;
		}
	};

public:
	[[nodiscard]] vector2d Convert(const vector2d& from) const final
	{
		auto moor = ParseMoorStateMachine(from);

		MealyStateMachineBuilder mealyBuilder;
		for (const auto& moorState : moor.GetStates())
		{
			for (const auto& terminal : moor.GetTerminals())
			{
				auto trans = moor.GetTransition(moorState.first, terminal);
				mealyBuilder.AddTransition(moorState.first, terminal, trans.first, trans.second);
			}
		}

		auto mealy = mealyBuilder.Build();

		return FormatToMealyStateMachine(mealy);
	}

private:
	static MoorStateMachine ParseMoorStateMachine(const vector2d& from)
	{
		MoorStateMachineBuilder builder;

		std::unordered_map<std::string, std::string> mealyToMoorStateAliasMap;
		int toStateAliasCounter = 0;

		std::unordered_map<std::string, std::string> states;
		for (int column = 1; column < std::min(from[0].size(), from[1].size()); column++)
		{
			states.emplace(
				GetOrCreateMooreToMealyAlias(mealyToMoorStateAliasMap, toStateAliasCounter, from[1][column]),
				from[0][column]);
		}

		for (int row = 2; row < from.size(); row++)
		{
			auto terminal = from[row][0];

			for (int column = 1; column < from[row].size(); column++)
			{
				auto fromState = from[1][column];
				auto toState = from[row][column];

				auto fromStateAlias = GetOrCreateMooreToMealyAlias(mealyToMoorStateAliasMap, toStateAliasCounter, fromState);
				auto toStateAlias = GetOrCreateMooreToMealyAlias(mealyToMoorStateAliasMap, toStateAliasCounter, toState);

				auto toFuncIt = states.find(toStateAlias);
				if (toFuncIt == states.end())
				{
					throw std::invalid_argument("could not find state func for " + toStateAlias + " state");
				}

				builder.AddTransition(fromStateAlias, terminal, toStateAlias, toFuncIt->second);
			}
		}

		return builder.Build();
	}

	static vector2d FormatToMealyStateMachine(const MealyStateMachine& sm)
	{
		vector2d result;

		auto states = sm.GetStates();
		std::vector<std::string> headerStateRow;
		headerStateRow.emplace_back("");
		for (const auto& state : states)
		{
			headerStateRow.emplace_back(state);
		}
		result.emplace_back(headerStateRow);

		for (const auto& terminal : sm.GetTerminals())
		{
			std::vector<std::string> resultRow;
			resultRow.emplace_back(terminal);

			for (const auto& state : states)
			{
				auto toState = sm.GetTransition(state, terminal);
				resultRow.emplace_back(toState.to + "/" + toState.func);
			}
			result.emplace_back(resultRow);
		}

		return result;
	}

	static std::string GetOrCreateMooreToMealyAlias(
		std::unordered_map<std::string, std::string>& mealyToMoorStateAliasMap,
		int& toStateAliasCounter,
		const std::string& toState)
	{
		std::string toStateAlias;
		auto toStateAliasIt = mealyToMoorStateAliasMap.find(toState);
		if (toStateAliasIt == mealyToMoorStateAliasMap.end())
		{
			toStateAlias = "a" + std::to_string(toStateAliasCounter++);
			mealyToMoorStateAliasMap.emplace(toState, toStateAlias);
			return toStateAlias;
		}

		return toStateAliasIt->second;
	}
};

#endif // MOORTOMEALYCONVERTINGSTRATEGY_H
