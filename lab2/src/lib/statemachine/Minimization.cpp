#include "Minimization.h"
#include "../utils/vector.h"
#include <regex>
#include <set>

using TransitionDest = std::string;
using TransitionFunc = std::string;

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

static Vector2D ReduceByColumnHeader(Vector2D table, std::size_t headerIndex = 0)
{
	Vector2D result;
	transpose<std::string>(table);
	std::set<std::string> processed;
	result.push_back(std::move(table[0]));
	for (std::size_t i = 1; i < table.size(); i++)
	{
		if (!processed.contains(table[i][headerIndex]))
		{
			processed.insert(table[i][headerIndex]);
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

static std::vector<std::vector<std::size_t>> GroupColumnsByUniqueness(const std::vector<std::vector<std::string>>& columns)
{
	std::vector<std::vector<std::size_t>> groups;

	for (std::size_t columnIndex = 0; columnIndex < columns.size(); columnIndex++)
	{
		bool isFound = false;
		for (std::size_t groupsIndex = 0; groupsIndex < groups.size(); groupsIndex++)
		{
			if (columns[groups[groupsIndex][0]] == columns[columnIndex])
			{
				isFound = true;
				groups[groupsIndex].push_back(columnIndex);
			}
		}
		if (!isFound)
		{
			groups.push_back(std::vector{ columnIndex });
		}
	}

	return std::move(groups);
}

static std::map<std::string, std::string> ReverseMap(const std::map<std::string, std::vector<std::string>>& map)
{
	std::map<std::string, std::string> result;

	for (const auto& [key, value] : map)
	{
		for (const auto& item : value)
		{
			result.insert(std::pair(item, key));
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


static std::map<std::string, std::vector<std::string>> SplitByColumnInGroupUniqueness(
	const Vector2D& protoTable,
	const std::map<std::string, std::vector<std::string>>& groupNameToState)
{
	auto stateToGroupName = ReverseMap(groupNameToState);

	Vector2D table = protoTable;
	for (std::size_t i = 2; i < protoTable.size(); i++)
	{
		for (std::size_t j = 1; j < protoTable[0].size(); j++)
		{
			auto cell = protoTable[i][j];
			table[i][j] = stateToGroupName[cell];
		}
	}

	std::map<std::string, std::size_t> stateNameToStateIndex;
	for (std::size_t i = 1; i < table[0].size(); i++)
	{
		stateNameToStateIndex.insert(std::pair(table[1][i], i));
	}

	int groupIndex = 0;
	std::map<std::string, std::vector<std::string>> resultingGroupNameToState;
	for (const auto& [_, stateNames] : groupNameToState)
	{
		std::vector<std::vector<std::string>> columns;
		std::map<std::size_t, std::string> currentIndexToStateName;
		std::size_t currentIndex = 0;
		for (const auto& stateName : stateNames)
		{
			currentIndexToStateName.insert(std::pair(currentIndex++, stateName));
			columns.push_back(GetColumn(table, stateNameToStateIndex[stateName], 2, table.size()));
		}

		auto groups = GroupColumnsByUniqueness(columns);

		for (const auto& group : groups)
		{
			std::string newGroupName = "G" + std::to_string(groupIndex++);
			std::vector<std::string> newGroupContent;
			for (auto stateColumnIndex : group)
			{
				newGroupContent.push_back(currentIndexToStateName[stateColumnIndex]);
			}
			resultingGroupNameToState.insert(std::pair(std::move(newGroupName), std::move(newGroupContent)));
		}
	}

	return resultingGroupNameToState;
}

Vector2D MinimizeMooreStateMachine(const Vector2D& mooreTable)
{
	std::map<std::string, std::vector<std::string>> groups;
	for (std::size_t i = 1; i < mooreTable[0].size(); i++)
	{
		auto stateName = mooreTable[1][i];
		auto signalName = mooreTable[0][i];

		auto it = groups.find(signalName);
		if (it == groups.end())
		{
			groups.insert(std::pair(signalName, std::vector{ stateName }));
			continue;
		}
		it->second.push_back(stateName);
	}

	std::size_t prevGroupsSize;
	do
	{
		prevGroupsSize = groups.size();
		groups = SplitByColumnInGroupUniqueness(mooreTable, groups);
	} while(prevGroupsSize != groups.size());

	Vector2D minimizedTable = mooreTable;
	auto stateToGroupName = ReverseMap(groups);
	for (std::size_t i = 1; i < minimizedTable.size(); i++)
	{
		for (std::size_t j = 1; j < minimizedTable[0].size(); j++)
		{
			minimizedTable[i][j] = stateToGroupName[minimizedTable[i][j]];
		}
	}

	return std::move(ReduceByColumnHeader(minimizedTable, 1));
}
