#include <iostream>
#include <execinfo.h>
#include <signal.h>
#include <stdio.h>
#include <streambuf>
#include <fstream>
#include <string>

#include "CLI.hpp"

int verboseLevel = 0;

void sigFaultHandler(int sig) {
	void *array[10];
	size_t size;

	// get void*'s for all entries on the stack
	size = backtrace(array, 10);

	// print out all the frames to stderr
	fprintf(stderr, "Error: signal %d:\n", sig);
	backtrace_symbols_fd(array, size, STDERR_FILENO);
	exit(1);
}

int		main(int argc, const char **argv) {
	try {
		CLI				cli(argc, argv);

		signal(SIGSEGV, sigFaultHandler);

		if (cli.isFlagSet("help"))
			return (0);

        cli.runProgram();

	}
	catch (std::exception &e) {
		std::cerr << e.what() << std::endl;
	}
	return (0);
}
