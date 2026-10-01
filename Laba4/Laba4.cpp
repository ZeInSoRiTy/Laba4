// Laba4.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//
#include "inputData.h"
#include <iostream>
#include <math.h>
#include <string>
#include <format>
#include <windows.h>
#include <regex>
#include <chrono>

int main()
{
    SetConsoleCP(CP_UTF8);
    SetConsoleOutputCP(CP_UTF8);

    setlocale(LC_ALL, "ru");
    std::setlocale(LC_NUMERIC, "C");


    int calcMode = 1;
    double xStart;
    double xEnd;
    double xDelta;
    double eps;

    while (true) {
        std::cout << "Меню:\n1.Таблица по диапазону X\n2.Таблица по диапазону eps\n\nВаш выбор: ";

        std::string input;
        if (!std::getline(std::cin, input)) break;

        if (input == "1" || input == "2") {
            calcMode = std::stoi(input);
            break;
        }

    }
    
    xStart = getInput("xStart");
    xEnd = getInput("xEnd");
    xDelta = getInput("xDelta");
    eps = getInput("epsilon");

    std::cout << xStart << xEnd << xDelta << eps;


}

// Запуск программы: CTRL+F5 или меню "Отладка" > "Запуск без отладки"
// Отладка программы: F5 или меню "Отладка" > "Запустить отладку"

// Советы по началу работы 
//   1. В окне обозревателя решений можно добавлять файлы и управлять ими.
//   2. В окне Team Explorer можно подключиться к системе управления версиями.
//   3. В окне "Выходные данные" можно просматривать выходные данные сборки и другие сообщения.
//   4. В окне "Список ошибок" можно просматривать ошибки.
//   5. Последовательно выберите пункты меню "Проект" > "Добавить новый элемент", чтобы создать файлы кода, или "Проект" > "Добавить существующий элемент", чтобы добавить в проект существующие файлы кода.
//   6. Чтобы снова открыть этот проект позже, выберите пункты меню "Файл" > "Открыть" > "Проект" и выберите SLN-файл.
