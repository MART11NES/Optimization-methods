#include <iostream>
#include <iomanip>
#include <cmath>
#include <Windows.h>

using namespace std;

// Цільова функція
double f(double x) {
    return (4.0 * x - 0.25) * atan(x) - pow(x, 3) / 12.0 - 0.75;
}

// Перша похідна
double df(double x) {
    return 4.0 * atan(x) + (4.0 * x - 0.25) / (1.0 + x * x) - 0.25 * x * x;
}

// Друга похідна
double d2f(double x) {
    return (8.0 + 0.5 * x - 4.0 * x * x) / pow(1.0 + x * x, 2) - 0.5 * x;
}

void print_separator(int width) {
    cout << string(width, '-') << endl;
}

void newton_classical(double x0, double eps) {
    cout << "\n--- Частина А: Класичний метод Ньютона ---\n";
    print_separator(75);
    cout << setw(3) << "k" << " | "
        << setw(10) << "xk" << " | "
        << setw(10) << "f(xk)" << " | "
        << setw(10) << "f'(xk)" << " | "
        << setw(10) << "|f'(xk)|" << " | "
        << setw(10) << "f''(xk)" << endl;
    print_separator(75);

    double x = x0;
    int k = 0;

    while (true) {
        double fx = f(x);
        double dfx = df(x);
        double abs_dfx = abs(dfx);
        double d2fx = d2f(x);

        cout << setw(3) << k << " | "
            << fixed << setprecision(6)
            << setw(10) << x << " | "
            << setw(10) << fx << " | "
            << setw(10) << dfx << " | "
            << setw(10) << abs_dfx << " | "
            << setw(10) << d2fx << endl;

        if (abs_dfx <= eps) {
            cout << "\nМінімум знайдено: x* = " << x << ", f(x*) = " << fx << endl;
            break;
        }

        if (d2fx <= 0) {
            cout << "Помилка: f''(x) <= 0. Метод може не збігатися до мінімуму." << endl;
            break;
        }

        x = x - dfx / d2fx;
        k++;

        if (k > 50) {
            cout << "Досягнуто ліміт ітерацій." << endl;
            break;
        }
    }
}

void newton_raphson(double x0, double eps, double alpha) {
    cout << "\n--- Частина Б: Ньютон-Рафсон із регулюванням кроку ---\n";
    print_separator(115);
    cout << setw(3) << "k" << " | "
        << setw(9) << "xk" << " | "
        << setw(9) << "f(xk)" << " | "
        << setw(9) << "f'(xk)" << " | "
        << setw(9) << "f''(xk)" << " | "
        << setw(8) << "t" << " | "
        << setw(9) << "xb" << " | "
        << setw(9) << "f(xb)" << " | "
        << setw(9) << "f'(xb)" << " | "
        << "(3.10)" << endl;
    print_separator(115);

    double x = x0;
    double fx = f(x);
    double d1 = df(x);
    double d2 = d2f(x);

    for (int k = 0; k < 50; k++) {
        if (abs(d1) <= eps) {
            cout << "\nМінімум знайдено: x* = " << x << ", f(x*) = " << fx << endl;
            break;
        }

        if (d2 <= 0) {
            cout << "Помилка: f''(x) <= 0." << endl;
            break;
        }

        double t = 1.0;
        bool is_first_trial = true;

        while (true) {
            double xb = x - t * d1 / d2;
            double d1b = df(xb);
            double fb = f(xb);

            // Перевірка умови (3.10)
            double promised_decrease = -alpha * t * pow(d1, 2) / d2;
            bool condition_met = (fb - fx <= promised_decrease);

            // Виведення рядка для кожної спроби кроку t
            if (is_first_trial) {
                cout << setw(3) << k << " | "
                    << fixed << setprecision(5)
                    << setw(9) << x << " | "
                    << setw(9) << fx << " | "
                    << setw(9) << d1 << " | "
                    << setw(9) << d2 << " | ";
                is_first_trial = false;
            }
            else {
                cout << setw(3) << " " << " | "
                    << setw(9) << " " << " | "
                    << setw(9) << " " << " | "
                    << setw(9) << " " << " | "
                    << setw(9) << " " << " | ";
            }

            cout << setw(8) << t << " | "
                << setw(9) << xb << " | "
                << setw(9) << fb << " | "
                << setw(9) << d1b << " | "
                << (condition_met ? "  Так" : "   Ні") << endl;

            if (abs(d1b) <= eps) {
                x = xb;
                fx = fb;
                d1 = d1b;
                break;
            }

            if (condition_met) {
                x = xb;
                fx = fb;
                d1 = d1b;
                d2 = d2f(x);
                break;
            }

            t /= 2.0;
            if (t < 1e-8) {
                cout << "Крок занадто малий, зупинка." << endl;
                return;
            }
        }
    }
}

int main() {

    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    double x0 = 0.05; 
    double eps = 0.001;
    double alpha = 0.5;

    cout << "Функція: f(x) = (4x - 1/4) * arctan(x) - x^3/12 - 3/4" << endl;
    cout << "Початкова точка x0 = " << x0 << ", eps = " << eps << "\n";

    newton_classical(x0, eps);
    newton_raphson(x0, eps, alpha);

    return 0;
}