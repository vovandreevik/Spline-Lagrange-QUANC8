#include <iostream>
#include "quanc8.h"
#include "spline.h"
#include <iomanip>


double x = 0;


double f(double t) {
	return (exp(x*t) * sin(t));
}


double lagrange(double value, double* stepValues, double* functionValues, int size) {
    double lagrange = 0;
    double temp = 0;
    for (int i = 0; i < size; i++)
    {
        temp = 1;
        for (int j = 0; j < size; j++)
        {
            if (j != i) {
                temp *= (value - stepValues[j]) / (stepValues[i] - stepValues[j]);
            }
        }
        lagrange += temp * functionValues[i];
    }
    return lagrange;
}


int main() {
    const int SIZE = 11;
    double functionValues[SIZE] = { 0 };
    double stepValues[SIZE] = { 0 };
    const double END_OF_X = 2;
    const double H = 0.2;

    std::cout << "QUANC8\n";

    const double A = 0, B = 1, ABSERR = 0.0, RELERR = 1.0e-10;
    double result = 0, errest = 0, posn = 0; int nofun = 0, flag = 0;

    std::cout << std::setw(1) << "x"
        << std::setw(16) << "result"
        << std::setw(17) << "errest"
        << std::setw(19) << "nofun"
        << std::setw(10) << "posn"
        << std::setw(10) << "flag"
        << std::endl;

    int i = 0;

    for (x; x <= END_OF_X; x += H) {

        quanc8(f, A, B, ABSERR, RELERR, &result, &errest, &nofun, &posn, &flag);

        std::cout << std::setw(3) << x
            << std::setw(20) << std::setprecision(10) << result
            << std::setw(20) << std::setprecision(10) << errest
            << std::setw(10) << nofun
            << std::setw(10) << posn
            << std::setw(10) << flag
            << std::endl;
        stepValues[i] = x;
        functionValues[i] = result;
        i++;
    }

    std::cout << "\nComparison of the values of the spline function and the Lagrange polynomial\n";

    double b[SIZE], c[SIZE], d[SIZE];
    int splineFlag = 0, last = 0;
    
    spline(SIZE, 0, 0, 0, 0, stepValues, functionValues, b, c, d, &splineFlag);

    if (splineFlag == 0){
        std::cout << std::setw(1) << "xk"
            << std::setw(13) << "f(x)"
            << std::setw(29) << "lagrange"
            << std::setw(23) << "spline"
            << std::setw(31) << "f(x) - lagrange"
            << std::setw(23) << "f(x) - spline"
            << std::endl;

        const int BEGIN_OF_K = 1, END_OF_K = 10;
        for (int k = BEGIN_OF_K; k <= END_OF_K; k++){
            x = (k - 0.5) * H;
            double lagrangeResult = lagrange(x, stepValues, functionValues, SIZE);
            double sevalResult = seval(SIZE, x, stepValues, functionValues, b, c, d, &last);

            //exact value
            quanc8(f, A, B, ABSERR, RELERR, &result, &errest, &nofun, &posn, &flag);

            std::cout << std::setw(3) << x
                << std::setw(25) << std::setprecision(15) << result
                << std::setw(25) << std::setprecision(15) << lagrangeResult
                << std::setw(25) << std::setprecision(15) << sevalResult
                << std::setw(25) << std::setprecision(15) << result - lagrangeResult
                << std::setw(25) << std::setprecision(15) << result - sevalResult
                << std::endl;
        }
    }

	return 0;
}