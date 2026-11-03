#include <iostream>
#include <vector>
#include <cmath>
#include <iomanip>
#include <functional>
#include <Windows.h>

using namespace std;

int N_F = 0; // Лічильник обчислень функції

// Варіант 5: F(x1, x2) = 2x1^2 + 3x2^2 + x1*x2 + 9x1 - 15x2
double F_var5(const vector<double>& x) {
    N_F++;
    return 2 * x[0] * x[0] + 3 * x[1] * x[1] + x[0] * x[1] + 9 * x[0] - 15 * x[1];
}

// Негладка функція з "кутами" (слайд 12)
double F_nonsmooth(const vector<double>& x) {
    N_F++;
    return 2 * abs(x[0] - 1) + 3 * abs(x[1] + 2) + 0.5 * pow(x[0] - 1, 2);
}

// Функція Розенброка
double F_rosen(const vector<double>& x) {
    N_F++;
    return pow(1 - x[0], 2) + 100 * pow(x[1] - x[0] * x[0], 2);
}

// Дослідний пошук
void explore(function<double(const vector<double>&)> F, vector<double>& z, double& Fz, double delta) {
    for (size_t i = 0; i < z.size(); i++) {
        vector<double> t = z;

        // Проба "+"
        t[i] += delta;
        double Ft = F(t);
        if (Ft < Fz) {
            z = t;
            Fz = Ft;
            continue;
        }

        // Проба "-"
        t[i] = z[i] - delta;
        Ft = F(t);
        if (Ft < Fz) {
            z = t;
            Fz = Ft;
        }
    }
}

// Метод Хука-Дживса
void HookeJeeves(function<double(const vector<double>&)> F, vector<double> x0, double delta0, double eps) {
    N_F = 0; // Скидання лічильника
    vector<double> base = x0;
    double Fb = F(base);

    vector<double> y = base;
    double Fy = Fb;
    double delta = delta0;
    bool from_pattern = false;
    int k = 0;

    cout << "k\tДія\t\t\t\tx1\tx2\tF\tdelta\tN_F" << endl;
    cout << k << "\tСтарт\t\t\t\t" << base[0] << "\t" << base[1] << "\t" << Fb << "\t" << delta << "\t" << N_F << endl;

    while (delta >= eps) {
        vector<double> z = y;
        double Fz = Fy;
        explore(F, z, Fz, delta);

        if (Fz < Fb) { // Успіх: нова база
            vector<double> prev_base = base;
            base = z;
            Fb = Fz;
            k++;

            // Зразок
            y = { base[0] + (base[0] - prev_base[0]), base[1] + (base[1] - prev_base[1]) };
            Fy = F(y);
            from_pattern = true;

            cout << k << "\tНова база (досл. пошук)\t\t" << base[0] << "\t" << base[1] << "\t" << Fb << "\t" << delta << "\t" << N_F << endl;
            cout << k << "\tСтрибок за зразком\t\t" << y[0] << "\t" << y[1] << "\t" << Fy << "\t" << delta << "\t" << N_F << endl;
        }
        else if (from_pattern) { // Невдача зі зразка -> Повернення в базу
            y = base;
            Fy = Fb;
            from_pattern = false;
            cout << k << "\tПовернення в базу\t\t" << base[0] << "\t" << base[1] << "\t" << Fb << "\t" << delta << "\t" << N_F << endl;
        }
        else { // Невдача з бази -> Дроблення кроку
            delta /= 2.0;
            cout << k << "\tДроблення кроку\t\t\t" << base[0] << "\t" << base[1] << "\t" << Fb << "\t" << delta << "\t" << N_F << endl;
        }
    }

    cout << "=> Знайдено мінімум: x* = (" << base[0] << ", " << base[1] << "), F(x*) = " << Fb << endl;
    cout << "=> Всього обчислень функції (N_F) = " << N_F << "\n\n";
}

int main() {

    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    cout << fixed << setprecision(4);

    cout << "=== ВАРІАНТ 5: Хук-Дживс (delta0 = 1.0, eps = 0.01) ===" << endl;
    HookeJeeves(F_var5, { -9.0, 0.0 }, 1.0, 0.01);

    cout << "=== ВАРІАНТ 5: Хук-Дживс (delta0 = 0.7, eps = 0.01) ===" << endl;
    HookeJeeves(F_var5, { -9.0, 0.0 }, 0.7, 0.01);

    cout << "=== НЕГЛАДКА ФУНКЦІЯ (delta0 = 1.0, eps = 0.01) ===" << endl;
    HookeJeeves(F_nonsmooth, { 5.0, -3.0 }, 1.0, 0.01);

    cout << "=== РОЗЕНБРОК: пастка (delta0 = 1.0, eps = 0.01) ===" << endl;
    HookeJeeves(F_rosen, { -1.2, 1.0 }, 1.0, 0.01);

    cout << "=== РОЗЕНБРОК: пройдено (delta0 = 1.0, eps = 1e-4) ===" << endl;
    HookeJeeves(F_rosen, { -1.2, 1.0 }, 1.0, 1e-4);

    return 0;
}