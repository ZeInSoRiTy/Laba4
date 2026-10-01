#include "validation.h"
#include "inputData.h"
#include <regex>
#include <stdexcept>
#include <iostream>
#include <Windows.h>

double getInput(const std::string& inputType) {

	SetConsoleCP(CP_UTF8);
	SetConsoleOutputCP(CP_UTF8);

	while (true) {

		std::cout << inputMsg(inputType);
		std::string input;
		if (!std::getline(std::cin, input)) break;

		std::string validResult = returnValidation(input);

		if (validResult != "Valid") {
			std::cout << validResult << std::endl << std::endl;
			continue;
		}

		return std::stod(input);
	}

}

std::string inputMsg(const std::string& inputType) {
	if (inputType == "xStart") {
		return "Введите значение начального X: ";
	}
	if (inputType == "xEnd") {
		return "Введите значение конечного X: ";
	}
	if (inputType == "xDelta") {
		return "Введите значение шага ∆X: ";
	}
	if (inputType == "epsilon") {
		return "Введите значение точности ε: ";
	}
}