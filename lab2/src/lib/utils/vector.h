#ifndef LAB2_VECTOR_H
#define LAB2_VECTOR_H

#include <vector>

template <typename T>
void transpose(std::vector<std::vector<T>>& v)
{
	if (v.empty())
	{
		return;
	}

	std::vector<std::vector<T>> trans(v[0].size(), std::vector<T>());

	for (int i = 0; i < v.size(); i++)
	{
		for (std::size_t j = 0; j < v[i].size(); j++)
		{
			trans[j].push_back(v[i][j]);
		}
	}

	v = trans;
}

#endif // LAB2_VECTOR_H
