#include <iostream>
#include <vector>
#include <cmath>
#include <functional>
#include <iomanip>
#include <Windows.h>

using namespace std;

// Глобальні лічильники
int N_F = 0;
int N_grad = 0;

// --- Варіант 5: Квадратична функція ---
// F(x1, x2) = 2x1^2 + 3x2^2 + x1*x2 + 9x1 - 15x2
double F_var5(const vector<double>& x) {
    N_F++;
    return 2 * x[0] * x[0] + 3 * x[1] * x[1] + x[0] * x[1] + 9 * x[0] - 15 * x[1];
}

vector<double> gradF_var5(const vector<double>& x) {
    N_grad++;
    return { 4 * x[0] + x[1] + 9, 6 * x[1] + x[0] - 15 };
}

// --- Функція Розенброка ---
double F_rosen(const vector<double>& x) {
    N_F++;
    return pow(1 - x[0], 2) + 100 * pow(x[1] - x[0] * x[0], 2);
}

vector<double> gradF_rosen(const vector<double>& x) {
    N_grad++;
    return { -2 * (1 - x[0]) - 400 * x[0] * (x[1] - x[0] * x[0]),
             200 * (x[1] - x[0] * x[0]) };
}

// --- Допоміжні векторні операції ---
double norm2(const vector<double>& v) {
    return sqrt(v[0] * v[0] + v[1] * v[1]);
}
double dot_prod(const vector<double>& a, const vector<double>& b) {
    return a[0] * b[0] + a[1] * b[1];
}
vector<double> add(const vector<double>& a, const vector<double>& b) {
    return { a[0] + b[0], a[1] + b[1] };
}
vector<double> mul(const vector<double>& a, double s) {
    return { a[0] * s, a[1] * s };
}

// --- Алгоритм Свенна ---
void svenn(function<double(double)> phi, double h0, double delta, double& a, double& b) {
    double f0 = phi(h0);
    double f_right = phi(h0 + delta);
    double f_left = phi(h0 - delta);
    double step = delta;
    double h = h0;

    if (f_left >= f0 && f0 >= f_right) {
        step = delta;
        a = h0;
    }
    else if (f_left <= f0 && f0 <= f_right) {
        step = -delta;
        b = h0;
    }
    else {
        a = h0 - delta;
        b = h0 + delta;
        return;
    }

    double h_prev2 = h0;
    double h_prev = h0 + step;
    h = h_prev + step * 2;
    double f_prev = phi(h_prev);
    double f_curr = phi(h);

    while (f_curr < f_prev) {
        step *= 2;
        h_prev2 = h_prev;
        h_prev = h;
        f_prev = f_curr;
        h += step;
        f_curr = phi(h);
    }

    if (step > 0) {
        a = h_prev2; b = h;
    }
    else {
        a = h; b = h_prev2;
    }
}

// --- Метод Золотого перерізу ---
double golden_section(function<double(double)> phi, double a, double b, double eps = 1e-6) {
    const double tau = 0.6180339887;
    double L = b - a;
    double x1 = b - tau * L;
    double x2 = a + tau * L;
    double f1 = phi(x1);
    double f2 = phi(x2);

    while (L > eps) {
        if (f1 <= f2) {
            b = x2; x2 = x1; f2 = f1;
            L = b - a; x1 = b - tau * L; f1 = phi(x1);
        }
        else {
            a = x1; x1 = x2; f1 = f2;
            L = b - a; x2 = a + tau * L; f2 = phi(x2);
        }
    }
    return (a + b) / 2.0;
}

// --- Точний лінійний пошук (Свенн + Золотий переріз) ---
double line_search(function<double(const vector<double>&)> F, vector<double> x, vector<double> d) {
    auto phi = [&](double h) { return F(add(x, mul(d, h))); };
    double a, b;
    svenn(phi, 0.0, 0.01, a, b);
    return golden_section(phi, a, b, 1e-6);
}

// --- Метод Флетчера-Рівса / Антиградієнт ---
void FletcherReeves(function<double(const vector<double>&)> F,
    function<vector<double>(const vector<double>&)> gradF,
    vector<double> x0, double eps, int n, bool use_restart, bool force_antigradient = false) {
    N_F = 0; N_grad = 0; // Скидання лічильників
    vector<double> x = x0;
    vector<double> g = gradF(x);
    vector<double> d = mul(g, -1.0);
    int k = 0;

    cout << "k\tx1\t\tx2\t\tF(x)\t\t||gradF||\tbeta\t\th_k" << endl;

    while (norm2(g) > eps && k < 5000) {
        double h = line_search(F, x, d);
        vector<double> x_next = add(x, mul(d, h));
        vector<double> g_next = gradF(x_next);

        double beta = dot_prod(g_next, g_next) / dot_prod(g, g);
        if (force_antigradient) beta = 0.0;
        else if (use_restart && (k + 1) % n == 0) beta = 0.0;

        cout << k << "\t" << fixed << setprecision(5) << x[0] << "\t" << x[1] << "\t"
            << F(x) << "\t" << norm2(g) << "\t" << beta << "\t" << h << endl;

        d = add(mul(g_next, -1.0), mul(d, beta));
        x = x_next;
        g = g_next;
        k++;
    }

    cout << k << "\t" << fixed << setprecision(5) << x[0] << "\t" << x[1] << "\t"
        << F(x) << "\t" << norm2(g) << "\t-\t\t-" << endl;

    cout << "=> Знайдено мінімум: x* = (" << x[0] << ", " << x[1] << "), F(x*) = " << F(x) << endl;
    cout << "=> Ітерацій: " << k << ", Викликів F: " << N_F << ", Викликів gradF: " << N_grad << "\n\n";
}

int main() {

    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    cout << "=== ВАРІАНТ 5: Флетчер-Рівс ===" << endl;
    FletcherReeves(F_var5, gradF_var5, { -9.0, 0.0 }, 0.01, 2, true);

    cout << "=== ВАРІАНТ 5: Антиградієнт + точний пошук ===" << endl;
    FletcherReeves(F_var5, gradF_var5, { -9.0, 0.0 }, 0.01, 2, false, true);

    cout << "=== РОЗЕНБРОК: Флетчер-Рівс (з перезапуском кожні n=2) ===" << endl;
    FletcherReeves(F_rosen, gradF_rosen, { -1.2, 1.0 }, 0.01, 2, true);

    cout << "=== РОЗЕНБРОК: Флетчер-Рівс (БЕЗ перезапуску) ===" << endl;
    FletcherReeves(F_rosen, gradF_rosen, { -1.2, 1.0 }, 0.01, 2, false);

    return 0;
}