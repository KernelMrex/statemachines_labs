#include "lib/ConverterCommand.h"
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

	// В зависимости от команды читаем автомат мили или мура

	// Переводим через конвертер в другой тип

	// Выводим результат

	return EXIT_SUCCESS;
}

void PrintUsage(std::ostream& out)
{
	out << "Usage ./lab02 <command> <input-path> <output-path>" << std::endl
		<< "  command:     mealy-to-moore - converts mealy state machine to moor state machine" << std::endl
		<< "               moore-to-mealy - converts moor state machine to mealy state machine" << std::endl
		<< "  input-path:  filepath with csv stored data of moor or mealy state machine input transitions data" << std::endl
		<< "  output-path: filepath with csv stored data of moor or mealy state machine output transitions data" << std::endl;
}