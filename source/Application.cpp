#include <iostream>

#include "Parser.h"

void test_parser(const std::filesystem::path& filename) {
	Parser parser(filename);
	
	auto ast = parser.parse();
}

int main() {
	std::string_view input_file = "tests/input.program";
	
	test_parser(input_file);

	std::cin.get();
}