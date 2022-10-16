#ifndef CSVWRITER_H
#define CSVWRITER_H

#include <iostream>
#include <string>
#include <vector>
#include <sstream>

class CsvWriter
{
	using vector2d = std::vector<std::vector<std::string>>;

public:
	explicit CsvWriter(std::ostream& out, char delimiter = ';')
		: m_out(out)
		, m_delimiter(delimiter){};

	void WriteAll(const vector2d& data)
	{
		for (const auto& row : data)
		{
			std::stringstream oss;
			for (const auto& cell : row)
			{
				oss << cell << m_delimiter;
			}
			auto resultRow = oss.str();
			m_out << resultRow.substr(0, resultRow.length() - 1) << std::endl;
			m_out.flush();
		}
	}

private:
	std::ostream& m_out;
	char m_delimiter;
};

#endif // CSVWRITER_H
