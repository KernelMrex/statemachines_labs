#ifndef CONVERTINGSTRATEGY_H
#define CONVERTINGSTRATEGY_H

#include <string>
#include <vector>

using vector2d = std::vector<std::vector<std::string>>;

class ConvertingStrategy
{
public:
	[[nodiscard]]
	virtual vector2d Convert(const vector2d& from) const = 0;

	virtual ~ConvertingStrategy() = default;
};

#endif // CONVERTINGSTRATEGY_H
