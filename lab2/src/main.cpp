#include "lib/args/Args.h"
#include "lib/csv/CSVReader.h"
#include "lib/csv/CSVWriter.h"
#include "lib/statemachine/Minimization.h"
#include <fstream>
#include <iostream>

int main(int argc, char* argv[])
{
	auto args = Args::ParseFromArgs(argc, argv);
	if (!args)
	{
		return EXIT_FAILURE;
	}

	Vector2D data;

	{
		std::ifstream inputFile(args->GetInputFilePath());
		if (!inputFile.good())
		{
			std::cerr << "Error: failed to open input file" << std::endl;
			return EXIT_FAILURE;
		}

		CSVReader csvReader(inputFile);
		data = csvReader.ReadAll();
		inputFile.close();
	}

	switch (args->GetStateMachineType())
	{
	case StateMachineType::MEALY:
		data = MinimizeMealyStateMachine(data);
		break;
	case StateMachineType::MOORE:
		// TODO: minimize moore
		break;
	}

	{
		std::ofstream outputFile(args->GetOutputFilePath());
		if (!outputFile.good())
		{
			std::cerr << "Error: failed to open output file" << std::endl;
			return EXIT_FAILURE;
		}

		CSVWriter csvWriter(outputFile);
		csvWriter.WriteAll(data);
		outputFile.close();
	}

	return EXIT_SUCCESS;
}