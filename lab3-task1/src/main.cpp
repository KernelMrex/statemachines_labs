#include "lib/args/Args.h"
#include "lib/grammar/Grammar.h"
#include <fstream>
#include <iostream>

int main(int argc, char *argv[])
{
	auto args = Args::ParseFromArgs(argc, argv);
	if (!args)
	{
		return EXIT_FAILURE;
	}

	std::ifstream inputFile(args->GetInputFilePath());
	if (!inputFile.good())
	{
		std::cerr << "Error: failed to open input file" << std::endl;
		return EXIT_FAILURE;
	}

	auto rules = ParseRules(inputFile);

	inputFile.close();

	return EXIT_SUCCESS;
}