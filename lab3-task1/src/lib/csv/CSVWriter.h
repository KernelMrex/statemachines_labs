#ifndef CSVWRITER_H
#define CSVWRITER_H

#include "Types.h"
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

class CSVWriter
{
public:
	explicit CSVWriter(std::ostream& out, char delimiter = ';')
		: m_out(out)
		, m_delimiter(delimiter){};

	void WriteAll(const Vector2D& data)
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
