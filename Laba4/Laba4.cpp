#include "inputData.h"
#include <iostream>
#include <cmath>
#include <string>
#include <format>
#include <array>
#include <vector>
#include <limits>
#include <algorithm>
#ifndef NOMINMAX
#define NOMINMAX
#endif
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <regex>
#include <chrono>
double recurrentFunc(double x, double eps) {
    constexpr double TWO_PI = 2.0 * 3.14159265358979323846;
    double xReduced = std::remainder(x, TWO_PI);

    double prevTerm = xReduced * xReduced * xReduced;
    double recurSum = prevTerm;

    double invNine = 1.0 / 9.0;
    int n = 1;

    while (true) {
        double nineFactor;
        if (invNine > 1e-15) {
            nineFactor = (9.0 - invNine) / (1.0 - invNine);
        }
        else {
            nineFactor = 9.0;
        }

        double nextTerm = -prevTerm * (xReduced * xReduced) * nineFactor / ((2.0 * n + 2.0) * (2.0 * n + 3.0));
        double nextSum = recurSum + nextTerm;

        if (!std::isfinite(nextTerm) || !std::isfinite(nextSum)) {
            break;
        }

        if (std::abs(nextSum - recurSum) <= eps) {
            recurSum = nextSum;
            break;
        }

        if (n >= 1000001) {
            break;
        }

        recurSum = nextSum;
        prevTerm = nextTerm;

        invNine /= 9.0;
        ++n;
    }

    return recurSum;
}



struct TableRow {
    std::string x;
    std::string recurValue;
    std::string libValue;
    std::string delta;
};


void printTable(const std::string& title, const std::array<std::string, 4>& headers, const std::vector<TableRow>& rows) {
    std::array<std::size_t, 4> widths{};

    for (std::size_t column = 0; column < headers.size(); ++column) {
        widths[column] = headers[column].length();

        for (const TableRow& row : rows) {
            const std::string* value = nullptr;

            if (column == 0) {
                value = &row.x;
            }
            else if (column == 1) {
                value = &row.recurValue;
            }
            else if (column == 2) {
                value = &row.libValue;
            }
            else {
                value = &row.delta;
            }

            widths[column] = std::max(widths[column], value->length());
        }
    }

    std::size_t tableWidth = 1;
    for (std::size_t width : widths) {
        tableWidth += width + 3;
    }

    const auto printSeparator = [&]() {
        std::cout << std::string(tableWidth, '-') << std::endl;
    };

    const auto printRow = [&](const std::array<std::string, 4>& row) {
        std::cout << "|";

        for (std::size_t column = 0; column < row.size(); ++column) {
            std::cout << std::format(" {:>{}} |", row[column], widths[column]);
        }

        std::cout << std::endl;
    };

    std::cout << title << std::endl;
    printSeparator();
    printRow(headers);
    printSeparator();

    for (const TableRow& row : rows) {
        printRow({ row.x, row.recurValue, row.libValue, row.delta });
        printSeparator();
    }
}

