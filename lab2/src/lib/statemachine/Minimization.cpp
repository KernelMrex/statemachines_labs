#include "Minimization.h"
#include "../utils/vector.h"
#include <regex>
#include <set>

using TransitionDest = std::string;
using TransitionFunc = std::string;
using StateName = std::string;
using GroupName = std::string;

static std::tuple<TransitionDest, TransitionFunc> ParseMealyTransition(const std::string& transition)
{
	std::regex transitionRegex("([^/]+)/([^/]+)");
	std::smatch matches;
	if (!std::regex_search(transition, matches, transitionRegex))
	{
		throw std::invalid_argument("Invalid transition \"" + transition + "\"");
	}
	return { std::move(matches[1].str()), std::move(matches[2].str()) };
}

static std::vector<std::vector<std::size_t>> FindEqualityGroups(Vector2D table)
{
	std::vector<std::vector<std::size_t>> groups;
	std::set<std::size_t> visited;
	transpose<std::string>(table);

	for (std::size_t i = 0; i < table.size(); i++)
	{
		if (visited.contains(i))
		{
			continue;
		}
		std::vector<std::size_t> group;
		group.push_back(i);
		for (std::size_t j = i + 1; j < table.size(); j++)
		{
			if (visited.contains(j))
			{
				continue;
			}
			if (table[i] == table[j])
			{
				group.push_back(j);
				visited.insert(j);
			}
		}
		groups.push_back(group);
	}

	return groups;
}

static std::tuple<Vector2D, std::size_t> MakeEqualityGroup(const Vector2D& table)
{
	Vector2D transFuncTable;
	Vector2D transDestTable;
	for (std::size_t i = 1; i < table.size(); i++)
	{
		std::vector<TransitionFunc> funcRow;
		std::vector<TransitionDest> destRow;
		for (std::size_t j = 1; j < table[i].size(); j++)
		{
			TransitionFunc func;
			TransitionDest dest;
			std::tie(dest, func) = ParseMealyTransition(table[i][j]);
			funcRow.push_back(func);
			destRow.push_back(dest);
		}
		transFuncTable.push_back(funcRow);
		transDestTable.push_back(destRow);
	}

	auto equalityGroups = FindEqualityGroups(transFuncTable);

	std::map<std::size_t, std::string> columnToNameMap;
	for (std::size_t i = 1; i < table[0].size(); i++)
	{
		columnToNameMap.insert(std::pair(i - 1, table[0][i]));
	}
	std::map<std::size_t, std::string> columnToEqualityGroupName;
	for (std::size_t i = 0; i < equalityGroups.size(); i++)
	{
		std::string groupName = "A" + std::to_string(i);
		for (auto column : equalityGroups[i])
		{
			columnToEqualityGroupName.insert(std::pair(column, groupName));
		}
	}
	std::map<std::string, std::string> oldToNewName;
	for (std::size_t i = 0; i < table[0].size() - 1; i++)
	{
		oldToNewName.insert(std::pair(columnToNameMap[i], columnToEqualityGroupName[i]));
	}

	Vector2D resultingTable = table;
	for (std::size_t i = 1; i < table[0].size(); i++)
	{
		resultingTable[0][i] = columnToEqualityGroupName[i - 1];
	}
	for (std::size_t i = 1; i < table.size(); i++)
	{
		for (std::size_t j = 1; j < table[i].size(); j++)
		{
			auto state = oldToNewName[transDestTable[i - 1][j - 1]];
			resultingTable[i][j] = state + "/" + state;
		}
	}

	return { std::move(resultingTable), equalityGroups.size() };
}

static Vector2D ReduceByColumnHeader(Vector2D table)
{
	Vector2D result;
	transpose<std::string>(table);
	std::set<std::string> processed;
	result.push_back(std::move(table[0]));
	for (std::size_t i = 1; i < table.size(); i++)
	{
		if (!processed.contains(table[i][0]))
		{
			processed.insert(table[i][0]);
			result.push_back(std::move(table[i]));
		}
	}

	transpose<std::string>(result);
	return std::move(result);
}

