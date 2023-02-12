#ifndef LAB2_ARGS_H
#define LAB2_ARGS_H

#include "../statemachine/StateMachineType.h"
#include <optional>
#include <string>

class Args
{
public:
	std::optional<Args> static ParseFromArgs(int argc, char* argv[])
	{
		if (argc != 4)
		{
			return std::nullopt;
		}

		auto minimizationType = ParseStateMachineType(argv[1]);
		if (!minimizationType)
		{
			return std::nullopt;
		}

		return {{
			minimizationType.value(),
			std::string(argv[2]),
			std::string(argv[3])
		}};
	}

	[[nodiscard]]
	StateMachineType GetStateMachineType() const
	{
		return m_stateMachineType;
	}

	[[nodiscard]]
	std::string GetInputFilePath() const
	{
		return m_inputFilePath;
	}

	[[nodiscard]]
	std::string GetOutputFilePath() const
	{
		return m_outputFilePath;
	}

private:
	StateMachineType m_stateMachineType;
	std::string m_inputFilePath, m_outputFilePath;

	inline static const std::string MEALY = "mealy";
	inline static const std::string MOORE = "moore";

	Args(StateMachineType stateMachineType, std::string inputFilePath, std::string outputFilePath)
		: m_stateMachineType(stateMachineType)
		, m_inputFilePath(std::move(inputFilePath))
		, m_outputFilePath(std::move(outputFilePath))
	{
	}

	static std::optional<StateMachineType> ParseStateMachineType(const std::string& arg)
	{
		return (arg == MEALY) ? std::optional(StateMachineType::MEALY) :
		       (arg == MOORE) ? std::optional(StateMachineType::MOORE) :
			   std::nullopt;
	}
};

#endif // LAB2_ARGS_H
