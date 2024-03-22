# Spline-Lagrange-QUANC8

The program calculates the value of the function for a given x and a given step using the QUANC8 program. 

_In the program_ $0 \leq x \leq 2, h = 0.2, f(x) = \displaystyle\int\limits_0^1\mathrm{e}^{xt}sin(t)\mathrm{d}t$

Builds a spline function and a Lagrange polynomial of degree 10 from the obtained points. 

Compares the values of the spline function and the polynomial with the exact values of the function at the points $x_k = (k - 0.5) h, k = 1, 2, ..., 10$.

```quanc8.h``` and ```spline.h``` are library programs
