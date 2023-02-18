#ifndef LAB2_ARGS_H
#define LAB2_ARGS_H

#include "../grammar/GrammarType.h"
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

		auto grammarType = ParseGrammarType(argv[1]);
		if (!grammarType)
		{
			return std::nullopt;
		}

		return {{ grammarType.value(), std::string(argv[2]), std::string(argv[3]) }};
	}

	[[nodiscard]]
	GrammarType GetGrammarType() const
	{
		return m_grammarType;
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
	GrammarType m_grammarType;
	std::string m_inputFilePath, m_outputFilePath;

	inline static const std::string LEFT = "left";
	inline static const std::string RIGHT = "right";

	Args(GrammarType grammarType, std::string inputFilePath, std::string outputFilePath)
		: m_grammarType(grammarType)
		, m_inputFilePath(std::move(inputFilePath))
		, m_outputFilePath(std::move(outputFilePath))
	{
	}

	static std::optional<GrammarType> ParseGrammarType(const std::string& arg)
	{
		return (arg == LEFT) ? std::optional(GrammarType::LEFT) :
		       (arg == RIGHT) ? std::optional(GrammarType::RIGHT) :
			   std::nullopt;
	}
};

#endif // LAB2_ARGS_H
