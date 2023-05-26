#include "pch.h"
#include "App.h"


int main() {

	Aura::App app;

	try {
		app.Run();
	}
	catch (const std::exception& e) {
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}
	


	return EXIT_SUCCESS;

}