int main()
{
    SetConsoleCP(CP_UTF8);
    SetConsoleOutputCP(CP_UTF8);
    std::setlocale(LC_NUMERIC, "C");

    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    HWND hwnd = GetConsoleWindow();
    SetConsoleDisplayMode(hConsole, CONSOLE_FULLSCREEN_MODE, 0);

    COORD bufferSize;
    bufferSize.X = 2000;
    bufferSize.Y = 4000;
    SetConsoleScreenBufferSize(hConsole, bufferSize);

    int calcMode = 1;
    double xStart;
    double xEnd;
    double xDelta;
    double eps;

    xStart = getInput("xStart");
    xEnd = getInput("xEnd");

    if (xStart == xEnd) {
        while (true) {
            std::cout << "Диапазон значений пуст, пожалуйста введите другое значение xEnd." << std::endl << std::endl;
            xEnd = getInput("xEnd");

            if (xStart != xEnd) {
                break;
            }
        }
    }



    xDelta = getInput("xDelta");

    if (xEnd < xStart && xDelta >= 0) {
        while (true) {
            std::cout << "Так как конечный x меньше чем начальный x, шаг должен быть отрицательным. Пожалуйста, введите другое значение ∆X." << std::endl << std::endl;
            xDelta = getInput("xDelta");

            if (xDelta < 0) {
                break;
            }
        }
    }

    if (xEnd > xStart && xDelta <= 0) {
        while (true) {
            std::cout << "Так как конечный x больше чем начальный x, шаг должен быть положительным. Пожалуйста, введите другое значение ∆X." << std::endl << std::endl;
            xDelta = getInput("xDelta");

            if (xDelta > 0) {
                break;
            }
        }
    }

    double pointCount = std::abs((xEnd - xStart) / xDelta) + 1.0;

    while (pointCount > 5000) {
        std::cout
            << "Слишком много точек для построения таблицы. Пожалуйста, увеличьте размер шага." << std::endl;

        xDelta = getInput("xDelta");

        if ((xEnd > xStart && xDelta <= 0) ||
            (xEnd < xStart && xDelta >= 0)) {
            std::cout
                << "Знак шага не соответствует направлению диапазона."
                << std::endl;
            continue;
        }

        pointCount = std::abs((xEnd - xStart) / xDelta) + 1.0;
    }


    eps = getInput("epsilon");

    const std::array<std::string, 4> headers{ "x", "f(x)", "F(x)", "delta" };

    const double toler = 100.0 * std::numeric_limits<double>::epsilon() * std::max({ 1.0, std::abs(xStart), std::abs(xEnd) });
    std::vector<TableRow> rows;

    for (long long index = 0;; ++index) {
        double x = xStart + static_cast<double>(index) * xDelta;

        if (std::abs(x - xEnd) <= toler) {
            x = xEnd;
        }

        if (xDelta > 0.0) {
            if (x > xEnd + toler) {
                break;
            }
        }
        else {
            if (x < xEnd - toler) {
                break;
            }
        }

        double FLib = std::pow(std::sin(x), 3.0);
        double FRecur = recurrentFunc(x, eps);
        double delta = std::sqrt(std::abs((std::pow(FRecur, 2.0) - std::pow(FLib, 2.0))));

        std::string deltaInsert;
        std::string FRecInsert;
        std::string FLibInsert;

        if (!std::isfinite(delta)) {
            deltaInsert = "-N/A-";
            FRecInsert = "-N/A-";
            FLibInsert = "-N/A-";
        }
        else {
            deltaInsert = std::format("{:.6f}", delta);
            FRecInsert = std::format("{:.6f}", FRecur);
            FLibInsert = std::format("{:.6f}", FLib);
        }

        rows.push_back({
            std::format("{:.6f}", x),
            std::format("{}", FRecInsert),
            std::format("{}", FLibInsert),
            std::format("{}", deltaInsert)
            });
    }

    printTable("Таблица №1. Расчёт для разных X", headers, rows);

    const std::array<std::string, 4> headers2{
        "epsilon", "f(x)", "F(x)", "delta"
    };

    const double xIdeal = getInput("xIdeal");
    double FLib = std::pow(std::sin(xIdeal), 3.0);


    std::vector<TableRow> rows2;

    for (int power = 1; power <= 7; ++power) {
        const double currentEpsilon =
            std::pow(10.0, -power);

        const double FRecur =
            recurrentFunc(xIdeal, currentEpsilon);

        double delta = std::sqrt(std::abs((std::pow(FRecur, 2.0) - std::pow(FLib, 2.0))));

        std::string deltaInsert;
        std::string FRecInsert;
        std::string FLibInsert;

        if (!std::isfinite(delta)) {
            deltaInsert = "-N/A-";
            FRecInsert = "-N/A-";
            FLibInsert = "-N/A-";

        }
        else {
            deltaInsert = std::format("{:.6f}", delta);
            FRecInsert = std::format("{:.6f}", FRecur);
            FLibInsert = std::format("{:.6f}", FLib);
        }

        rows2.push_back({
            std::format("{:.{}f}", currentEpsilon, power),
            std::format("{}", FRecInsert),
            std::format("{}", FLibInsert),
            std::format("{}", deltaInsert)
            });
    }

    printTable("Таблица №2. Расчёт для разных значений ε", headers2, rows2);
    return 0;
}