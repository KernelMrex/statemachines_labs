#ifndef CONVERTERCOMMAND_H
#define CONVERTERCOMMAND_H

#include "../converter/ConversionType.h"
#include <optional>
#include <stdexcept>

class ConverterCommand
{
public:
	std::optional<ConverterCommand> static ParseFromArgs(int argc, char* argv[])
	{
		if (argc != 4)
		{
			return std::nullopt;
		}

		auto conversationType = ParseConversationType(std::string(argv[1]));
		if (!conversationType.has_value())
		{
			return std::nullopt;
		}

		return {{
			conversationType.value(),
			std::string(argv[2]),
			std::string(argv[3])
		}};
	}

	[[nodiscard]] std::string GetInputFilePath() const
	{
		return m_inputPath;
	}

	[[nodiscard]] std::string GetOutputFilePath() const
	{
		return m_outputPath;
	}

	[[nodiscard]] ConversionType GetConversationType() const
	{
		return m_conversionType;
	}

private:
	inline static const std::string MEALY_TO_MOOR_CONVERSATION_TYPE = "mealy-to-moore";
	inline static const std::string MOOR_TO_MEALY_CONVERSATION_TYPE = "moore-to-mealy";

	ConversionType m_conversionType;
	std::string m_inputPath, m_outputPath;

	ConverterCommand(ConversionType conversionType, std::string inputPath, std::string outputPath)
		: m_conversionType(conversionType)
		, m_inputPath(std::move(inputPath))
		, m_outputPath(std::move(outputPath))
	{
	}

	[[nodiscard]] std::optional<ConversionType> static ParseConversationType(const std::string& conversationType)
	{
		if (conversationType == MEALY_TO_MOOR_CONVERSATION_TYPE)
		{
			return ConversionType::MEALY_TO_MOORE;
		}
		else if (conversationType == MOOR_TO_MEALY_CONVERSATION_TYPE)
		{
			return ConversionType::MOORE_TO_MEALY;
		}

		return std::nullopt;
	}
};

#endif // CONVERTERCOMMAND_H