Vector2D MinimizeMealyStateMachine(const Vector2D& mealyTable)
{
	Vector2D processingTabel = mealyTable;
	std::size_t prevAmountOfEqualityGroups = 0;
	std::size_t amountOfEqualityGroups = -1;

	while (prevAmountOfEqualityGroups != amountOfEqualityGroups)
	{
		prevAmountOfEqualityGroups = amountOfEqualityGroups;
		std::tie(processingTabel, amountOfEqualityGroups) = MakeEqualityGroup(processingTabel);
	}

	Vector2D minimizedTable = processingTabel;
	for (std::size_t i = 1; i < processingTabel[0].size(); i++)
	{
		minimizedTable[0][i] = processingTabel[0][i];
	}
	for (std::size_t i = 1; i < mealyTable.size(); i++)
	{
		for (std::size_t j = 1; j < mealyTable[i].size(); j++)
		{
			TransitionFunc func;
			std::tie(std::ignore, func) = ParseMealyTransition(mealyTable[i][j]);

			TransitionDest dest;
			std::tie(dest, std::ignore) = ParseMealyTransition(processingTabel[i][j]);

			processingTabel[i][j] = dest + "/" + func;
		}
	}

	return std::move(ReduceByColumnHeader(processingTabel));
}

static Vector2D MooreInitialSplitting(const Vector2D& table)
{
	std::map<StateName, TransitionFunc> stateToSignal;
	for (std::size_t i = 1; i < table[0].size(); i++)
	{
		stateToSignal.insert(std::pair(table[1][i], table[0][i]));
	}

	Vector2D result = table;
	for (std::size_t i = 2; i < result.size(); i++)
	{
		for (std::size_t j = 1; j < result[0].size(); j++)
		{
			result[i][j] = stateToSignal[table[i][j]];
		}
	}

	return std::move(result);
}

static std::vector<std::string> GetColumn(const Vector2D& table, std::size_t columnI, std::size_t rowStart, std::size_t rowEnd)
{
	std::vector<std::string> result;
	for (std::size_t i = rowStart; i < rowEnd; i++)
	{
		result.push_back(table[i][columnI]);
	}
	return std::move(result);
}

static std::tuple<Vector2D, std::size_t> SplitIntoGroups(const Vector2D& table)
{
	// Получаем группы
	std::map<std::string, std::set<std::string>> groupNameToStateName;
	std::map<std::string, std::size_t> stateToIndex;

	for (std::size_t i = 1; i < table[0].size(); i++)
	{
		std::string groupName = table[0][i];
		std::string stateName = table[1][i];

		stateToIndex.insert(std::pair(stateName, i));

		auto it = groupNameToStateName.find(groupName);
		if (it == groupNameToStateName.end())
		{
			groupNameToStateName.insert(std::pair(groupName, std::set<std::string>{ stateName }));
			continue;
		}
		it->second.insert(stateName);
	}

	Vector2D newTable;
	std::vector<std::string> row;
	for (const auto& i : table)
	{
		row.push_back(i[0]);
	}
	newTable.push_back(row);

	int newStateIndex = 0;
	// Сравниваем между собой группируя похожие классы эквивалентности
	for (auto [groupName, states] : groupNameToStateName)
	{
		// Извлекаем столбцы
		std::map<StateName, std::vector<std::string>> stringStatesContent;

		for (const std::string& state : states)
		{
			auto columnContent = GetColumn(table, stateToIndex[state], 2, table.size());
			stringStatesContent.insert(std::pair(state, columnContent));
		}

		// Сравниваем между собой группируя похожие классы эквивалентности

		// Предполгаем что они всё разные. Если при проверке выясниться, что они похожие задаём новый индекс обоим столбцам
		std::vector<std::tuple<GroupName, StateName, std::vector<std::string>>> newSubGroups;
		for (auto [stateName, stateContent] : stringStatesContent)
		{
			std::string foundSubGroupName;
			for (auto [subgroupName, _, subgroupContent] : newSubGroups)
			{
				if (stateContent == subgroupContent)
				{
					foundSubGroupName = subgroupName;
					break;
				}
			}

			if (foundSubGroupName.empty())
			{
				foundSubGroupName = "S" + std::to_string(newStateIndex++);
			}

			newSubGroups.emplace_back(foundSubGroupName, stateName, stateContent);
		}

		for (auto [newGroupName, stateName, stateContent] : newSubGroups)
		{
			std::vector<std::string> newStateRow;
			newStateRow.push_back(newGroupName);
			newStateRow.push_back(stateName);
			for (const auto& cell : stateContent)
			{
				newStateRow.push_back(cell);
			}
			newTable.push_back(newStateRow);
		}
	}

	transpose(newTable);

	return { newTable, newStateIndex };
}

Vector2D MinimizeMooreStateMachine(const Vector2D& mooreTable)
{
	Vector2D table = MooreInitialSplitting(mooreTable);

	std::size_t prevGroupNumber = -1;
	std::size_t groupNumber = 0;

	while (groupNumber != prevGroupNumber)
	{
		prevGroupNumber = groupNumber;
		std::tie(table, groupNumber) = SplitIntoGroups(table);
	}

	return table;
}
