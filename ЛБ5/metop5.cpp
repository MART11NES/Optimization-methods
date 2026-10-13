#include <iostream>
#include <cmath>
#include <vector>
#include <iomanip>
#include <algorithm>
#include <Windows.h>

double f(double x) {
    return (4.0 * x - 0.25) * atan(x) - std::pow(x, 3) / 12.0 - 0.75;
}

int main() {

    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    double x1 = 0.05;

    double dx = 0.05;

 
    double sigma_f = 0.01; 
    double sigma_x = 0.001; 

  
    double x2 = x1 + dx;
    double f1 = f(x1); 
    double f2 = f(x2); 

    double x3;
    if (f1 > f2) { 
        x3 = x1 + 2 * dx; 
    }
    else { 
        x3 = x1 - dx;
    }
    double f3 = f(x3); 

    std::vector<double> xs = { x1, x2, x3 };
    std::vector<double> fs = { f1, f2, f3 };

    std::cout << std::fixed << std::setprecision(6);
 
    std::cout << "k\tx1\t\tx2\t\tx3\t\tf(x1)\t\tf(x2)\t\tf(x3)\t\ta1\t\ta2\t\tx~\t\tf(x~)\n";

    int n = 0;

 
    for (int k = 1; k <= 50; ++k) {
    
        int i_min = std::distance(fs.begin(), std::min_element(fs.begin(), fs.end()));
        int i_max = std::distance(fs.begin(), std::max_element(fs.begin(), fs.end()));

   
        if (std::abs(xs[1] - xs[0]) < 1e-12 || std::abs(xs[2] - xs[1]) < 1e-12 || std::abs(xs[2] - xs[0]) < 1e-12) {
            std::cout << "\nЗупинка: Точки збігаються. Змініть початковий крок dx.\n";
            break;
        }


        double a1 = (fs[1] - fs[0]) / (xs[1] - xs[0]); 
        double a2 = ((fs[2] - fs[0]) / (xs[2] - xs[0]) - a1) / (xs[2] - xs[1]); 
        double xt;
        if (a2 > 0) { 
            xt = -a1 / (2 * a2) + (xs[0] + xs[1]) / 2.0; 
        }
        else { 
            xt = 2 * xs[i_min] - xs[i_max]; 
        }

        double ft = f(xt); 
        n++;

    
        std::cout << k << "\t" << xs[0] << "\t" << xs[1] << "\t" << xs[2] << "\t"
            << fs[0] << "\t" << fs[1] << "\t" << fs[2] << "\t"
            << a1 << "\t" << a2 << "\t" << xt << "\t" << ft << "\n";

        if (std::abs(fs[i_min] - ft) <= sigma_f && std::abs(xs[i_min] - xt) <= sigma_x) { 
            std::cout << "\nМінімум знайдено: x* = " << xt << ", f(x*) = " << ft << "\n"; 
            std::cout << "Кількість ітерацій n = " << n << "\n";
            std::cout << "Кількість обчислень функції Nf = " << n + 3 << "\n"; 
            break;
        }

        xs[i_max] = xt; 
        fs[i_max] = ft; 
    }

    return 0;
}