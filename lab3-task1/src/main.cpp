#include "lib/args/Args.h"
#include <cstdlib>

int main(int argc, char *argv[])
{
	auto args = Args::ParseFromArgs(argc, argv);
	if (!args)
	{
		return EXIT_FAILURE;
	}



	return EXIT_SUCCESS;
}