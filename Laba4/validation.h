#pragma once
#include <string>

enum class ValidationError {
    None,
    EmptyInput,
    InvalidChars,
    LeadingZeros,
    EmptyDot,
    NegativeZero,
    PositiveZero,
    FloatNum,
    NotInt
};

ValidationError validate(const std::string& input);
std::string returnValidation(const std::string& input);