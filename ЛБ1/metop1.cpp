#include <iostream>
#include <cmath>
#include <iomanip>
#include <vector>
#include <Windows.h>

using namespace std;

// Цільова функція для варіанта 5
double f(double x) {
    return (4.0 * x - 0.25) * atan(x) - (pow(x, 3) / 12.0) - 0.75;
}

// 1. Метод Свенна
void svenn(double x0, double delta, double& a, double& b) {
    cout << "Метод Свенна" << endl;
    double x_k = x0;
    double f_k = f(x_k);
    double x_next = x_k + delta;
    double f_next = f(x_next);

    int p = 0;
    int k = 0;

    if (f_next >= f_k) {
        delta = -delta;
        x_next = x_k + delta;
        f_next = f(x_next);
        p++;
    }

    if (f_next >= f_k && p == 1) {
        a = x0 - abs(delta);
        b = x0 + abs(delta);
        cout << "Мінімум між " << a << " та " << b << endl;
        return;
    }

    double x_prev = x_k;
    while (f_next < f_k) {
        x_prev = x_k;
        x_k = x_next;
        f_k = f_next;

        k++;
        x_next = x_k + pow(2, k) * delta;
        f_next = f(x_next);
    }

    if (delta > 0) {
        a = x_prev;
        b = x_next;
    }
    else {
        a = x_next;
        b = x_prev;
    }

    cout << "Інтервал локалізації: [" << a << ", " << b << "]" << endl;
    cout << "Витрачено ітерацій: " << k + 1 << endl;
}

// 2. Метод дихотомії
void dichotomy(double a, double b, double sigma, double epsilon) {
    cout << "\nМетод дихотомії" << endl;
    int k = 0;
    double x1, x2, f1, f2, L;

    do {
        x1 = (a + b) / 2.0 - epsilon / 2.0;
        x2 = x1 + epsilon;

        f1 = f(x1);
        f2 = f(x2);

        if (f1 < f2) {
            b = x2;
        }
        else {
            a = x1;
        }

        L = b - a;
        k++;
    } while (L > sigma);

    double x_opt = (a + b) / 2.0;
    cout << "Точка мінімуму: x* = " << x_opt << endl;
    cout << "Мінімум функції: f(x*) = " << f(x_opt) << endl;
    cout << "Кількість ітерацій: " << k << endl;
}

// 3. Метод половинного поділу
void half_division(double a, double b, double sigma) {
    cout << "\nМетод половинного поділу" << endl;
    int k = 0;
    double xm = (a + b) / 2.0;
    double fm = f(xm);
    double L = b - a;
    double x1, x2, f1, f2;

    do {
        L = b - a;
        x1 = a + L / 4.0;
        x2 = b - L / 4.0;

        f1 = f(x1);
        f2 = f(x2);

        if (f1 < fm) {
            b = xm;
            xm = x1;
            fm = f1;
        }
        else if (f2 < fm) {
            a = xm;
            xm = x2;
            fm = f2;
        }
        else {
            a = x1;
            b = x2;
            // xm та fm залишаються без змін
        }

        k++;
        L = b - a;
    } while (L > sigma);

    cout << "Точка мінімуму: x* = " << xm << endl;
    cout << "Мінімум функції: f(x*) = " << fm << endl;
    cout << "Кількість ітерацій: " << k << endl;
}

int main() {

    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    double x0 = 0.05;
    double delta = 0.1;   // Початковий крок для алгоритму Свенна
    double sigma = 0.01;  // Задана точність для методів звуження
    double epsilon = 0.001; // Відстань між пробними точками для дихотомії

    double a, b;

    // Встановлення точності виведення
    cout << fixed << setprecision(5);

    svenn(x0, delta, a, b);
    dichotomy(a, b, sigma, epsilon);
    half_division(a, b, sigma);

    return 0;
}