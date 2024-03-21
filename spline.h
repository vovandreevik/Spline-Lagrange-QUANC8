#pragma once

int spline(int n, int end1, int end2,
	double slope1, double slope2,
	double x[], double y[],
	double b[], double c[], double d[],
	int *iflag);

/*
2.2  Input to spline().

     n         : The number of knots.  These are numbered 0 .. n-1.
     end1        : e1=0 use natural end condition at x , ignore s1
                 e1=1 use slope s1 at x
     end2        : e2=0 use natural end condition at x   , ignore s2
                 e2=1 use slope s2 at x
     slope1        : slope (dydx) at x
     slope2        : slope (dydx) at x
     x         : vector of x (abscissa) values, x , x , ... x   .
                 Note that these values must be strictly in
                 increasing order.
     y         : vector of y (ordinate) values, y , y , ... y


2.3  Output from spline().

     b, c, d   : vectors of spline coefficients such that
                 f(x) = y  + b w + c w  + d w
                 where w = x - x  and x  < x < x   .
     iflag      : flag = 0, normal return.
                 flag = 1, n < 2, cannot interpolate.
                 flag = 2, x  are not in ascending order
*/

double seval(int n, double u,
	double x[], double y[],
	double b[], double c[], double d[],
	int *last);

/*
2.4  Input to seval().

     n         : The number of knots.
     u        : The abscissa at which the spline is to be
                 evaluated.
     x         : vector of abscissa values (as for spline())
     y         : vector of ordinate values (     "         )
     b, c, d   : vectors of spline coefficients as computed
                 by spline()
     last      : The segment that was last used to evaluate
                 the spline.  On the first call, set last to any
                 reasonable value (preferably your best guess).


2.5  Output from seval().

     seval     : The value of the spline function at xx.
     last      : The segment in which xx lies.
*/

double deriv(int n, double u,
	double x[],
	double b[], double c[], double d[],
	int *last);

double sinteg(int n, double u,
	double x[], double y[],
	double b[], double c[], double d[],
	int *last);