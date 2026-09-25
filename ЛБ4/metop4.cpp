#include <iostream>
#include <cmath>
#include <iomanip>
#include <vector>
#include <Windows.h>

using namespace std;

// Цільова функція (Варіант 5)
double f(double x) {
    return (4.0 * x - 0.25) * atan(x) - (pow(x, 3) / 12.0) - 0.75;
}

int main() {

    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    double a, b, eps;

    // За замовчуванням беремо значення з результату методу Свенна
    cout << "Введіть ліву межу інтервалу a: ";
    cin >> a;
    cout << "Введіть праву межу інтервалу b: ";
    cin >> b;
    cout << "Введіть точність eps: ";
    cin >> eps;

    vector<long long> F = { 0, 1, 1 };
    double R = (b - a) / eps;

    while (F.back() < R) {
        F.push_back(F[F.size() - 1] + F[F.size() - 2]);
    }
    int N = F.size() - 1;

    cout << "\nПотрібне стиснення R = " << R << "\n";
    cout << "Знайдено N = " << N << " (F_N = " << F[N] << " >= " << R << ")\n";

    double x1 = a + (double(F[N - 2]) / F[N]) * (b - a);
    double x2 = a + (double(F[N - 1]) / F[N]) * (b - a);

    double f1 = f(x1);
    double f2 = f(x2);

    int nf_count = 2;

    cout << fixed << setprecision(6);
    cout << "\n-------------------------------------------------------------------------------------------------\n";
    cout << "k\tm\ta\t\tb\t\tx1\t\tx2\t\tf(x1)\t\tf(x2)\t\tL\n";
    cout << "-------------------------------------------------------------------------------------------------\n";

    for (int k = 0; k <= N - 3; ++k) {
        int m = N - k;

        cout << k << "\t" << m << "\t" << a << "\t" << b << "\t" << x1 << "\t" << x2 << "\t" << f1 << "\t" << f2 << "\t" << (b - a) << "\n";

        if (m > 3) {
         
            if (f1 <= f2) {
                b = x2;
                x2 = x1;
                f2 = f1; 
                x1 = a + (double(F[m - 3]) / F[m - 1]) * (b - a);
                f1 = f(x1); 
            }
            else {
                a = x1;
                x1 = x2;
                f1 = f2;
                x2 = a + (double(F[m - 2]) / F[m - 1]) * (b - a);
                f2 = f(x2);
            }
        }
        else {
      
            double delta = eps / 10.0;
            if (f1 <= f2) {
                b = x2;
                x2 = x1; 
                f2 = f1;
                x1 = x2 - delta; 
                f1 = f(x1);
            }
            else {
                a = x1;
                x1 = x2; 
                f1 = f2;
                x2 = x1 + delta; 
                f2 = f(x2);
            }
        }
        nf_count++; 
    }

    cout << (N - 2) << "\t-\t" << a << "\t" << b << "\t-\t\t-\t\t-\t\t-\t\t" << (b - a) << "\n";

    double x_star = (a + b) / 2.0;

    cout << "-------------------------------------------------------------------------------------------------\n";
    cout << "\nРезультати:\n";
    cout << "Точка мінімуму: x* = " << x_star << "\n";
    cout << "Значення функції в точці мінімуму: f(x*) = " << f(x_star) << "\n";
    cout << "Кінцева довжина інтервалу L: " << (b - a) << "\n";
    cout << "Гарантія L_кін <= L0 / F_N: " << (b - a) << " <= " << ((0.15 - (-0.05)) / F[N]) << "\n";
    cout << "Кількість ітерацій: " << (N - 2) << "\n";
    cout << "Кількість обчислень функції N_f: " << nf_count << " (Теоретична межа N-1: " << (N - 1) << ")\n";

    return 0;
}