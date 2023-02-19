#include "lib/args/Args.h"
#include "lib/csv/CSVWriter.h"
#include "lib/grammar/Grammar.h"
#include "lib/graph/Graph.h"
#include <fstream>
#include <iostream>

int main(int argc, char *argv[])
{
	auto args = Args::ParseFromArgs(argc, argv);
	if (!args)
	{
		return EXIT_FAILURE;
	}

	std::map<std::string, std::set<std::string>> rules;
	{
		std::ifstream inputFile(args->GetInputFilePath());
		if (!inputFile.good())
		{
			std::cerr << "Error: failed to open input file" << std::endl;
			return EXIT_FAILURE;
		}

		rules = ParseRules(inputFile);

		inputFile.close();
	}

	auto graph = BuildLeftGrammarGraph(rules);
	auto table = DumpGraphToCSVTable(graph);

	{
		std::ofstream outputFile(args->GetOutputFilePath());
		if (!outputFile.good())
		{
			std::cerr << "Error: failed to open output file" << std::endl;
			return EXIT_FAILURE;
		}

		CSVWriter csvWriter(outputFile);
		csvWriter.WriteAll(table);
		outputFile.close();
	}

	return EXIT_SUCCESS;
}