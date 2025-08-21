#include <iostream>
#include <filesystem>
#include "LimeStone/LimeStone.h"

int main(int argc, char** argv) {
	try {
		std::filesystem::path exeDir = std::filesystem::absolute(argv[0]).parent_path();
		LimeStone::Application app = LimeStone::Application(exeDir.generic_string());
	} 
	catch (const std::exception& e) {
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}