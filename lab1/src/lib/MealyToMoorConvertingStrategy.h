#ifndef MEALYTOMOORCONVERTINGSTRATEGY_H
#define MEALYTOMOORCONVERTINGSTRATEGY_H

#include "ConvertingStrategy.h"
#include "MealyStateMachineBuilder.h"
#include <iostream>
#include <regex>

class MealyToMoorConvertingStrategy : public ConvertingStrategy
{
public:
	[[nodiscard]]
	vector2d Convert(const vector2d& from) const final
	{
		auto mealy = ParseMealyStateMachine(from);

		auto testTrans = mealy.GetTransition("a1", "z1");

		std::cout << "Test getting a1/z1 trans: " << testTrans.to << " " << testTrans.func << std::endl;

		return vector2d{};
	}

private:
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
					throw std::invalid_argument("Invalid cellValue value " + cellValue);
				}
				auto transitionToState = matches[1].str();
				auto transitionFunction = matches[2].str();

				builder.AddTransition(cellState, cellTerminal, transitionToState, transitionFunction);

				std::cout << "Transition saved " << cellValue << ": "
						  << "from: " << cellState << " "
						  << "cellTerminal: " << cellTerminal << " "
						  << "to: " << transitionToState << " "
						  << "func: " << transitionFunction << std::endl;
			}
		}

		return builder.Build();
	}
};

#endif // MEALYTOMOORCONVERTINGSTRATEGY_H
