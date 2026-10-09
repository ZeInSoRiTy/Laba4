#include "validation.h"
#include <regex>
#include <cmath>

ValidationError validate(const std::string& input) {
    if (input.empty()) {
        return ValidationError::EmptyInput;
    }
    if (input.back() == '.' || input.back() == ',') {
        return ValidationError::EmptyDot;
    }
    if (!std::regex_match(input, std::regex(R"(^[+-]?\d*[.,]?\d*$)"))) {
        return ValidationError::InvalidChars;
    }
    if (std::regex_match(input, std::regex(R"(^-0+([.,].0+)?$)"))) {
        return ValidationError::NegativeZero;
    }
    if (std::regex_match(input, std::regex(R"(^\+0+([.,].0+)?$)"))) {
        return ValidationError::PositiveZero;
    }
    if (std::regex_match(input, std::regex(R"(^[+-]?0\d+([.,]\d+)?$)"))) {
        return ValidationError::LeadingZeros;
    }
    if (!std::regex_match(input, std::regex(R"(^[+-]?(0|[1-9]\d*)([.,]\d+)?$)"))) {
        return ValidationError::InvalidChars;
    }
    try {
		double value = std::stod(input);
		if (!std::isfinite(value)) {
			return ValidationError::OutOfRange;
		}

	}
    catch (const std::out_of_range&) {
        return ValidationError::OutOfRange;
    }
    catch (const std::invalid_argument&) {
        return ValidationError::InvalidChars;
    }
    return ValidationError::None;
}

std::string returnValidation(const std::string& input) {
    switch (validate(input)) {
    case ValidationError::None:
        return "Valid";
    case ValidationError::EmptyInput:
        return "Введена пустая строка. Пожалуйста, не будьте динозавром и введите число в формате <число.дробь> или 0";
    case ValidationError::InvalidChars:
        return "Во вводе присутствуют буквы, специальные символы, пустые пробелы или ввод в целом некорректен. Пожалуйста, не будьте динозавром и введите число в формате <число.дробь> или 0";
    case ValidationError::EmptyDot:
        return "Число не может оканчиваться на точку. Пожалуйста, не будьте динозавром и введите число в формате <число.дробь> или 0";
    case ValidationError::NegativeZero:
        return "Введен отрицательный ноль, это недопустимо. Пожалуйста, не будьте динозавром и введите число в формате <число.дробь> или 0";
    case ValidationError::PositiveZero:
        return "Введен положительный ноль, это недопустимо. Пожалуйста, не будьте динозавром и введите число в формате <число.дробь> или 0";
    case ValidationError::LeadingZeros:
        return "Введено число с ведущими нулями. Пожалуйста, не будьте динозавром и введите число в формате <число.дробь> или 0";
    case ValidationError::OutOfRange:
		return "Введенное число слишком большое или не является double. Пожалуйста, не будьте динозавром и введите число в формате <число.дробь> или 0";
    }
    return "Неизвестная ошибка. Поздравляю, вы динозавр";
}