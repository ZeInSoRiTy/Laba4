#include "validation.h"
#include "inputData.h"
#include <regex>
#include <stdexcept>
#include <iostream>
#include <Windows.h>

double getInput(const std::string& inputType) {

	while (true) {

		std::cout << inputMsg(inputType);
		std::string input;
		if (!std::getline(std::cin, input)) break;

		for (char& symbol : input) {
			if (symbol == ',') {
				symbol = '.';
			}
		}

		std::string validResult = returnValidation(input);
		

		if (validResult != "Valid") {
			std::cout << validResult << std::endl << std::endl;
			continue;
		}

		std::cout << "\n";
		
		double res = std::stod(input);

		if ((inputType == "xDelta") && res == 0) {
			std::cout << "∆X не может быть равен 0." << std::endl << std::endl;
			continue;
		}

		if ((inputType == "epsilon") && res <= 0) {
			std::cout << "ε должен быть положительным числом." << std::endl << std::endl;
			continue;
		}
		return res;
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
	if (inputType == "xIdeal") {
		return "Введите значение x_ideal: ";
	}
}