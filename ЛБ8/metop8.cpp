#include <iostream>
#include <iomanip>
#include <cmath>
#include <string>
#include <Windows.h>

using namespace std;

double F(double x1, double x2) {
    return 2.0 * x1 * x1 + 3.0 * x2 * x2 + x1 * x2;
}


void gradF(double x1, double x2, double& g1, double& g2) {
    g1 = 4.0 * x1 + x2;
    g2 = 6.0 * x2 + x1;
}



double get_norm2(double g1, double g2) {
    return sqrt(g1 * g1 + g2 * g2);
}


double sign(double val) {
    if (val > 0.0) return 1.0;
    if (val < 0.0) return -1.0;
    return 0.0;
}

void run_gradient_method(string metric_name, double x1_start, double x2_start, double h0, double eps) {
    cout << "\n=== Метрика: " << metric_name << " (h0 = " << h0 << ") ===\n";
    cout << string(110, '-') << endl;
    cout << setw(3) << "k" << " | "
        << setw(9) << "x1" << " | "
        << setw(9) << "x2" << " | "
        << setw(9) << "F" << " | "
        << setw(9) << "g1" << " | "
        << setw(9) << "g2" << " | "
        << setw(9) << "||g||2" << " | "
        << setw(6) << "d1" << " | "
        << setw(6) << "d2" << " | "
        << setw(8) << "h" << " | "
        << "дробл." << " | "
        << setw(9) << "F_new" << endl;
    cout << string(110, '-') << endl;

    double x1 = x1_start;
    double x2 = x2_start;
    double F_val = F(x1, x2);
    double h = h0;


    for (int k = 1; k <= 5000; k++) {
        double g1, g2;


        gradF(x1, x2, g1, g2);
        double norm_g = get_norm2(g1, g2);


        if (norm_g <= eps) {
            cout << "\nМінімум знайдено: x* = (" << x1 << "; " << x2 << "), F(x*) = " << F_val
                << ", кількість ітерацій = " << k - 1 << endl;
            break;
        }

        double d1 = 0, d2 = 0;


        if (metric_name == "Сферична") {
            d1 = -g1 / norm_g;
            d2 = -g2 / norm_g;
        }
        else if (metric_name == "Кубічна") {
            d1 = -sign(g1);
            d2 = -sign(g2);
        }
        else if (metric_name == "Октаедрична") {
            if (abs(g1) >= abs(g2)) { 
                d1 = -sign(g1);
                d2 = 0;
            }
            else {
                d1 = 0;
                d2 = -sign(g2);
            }
        }

        int drobl = 0;
        double x1_new, x2_new, F_new;

        while (true) {
            x1_new = x1 + h * d1;
            x2_new = x2 + h * d2;
            F_new = F(x1_new, x2_new);


            if (F_new < F_val) {
                break;
            }

 
            h /= 2.0;
            drobl++;

            if (h < 1e-10) {
                cout << "Помилка: крок здрібнів до нуля. Метод застряг." << endl;
                return;
            }
        }


        cout << setw(3) << k << " | "
            << fixed << setprecision(5)
            << setw(9) << x1 << " | "
            << setw(9) << x2 << " | "
            << setw(9) << F_val << " | "
            << setw(9) << g1 << " | "
            << setw(9) << g2 << " | "
            << setw(9) << norm_g << " | "
            << setw(6) << d1 << " | "
            << setw(6) << d2 << " | "
            << setw(8) << h << " | "
            << setw(6) << drobl << " | "
            << setw(9) << F_new << endl;


        x1 = x1_new;
        x2 = x2_new;
        F_val = F_new;
    }
}

int main() {

    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);


    double x1_start = 5.0; 
    double x2_start = -3.0; 
    double eps = 0.01;

    cout << "Старт: x0 = (" << x1_start << "; " << x2_start << "), eps = " << eps << "\n";

    run_gradient_method("Сферична", x1_start, x2_start, 1.0, eps);
    run_gradient_method("Кубічна", x1_start, x2_start, 1.0, eps);
    run_gradient_method("Октаедрична", x1_start, x2_start, 1.0, eps);

    cout << "\n\n*** ЕКСПЕРИМЕНТ З h0 = 0.7 ***\n";
    run_gradient_method("Сферична", x1_start, x2_start, 0.7, eps);
    run_gradient_method("Кубічна", x1_start, x2_start, 0.7, eps);
    run_gradient_method("Октаедрична", x1_start, x2_start, 0.7, eps);

    return 0;
}