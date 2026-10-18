#include <iostream>
#include <iomanip>
#include <cmath>
#include <Windows.h>

using namespace std;


double f(double x) {
    return (4.0 * x - 0.25) * atan(x) - pow(x, 3) / 12.0 - 0.75;
}


double df(double x) {
    return 4.0 * atan(x) + (4.0 * x - 0.25) / (1.0 + x * x) - 0.25 * x * x;
}

void print_table_header() {
    cout << string(105, '-') << endl;
    cout << setw(3) << "k" << " | "
        << setw(10) << "x_{k-1}" << " | "
        << setw(11) << "f'(x_{k-1})" << " | "
        << setw(10) << "x_k" << " | "
        << setw(10) << "f'(x_k)" << " | "
        << setw(10) << "x_{k+1}" << " | "
        << setw(12) << "f'(x_{k+1})" << " | "
        << setw(10) << "f(x_{k+1})" << endl;
    cout << string(105, '-') << endl;
}


void secant_method_guide(double a0, double b0, double eps) {
    cout << "\n--- Метод січних (за схемою посібника, Таблиця 4.11) ---\n";

    double xm = a0; // x_{k-1}
    double xk = b0; // x_k
    double pm = df(xm);
    double pk = df(xk);

  
    if (pm * pk >= 0) {
        cout << "Помилка: f'(x_0) та f'(x_1) повинні мати різні знаки!" << endl;
        return;
    }

    print_table_header();

    for (int k = 1; k <= 100; k++) {
 
        if (abs(pk - pm) < 1e-12) {
            cout << "Помилка: ділення на нуль (січна горизонтальна)." << endl;
            break;
        }

        double xn = (pk * xm - pm * xk) / (pk - pm);

        double pn = df(xn);
        double fn = f(xn);

        cout << setw(3) << k << " | "
            << fixed << setprecision(6)
            << setw(10) << xm << " | "
            << setw(11) << pm << " | "
            << setw(10) << xk << " | "
            << setw(10) << pk << " | "
            << setw(10) << xn << " | "
            << setw(12) << pn << " | "
            << setw(10) << fn << endl;


        if (abs(pn) <= eps) {
            cout << "\nМінімум знайдено: x* = " << xn << ", f(x*) = " << fn << ", кількість ітерацій = " << k << endl;
            break;
        }

       
        if (pn > 0) {
            xk = xn; 
            pk = pn;
        }
        else {
            xm = xn; 
            pm = pn;
        }
    }
}


void secant_method_pure(double a0, double b0, double eps) {
    cout << "\n--- Експеримент: Метод \"чистих\" січних (без перевірки знаку) ---\n";

    double xm = a0;
    double xk = b0;
    double pm = df(xm);
    double pk = df(xk);

    print_table_header();

    for (int k = 1; k <= 100; k++) {
        if (abs(pk - pm) < 1e-12) {
            cout << "Помилка: ділення на нуль (січна горизонтальна)." << endl;
            break;
        }

        double xn = (pk * xm - pm * xk) / (pk - pm);
        double pn = df(xn);
        double fn = f(xn);

        cout << setw(3) << k << " | "
            << fixed << setprecision(6)
            << setw(10) << xm << " | "
            << setw(11) << pm << " | "
            << setw(10) << xk << " | "
            << setw(10) << pk << " | "
            << setw(10) << xn << " | "
            << setw(12) << pn << " | "
            << setw(10) << fn << endl;

        if (abs(pn) <= eps) {
            cout << "\nМінімум знайдено: x* = " << xn << ", f(x*) = " << fn << ", кількість ітерацій = " << k << endl;
            break;
        }

 
        xm = xk;
        pm = pk;
        xk = xn;
        pk = pn;
    }
}

int main() {

    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    
    double a0 = 0.05; 
    double b0 = 1.0; 
    double eps = 0.001;

    cout << "Функція: f(x) = (4x - 1/4) * arctan(x) - x^3/12 - 3/4\n";
    cout << "Інтервал: [" << a0 << ", " << b0 << "], eps = " << eps << "\n";

    secant_method_guide(a0, b0, eps);
    secant_method_pure(a0, b0, eps);

    return 0;
}