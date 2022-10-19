#ifndef MEALYTOMOORCONVERTER_H
#define MEALYTOMOORCONVERTER_H

#include "../statemachine/MealyStateMachineBuilder.h"
#include "../statemachine/MoorStateMachineBuilder.h"
#include <iostream>
#include <queue>
#include <regex>

using vector2d = std::vector<std::vector<std::string>>;

class MealyToMoorConverter
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
	[[nodiscard]] static vector2d Convert(const vector2d& from)
	{
		auto mealy = ParseMealyStateMachine(from);
		auto states = mealy.GetStates();
		auto terminals = mealy.GetTerminals();

		if (states.empty() || terminals.empty())
		{
			return vector2d{};
		}

		int moorStateCounter = 0;
		std::unordered_map<std::pair<std::string, std::string>, std::string, PairHash> mealyToMoorStateAliasMap;
		std::queue<std::pair<std::string, std::string>> mealyCellsToVisit;
		mealyCellsToVisit.push(std::make_pair(*states.begin(), *terminals.begin()));
		std::unordered_set<std::pair<std::string, std::string>, PairHash> mealyVisited;

		MoorStateMachineBuilder moorBuilder;

		for (std::pair<std::string, std::string> cellToVisit; !mealyCellsToVisit.empty(); mealyCellsToVisit.pop())
		{
			cellToVisit = mealyCellsToVisit.front();
			// Запоминаем что посещали данную ячейку
			mealyVisited.emplace(cellToVisit.first, cellToVisit.second);

			// Получаем текущую ячейку
			auto mealyTransition = mealy.GetTransition(cellToVisit.first, cellToVisit.second);

			// Получаем алиас на таблицу автомата Мура(из какого столбца вышли)
			std::string mealyToMoorVisitedAlias = GetOrCreateMealyToMoorVisitedAlias(mealyToMoorStateAliasMap, mealyTransition.to, mealyTransition.func, moorStateCounter);

			// Добавляем переходы по другим ячейкам в очередь
			for (const auto& terminal : terminals)
			{
				auto nextCellToVisit = std::make_pair(mealyTransition.to, terminal);
				if (mealyVisited.find(nextCellToVisit) == mealyVisited.end())
				{
					mealyVisited.emplace(nextCellToVisit.first, nextCellToVisit.second);
					mealyCellsToVisit.emplace(nextCellToVisit.first, nextCellToVisit.second);
				}

				// Получаем ячейку для перехода
				auto mealyNewTransition = mealy.GetTransition(mealyTransition.to, terminal);

				// Получаем алиас на таблицу автомата Мура(в какой столбец попадаем)
				auto mealyToMoorNewVisitedAlias = GetOrCreateMealyToMoorVisitedAlias(mealyToMoorStateAliasMap, mealyNewTransition.to, mealyNewTransition.func, moorStateCounter);

				// Записываем переход из mealyToMoorVisitedAlias -(terminal)> mealyToMoorNewVisitedAlias
				moorBuilder.AddTransition(mealyToMoorVisitedAlias, terminal, mealyToMoorNewVisitedAlias, mealyNewTransition.func);
			}
		}

		return FormatToMoorStateMachine(moorBuilder.Build());
	}

private:
	static std::string GetOrCreateMealyToMoorVisitedAlias(
		std::unordered_map<std::pair<std::string, std::string>, std::string, PairHash>& map,
		const std::string& toState,
		const std::string& func,
		int& counter)
	{

		auto mealyNewTransitionPair = std::make_pair(toState, func);

		auto mealyToMoorNewVisitedAliasIt = map.find(mealyNewTransitionPair);
		std::string mealyToMoorNewVisitedAlias;
		if (mealyToMoorNewVisitedAliasIt == map.end())
		{
			mealyToMoorNewVisitedAlias = "S" + std::to_string(counter++);
			map.emplace(mealyNewTransitionPair, mealyToMoorNewVisitedAlias);
		}
		else
		{
			mealyToMoorNewVisitedAlias = mealyToMoorNewVisitedAliasIt->second;
		}

		return mealyToMoorNewVisitedAlias;
	}

	static MealyStateMachine ParseMealyStateMachine(const vector2d& from)
	{
		std::regex mealyCellRegex("([^/]+)/([^/]+)");

		MealyStateMachineBuilder builder;

		for (int row = 1; row < from.size(); row++)
		{
			auto cellTerminal = from[row][0];

			for (int column = 1; column < from[row].size(); column++)
			{
				auto cellState = from[0][column];

				auto cellValue = from[row][column];

				std::smatch matches;
				if (!std::regex_search(cellValue, matches, mealyCellRegex))
				{
					throw std::invalid_argument("Invalid cell value \"" + cellValue + "\"");
				}
				auto transitionToState = matches[1].str();
				auto transitionFunction = matches[2].str();

				builder.AddTransition(cellState, cellTerminal, transitionToState, transitionFunction);
			}
		}

		return builder.Build();
	}

	static vector2d FormatToMoorStateMachine(const MoorStateMachine& sm)
	{
		vector2d result;

		auto states = sm.GetStates();
		std::vector<std::string> headerStateAliasRow;
		std::vector<std::string> headerStateFuncRow;
		headerStateAliasRow.emplace_back("");
		headerStateFuncRow.emplace_back("");
		for (const auto& state : states)
		{
			headerStateAliasRow.emplace_back(state.first);
			headerStateFuncRow.emplace_back(state.second);
		}
		result.emplace_back(headerStateFuncRow);
		result.emplace_back(headerStateAliasRow);

		for (const auto& terminal : sm.GetTerminals())
		{
			std::vector<std::string> resultRow;
			resultRow.emplace_back(terminal);

			for (const auto& state : states)
			{
				auto toState = sm.GetTransition(state.first, terminal);
				resultRow.emplace_back(toState.first);
			}
			result.emplace_back(resultRow);
		}

		return result;
	}
};

#endif // MEALYTOMOORCONVERTER_H
