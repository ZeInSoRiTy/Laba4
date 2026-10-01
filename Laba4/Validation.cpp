#include "validation.h"
#include <regex>
#include <stdexcept>

ValidationError validate(const std::string& input) {
    if (input.empty()) {
        return ValidationError::EmptyInput;
    }
    if (input.back() == '.' || input.back() == ',') {
        return ValidationError::EmptyDot;
    }
    if (input.find('.') != std::string::npos || input.find(',') != std::string::npos) {
        return ValidationError::FloatNum;
    }
    if (!std::regex_match(input, std::regex(R"(^[+-]?\d+$)"))) {
        return ValidationError::InvalidChars;
    }
    if (std::regex_match(input, std::regex(R"(^-0+$)"))) {
        return ValidationError::NegativeZero;
    }
    if (std::regex_match(input, std::regex(R"(^\+0+$)"))) {
        return ValidationError::PositiveZero;
    }
    if (std::regex_match(input, std::regex(R"(^[+-]?0\d+$)"))) {
        return ValidationError::LeadingZeros;
    }
    if (!std::regex_match(input, std::regex(R"(^[+-]?(0|[1-9]\d*)$)"))) {
        return ValidationError::InvalidChars;
    }
    try {
        int value = std::stoi(input);
        (void)value;
    }
    catch (const std::out_of_range& e) {
        return ValidationError::NotInt;
    }

    return ValidationError::None;
}

std::string returnValidation(const std::string& input) {
    switch (validate(input)) {
    case ValidationError::None:
        return "Valid";
    case ValidationError::EmptyInput:
        return "Введена пустая строка. Пожалуйста, не будьте динозавром и введите число в формате <неотрицательное число>";
    case ValidationError::InvalidChars:
        return "Во вводе присутствуют буквы, специальные символы, пустые пробелы или ввод в целом некорректен.  Пожалуйста, не будьте динозавром и введите целое число в формате <неотрицательное число>";
    case ValidationError::EmptyDot:
        return "Число не может оканчиваться на точку.  Пожалуйста, не будьте динозавром и введите целое число в формате <неотрицательное число>";
    case ValidationError::NegativeZero:
        return "Введен отрицательный ноль, это недопустимо.  Пожалуйста, не будьте динозавром и введите целое число в формате <неотрицательное число>";
    case ValidationError::PositiveZero:
        return "Введен положительный ноль, это недопустимо.  Пожалуйста, не будьте динозавром и введите целое число в формате <неотрицательное число>";
    case ValidationError::LeadingZeros:
        return "Введено число с ведущими нулями.  Пожалуйста, не будьте динозавром и введите целое число в формате <неотрицательное число>";
    case ValidationError::NotInt:
        return "Введенное число является слишком большим. Пожалуйста, введите число меньше.";
    case ValidationError::FloatNum:
        return "Введенное число не является целым. Пожалуйста, не будьте динозавром и введите целое число в формате <неотрицательное число>";
    }
    return "Неизвестная ошибка. Поздравляю, вы динозавр";
}