#include <iostream>
#include <cmath>
#include <iomanip>
#include <algorithm>
#include <Windows.h>

using namespace std;

// Цільова функція для варіанта 5
double f(double x) {
    return (4.0 * x - 0.25) * atan(x) - pow(x, 3) / 12.0 - 0.75;
}

// 1. Алгоритм Свенна
void svenn(double x0, double delta0, double& a_out, double& b_out) {
    cout << "ЛАБОРАТОРНА РОБОТА №1: МЕТОД СВЕННА\n";

    double f0 = f(x0);
    cout << "Початкова точка: x0 = " << x0 << ", f(x0) = " << f0 << "\n";

    double x_right = x0 + delta0;
    double f_right = f(x_right);

    double delta, x_prev, x_curr, f_curr;
    int k = 1;

    if (f_right < f0) {
        delta = delta0;
        x_prev = x0;
        x_curr = x_right;
        f_curr = f_right;
    }
    else {
        delta = -delta0;
        double x_left = x0 + delta;
        double f_left = f(x_left);

        if (f_left >= f0) {
            a_out = min(x_left, x_right);
            b_out = max(x_left, x_right);
            cout << "Мінімум знайдено між лівою та правою точками.\n";
            cout << "[a0; b0] = [" << a_out << "; " << b_out << "]\n\n";
            return;
        }
        x_prev = x0;
        x_curr = x_left;
        f_curr = f_left;
    }

    while (true) {
        double x_next = x_curr + pow(2, k) * delta;
        double f_next = f(x_next);

        if (f_next >= f_curr) {
            a_out = min(x_prev, x_next);
            b_out = max(x_prev, x_next);
            break;
        }

        x_prev = x_curr;
        x_curr = x_next;
        f_curr = f_next;
        k++;
    }

    cout << "Початковий інтервал невизначеності:\n";
    cout << "[a0; b0] = [" << a_out << "; " << b_out << "]\n";
    cout << "Довжина L0 = " << b_out - a_out << "\n\n";
}

// 2. Метод дихотомії
void dichotomy(double a, double b, double sigma, double epsilon) {
    cout << "ЛАБОРАТОРНА РОБОТА №2: МЕТОД ДИХОТОМІЇ\n";
    cout << setw(3) << "k" << " | "
        << setw(10) << "x1" << " | "
        << setw(10) << "x2" << " | "
        << setw(10) << "f(x1)" << " | "
        << setw(10) << "f(x2)" << " | "
        << setw(10) << "a" << " | "
        << setw(10) << "b" << " | "
        << setw(10) << "L" << "\n";

    int k = 0;
    int nf = 0;

    while ((b - a) > sigma) {
        double x1 = (a + b) / 2.0 - epsilon / 2.0;
        double x2 = (a + b) / 2.0 + epsilon / 2.0;
        double f1 = f(x1);
        double f2 = f(x2);
        nf += 2;

        cout << fixed << setprecision(5);
        cout << setw(3) << k << " | " << setw(10) << x1 << " | " << setw(10) << x2 << " | "
            << setw(10) << f1 << " | " << setw(10) << f2 << " | ";

        if (f1 < f2) {
            b = x2;
        }
        else if (f1 > f2) {
            a = x1;
        }
        else {
            a = x1;
            b = x2;
        }

        cout << setw(10) << a << " | " << setw(10) << b << " | " << setw(10) << (b - a) << "\n";
        k++;
    }

    double x_min = (a + b) / 2.0;
    cout << "\nx* = " << setprecision(6) << x_min << "\n";
    cout << "f(x*) = " << f(x_min) << "\n";
    cout << "Кількість обчислень функції Nf = " << nf << "\n\n";
}

int main() {

    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    // Вхідні дані
    double x0 = 0.05;
    double delta0 = 0.02;
    double sigma = 0.001;
    double epsilon = 0.0001;

    double a0 = 0, b0 = 0;

    // Виконання методів
    svenn(x0, delta0, a0, b0);
    dichotomy(a0, b0, sigma, epsilon);

    return 0;
}