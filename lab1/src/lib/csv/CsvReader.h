#ifndef CSVREADER_H
#define CSVREADER_H

#include <vector>
#include <string>
#include <sstream>

class CsvReader
{
	using vector2d = std::vector<std::vector<std::string>>;

public:
	explicit CsvReader(std::istream& in, char delimiter = ';')
		: m_in(in)
		, m_delimiter(delimiter)
	{
	}

	[[nodiscard]]
	vector2d ReadAll()
	{
		vector2d result;

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

		return result;
	}

private:
	std::istream& m_in;
	char m_delimiter;
};

#endif // CSVREADER_H
