#include "validation.h"
#include "inputData.h"
#include <regex>
#include <stdexcept>
#include <iostream>
#include <cstdlib>
#include <cmath>
#include <Windows.h>

double getInput(const std::string& inputType) {

	while (true) {

		std::cout << inputMsg(inputType);
		std::string input;
		if (!std::getline(std::cin, input)) 
		{
			std::exit(0);
		}

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

		if ((inputType == "xStart" || inputType == "xEnd" || inputType == "xDelta") && (std::abs(res) > 10000 || std::abs(res) < 0.000001)) {
			std::cout << "Для подобной функции данное значение излишне большое и уведет ее в бесконечность или излишне маленькое. Пожалуйста введите значение не больше 10000, а лучше в диапазоне от -5 до 5, так как это гарантирует устойчивость функции." << std::endl << std::endl;
			continue;
		}
		if ((inputType == "xIdeal") && (std::abs(res) >= 122 || std::abs(res) < 0.000001)) {
			std::cout << "Для подобной функции данное значение излишне большое и уведет ее в бесконечность или излишне маленькое. Пожалуйста введите значение менее 122, а лучше в диапазоне от -5 до 5, так как это гарантирует устойчивость функции." << std::endl << std::endl;
			continue;
		}

		if ((inputType == "xDelta") && (res == 0 || std::abs(res) < 0.000001)) {
			std::cout << "∆X не может быть равен 0 и должен быть более 0.000001." << std::endl << std::endl;
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
	return "Неизвестный тип параметра";
}