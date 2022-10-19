#include "lib/command/ConverterCommand.h"
#include "lib/converter/MealyToMoorConverter.h"
#include "lib/converter/MoorToMealyConverter.h"
#include "lib/csv/CsvReader.h"
#include "lib/csv/CsvWriter.h"
#include <fstream>
#include <iostream>

void PrintUsage(std::ostream& out);

int main(int argc, char* argv[])
{
	auto command = ConverterCommand::ParseFromArgs(argc, argv);
	if (!command.has_value())
	{
		PrintUsage(std::cerr);
		return EXIT_FAILURE;
	}

	// Открываем файл на чтение
	std::ifstream inputFile(command->GetInputFilePath());
	if (!inputFile.good())
	{
		std::cerr << "Error: failed to open input file" << std::endl;
		return EXIT_FAILURE;
	}

	// Читаем входные значения
	CsvReader csvReader(inputFile);
	auto csvContent = csvReader.ReadAll();
	inputFile.close();

	// В зависимости от команды читаем автомат Мили или Мура
	vector2d converted;

	// Переводим через конвертер в другой тип
	try
	{
		switch (command->GetConversationType())
		{
		case ConversionType::MEALY_TO_MOORE:
			converted = MealyToMoorConverter::Convert(csvContent);
			break;
		case ConversionType::MOORE_TO_MEALY:
			converted = MoorToMealyConverter::Convert(csvContent);
			break;
		}
	}
	catch (const std::invalid_argument& ex)
	{
		std::cerr << "Error: could not convert. " << ex.what() << std::endl;
		return EXIT_FAILURE;
	}

	// Открываем файл на запись
	std::ofstream outputFile(command->GetOutputFilePath());
	if (!outputFile.good())
	{
		std::cerr << "Error: failed to open output file" << std::endl;
		return EXIT_FAILURE;
	}

	// Выводим результат
	CsvWriter csvWriter(outputFile);
	csvWriter.WriteAll(converted);
	outputFile.close();

	return EXIT_SUCCESS;
}

void PrintUsage(std::ostream& out)
{
	out << "Usage ./lab1 <command> <input-path> <output-path>" << std::endl
		<< "  command:     mealy-to-moore - converts mealy state machine to moor state machine" << std::endl
		<< "               moore-to-mealy - converts moor state machine to mealy state machine" << std::endl
		<< "  input-path:  filepath with csv stored data of moor or mealy state machine input transitions data" << std::endl
		<< "  output-path: filepath with csv stored data of moor or mealy state machine output transitions data" << std::endl;
}