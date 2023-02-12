#ifndef CSVREADER_H
#define CSVREADER_H

#include "Types.h"
#include <sstream>
#include <string>
#include <vector>

class CSVReader
{
public:
	explicit CSVReader(std::istream& in, char delimiter = ';')
		: m_in(in)
		, m_delimiter(delimiter)
	{
	}

	[[nodiscard]]
	Vector2D ReadAll()
	{
		Vector2D result;

		for (std::string line; std::getline(m_in, line);)
		{
			std::stringstream lineStream(line);
			std::vector<std::string> row;
			for (std::string cell; std::getline(lineStream, cell, m_delimiter);)
			{
				row.push_back(cell);
			}
			result.push_back(row);
		}

		return std::move(result);
	}

private:
	std::istream& m_in;
	char m_delimiter;
};

#endif // CSVREADER_H
