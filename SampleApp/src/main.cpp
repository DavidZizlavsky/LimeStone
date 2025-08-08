#include <iostream>
#include "LimeStone/LimeStone.h"

int main() {
	try {
		LimeStone::Application app = LimeStone::Application();
	} 
	catch (const std::exception& e) {
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}