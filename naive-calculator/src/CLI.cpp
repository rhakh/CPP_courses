#include <iostream>
#include <fstream>
#include <iomanip>
#include <boost/lexical_cast.hpp>

#include "CLI.hpp"
#include "CalculatorParser.hpp"
#include "main.hpp"

bool	CLI::isFlagSet(const std::string &flag) const {
	if (this->vm.count(flag))
		return (true);
	return (false);
}

bool	CLI::getFlag(const std::string &flag, int &result) const {
	if (this->vm.count(flag)) {
		try {
			result = this->vm[flag].as<int>();
		}
		catch (std::exception &e) {
			std::cerr << "Error: " << e.what() << std::endl;
			return (false);
		}
		return (true);
	}
	return (false);
}

bool	CLI::getFlag(const std::string &flag, std::string &result) const {
	if (this->vm.count(flag)) {
		try {
			result = this->vm[flag].as<std::string>();
		}
		catch (std::exception &e) {
			std::cerr << "Error: " << e.what() << std::endl;
			return (false);
		}
		return (true);
	}
	return (false);
}

bool	CLI::processArguments(int argc, const char **argv) {
	namespace po = boost::program_options;
	try {
		po::store(po::parse_command_line(argc, argv, this->desc), this->vm);
		po::notify(this->vm);

		if (this->vm.count("help")) {
			std::cout << desc << std::endl;
		}
	}
	catch (po::error &e) {
		std::cerr << "Error: " << e.what() << std::endl;
		std::cerr << desc << std::endl;
		return (false);
	}
	return (true);
}

const char *program_description =
"\
The program allows you to enter into the console\n\
and calculate the simple arithmetic expressions\n\n\
Options\
";

CLI::CLI(int _argc, const char **_argv) :
desc(program_description), argc(_argc), argv(_argv)
{
	namespace po = boost::program_options;
	std::string	helpStr = std::string(
	"Print help\n\
	Example of usage:\n\
	$ " + std::string(_argv[0]) + "\n" + "\
	1\n\
	1\n\
	1 + 2\n\
	3\n\
	2 * 3 + $0 + $1\n\
	10\n\
	$2\n\
	10\n");

	this->desc.add_options()
			("help,h", helpStr.c_str());

	if (!processArguments(_argc, _argv))
		throw CLI_invalidArguments();
}

CLI::~CLI() {}

void	CLI::runProgram() const {
	std::vector<double>	memory(10, 0);
	CalculatorParser	parser;
	double result;
	bool success;
	size_t iter = 0;

	for (std::string expression; std::getline(std::cin, expression);) {
        auto begin = expression.begin(), end = expression.end();

		if (expression.empty())
			continue;

		success = qi::phrase_parse(begin, end, parser(phx::ref(memory)), qi::space, result);
		if(!success || begin != end) {
			std::cout << "Parsing failed\nstopped at: \""
					  << std::string(begin, end) << "\"\n";
		} else {
			std::cout << result << std::endl;
			memory[iter] = result;
			iter = (iter + 1) % 10;
		}
    }
}
