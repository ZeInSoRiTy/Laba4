// Laba4.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//
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
    //Чтобы вычислить рядную функцию для начала найдем значение первого члена этого ряда a1. 
    // a1 = 1/4 * (1 * (((3^3 - 3)/3!)*x^3)) = 1/4 * 24/6 * x^3 = x^3
    //Далее нужно вычислить следующие члены ряда по реккурентной формуле
    //Для начала нужно вычислить саму формулу
	//Для этого a_n нужно разделить на a_(n-1) и упростить выражение...
	//Итоговая формула для вычисления следующего члена ряда будет выглядеть так:
    //a_(n+1) = -a_n * ((9^(n+1)-1)/((9^n-1)(2n+2)(2n+3))) * x^2
    


	double prevTerm = pow(x, 3);
	double recurSum = prevTerm;

	int n = 1;

    while (true) {

        double nextTerm = (-prevTerm * pow(x, 2.0) * (pow(9.0, n + 1.0) - 1.0)) / ((pow(9.0, n) - 1.0) * (2.0 * n + 2.0) * (2.0 * n + 3.0));

        double nextSum = recurSum + nextTerm;

        if (!std::isfinite(nextTerm) || !std::isfinite(nextSum)) {
            break;
        }

        if (std::abs(nextSum - recurSum) < eps) {
            recurSum = nextSum;
            break;
        }

        recurSum = nextSum;
		prevTerm = nextTerm;
        ++n;
    }

    return recurSum;
}

struct TableRow {
    std::string x;
    std::string seriesValue;
    std::string libraryValue;
    std::string delta;
};

struct EpsilonTableRow {
    std::string epsilon;
    std::string seriesValue;
    std::string libraryValue;
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
                value = &row.seriesValue;
            }
            else if (column == 2) {
                value = &row.libraryValue;
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
        printRow({ row.x, row.seriesValue, row.libraryValue, row.delta });
        printSeparator();
    }
}

int main()
{
    SetConsoleCP(CP_UTF8);
    SetConsoleOutputCP(CP_UTF8);
    std::setlocale(LC_NUMERIC, "C");


    int calcMode = 1;
    double xStart;
    double xEnd;
    double xDelta;
    double eps;

    while (true) {
        std::cout << "Меню:\n1.Таблица по диапазону X\n2.Таблица по диапазону ε\n\nВаш выбор: ";

        std::string input;
        if (!std::getline(std::cin, input)) break;

        if (input == "1" || input == "2") {
            calcMode = std::stoi(input);
            std::cout << "\n";
            break;
        }

    }
    
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

    eps = getInput("epsilon");


    switch (calcMode) {
    case 1:
    {

        const std::array<std::string, 4> headers{"x", "f(x)", "F(x)", "delta"};

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

            double FLib = pow(sin(x), 3);
            double FRecur = recurrentFunc(x, eps);
            double delta = std::sqrt(std::abs((pow(FRecur, 2) - pow(FLib, 2))));

            rows.push_back({
                std::format("{}", x),
                std::format("{}", FRecur),
                std::format("{}", FLib),
                std::format("{}", delta)
            });
        }

        printTable("Таблица №1. Расчёт для разных X", headers, rows);

        break;
    }

    case 2:
    {

        const std::array<std::string, 4> headers{
        "epsilon", "f(x)", "F(x)", "delta"
        };

        const double xIdeal = getInput("xIdeal");
        double FLib = pow(sin(xIdeal), 3);

        std::vector<TableRow> rows;
        
        for (int power = 1; power <= 7; ++power) {
            const double currentEpsilon =
                std::pow(10.0, -power);

            const double FRecur =
                recurrentFunc(xIdeal, currentEpsilon);

            double delta = std::sqrt(std::abs((pow(FRecur, 2) - pow(FLib, 2))));

            rows.push_back({
                std::format("{}", currentEpsilon),
                std::format("{}", FRecur),
                std::format("{}", FLib),
                std::format("{}", delta)
                });
        }

        printTable("Таблица №2. Расчёт для разных значений epsilon", headers, rows);

        break;

    }
    return 0;
}