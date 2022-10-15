#ifndef STATEMACHINECONVERTER_H
#define STATEMACHINECONVERTER_H

#include "ConvertingStrategy.h"
#include <string>
#include <vector>

using vector2d = std::vector<std::vector<std::string>>;

class StateMachineConverter
{
public:
	StateMachineConverter() = delete;

	static vector2d Convert(const ConvertingStrategy& convertingStrategy, const vector2d& convertable)
	{
		return convertingStrategy.Convert(convertable);
	}
};

#endif // STATEMACHINECONVERTER_H
