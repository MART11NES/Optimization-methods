#include <iostream>
#include <cmath>
#include <iomanip>
#include <Windows.h>

using namespace std;


double f(double x) {
    return (4.0 * x - 0.25) * atan(x) - (pow(x, 3) / 12.0) - 0.75;
}

int main() {

    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    double a, b, eps;

    cout << "Введіть ліву межу інтервалу a: ";
    cin >> a;
    cout << "Введіть праву межу інтервалу b: ";
    cin >> b;
    cout << "Введіть точність eps (наприклад, 0.05): ";
    cin >> eps;

    double tau = (sqrt(5.0) - 1.0) / 2.0;

    double x1 = a + (1.0 - tau) * (b - a);
    double x2 = a + tau * (b - a);

    double f1 = f(x1);
    double f2 = f(x2);

    int iter = 0;

    cout << fixed << setprecision(5);
    cout << "\n----------------------------------------------------------------------------------------\n";
    cout << "k\ta\t\tb\t\tx1\t\tx2\t\tf(x1)\t\tf(x2)\t\tL\n";
    cout << "----------------------------------------------------------------------------------------\n";

    while ((b - a) > eps) {
        cout << iter + 1 << "\t" << a << "\t" << b << "\t" << x1 << "\t" << x2 << "\t" << f1 << "\t" << f2 << "\t" << (b - a) << "\n";

        if (f1 <= f2) {

            b = x2;

            
            x2 = x1;
            f2 = f1;

            x1 = a + (1.0 - tau) * (b - a);
            f1 = f(x1);
        }
        else {
 
            a = x1;

            x1 = x2;
            f1 = f2;

            x2 = a + tau * (b - a);
            f2 = f(x2);
        }
        iter++;
    }

    double x_star = (a + b) / 2.0;

    cout << "----------------------------------------------------------------------------------------\n";
    cout << "\nРезультати:\n";
    cout << "Мінімум функції: x* = " << x_star << "\n";
    cout << "Значення функції в точці мінімуму: f(x*) = " << f(x_star) << "\n";
    cout << "Кількість ітерацій: " << iter << "\n";
    cout << "Кількість обчислень функції N_f: " << iter + 2 << "\n"; 

    return 0;
}