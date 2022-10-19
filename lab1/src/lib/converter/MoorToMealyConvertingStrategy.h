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

		return vector2d{};
	}

private:
	static MoorStateMachine ParseMoorStateMachine(const vector2d& from)
	{
		MoorStateMachineBuilder builder;

		std::unordered_map<std::string, std::string> states;
		for (int column = 1; column < std::min(from[0].size(), from[1].size()); column++)
		{
			states.emplace(from[1][column], from[0][column]);
		}

		for (int row = 2; row < from.size(); row++)
		{
			auto terminal = from[row][0];

			for (int column = 1; column < from[row].size(); column++)
			{
				auto fromState = from[1][column];
				auto toState = from[row][column];

				auto toFuncIt = states.find(toState);
				if (toFuncIt == states.end())
				{
					throw std::invalid_argument("could not find state func for " + toState + " state");
				}

				builder.AddTransition(fromState, terminal, toState, toFuncIt->second);
			}
		}

		return builder.Build();
	}
};

#endif // MOORTOMEALYCONVERTINGSTRATEGY_H
