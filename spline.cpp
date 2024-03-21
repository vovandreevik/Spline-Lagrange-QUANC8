/* spline.c
Cubic interpolating spline. */

/************************************************/
/*                                              */
/*  CMATH.  Copyright (c) 1989 Design Software  */
/*                                              */
/************************************************/

#include <math.h>

/*-----------------------------------------------------------------*/

int spline(int n, int end1, int end2,
	double slope1, double slope2,
	double x[], double y[],
	double b[], double c[], double d[],
	int *iflag)

{  /* begin procedure spline() */

	int    nm1, ib, i;
	double t;
	int    ascend;

	nm1 = n - 1;
	*iflag = 0;

	if (n < 2)
	{  /* no possible interpolation */
		*iflag = 1;
		goto LeaveSpline;
	}

	ascend = 1;
	for (i = 1; i < n; ++i) if (x[i] <= x[i - 1]) ascend = 0;
	if (!ascend)
	{
		*iflag = 2;
		goto LeaveSpline;
	}

	if (n >= 3)
	{    /* ---- At least quadratic ---- */

		 /* ---- Set up the symmetric tri-diagonal system
		 b = diagonal
		 d = offdiagonal
		 c = right-hand-side  */
		d[0] = x[1] - x[0];
		c[1] = (y[1] - y[0]) / d[0];
		for (i = 1; i < nm1; ++i)
		{
			d[i] = x[i + 1] - x[i];
			b[i] = 2.0 * (d[i - 1] + d[i]);
			c[i + 1] = (y[i + 1] - y[i]) / d[i];
			c[i] = c[i + 1] - c[i];
		}

		/* ---- Default End conditions
		Third derivatives at x[0] and x[n-1] obtained
		from divided differences  */
		b[0] = -d[0];
		b[nm1] = -d[n - 2];
		c[0] = 0.0;
		c[nm1] = 0.0;
		if (n != 3)
		{
			c[0] = c[2] / (x[3] - x[1]) - c[1] / (x[2] - x[0]);
			c[nm1] = c[n - 2] / (x[nm1] - x[n - 3]) - c[n - 3] / (x[n - 2] - x[n - 4]);
			c[0] = c[0] * d[0] * d[0] / (x[3] - x[0]);
			c[nm1] = -c[nm1] * d[n - 2] * d[n - 2] / (x[nm1] - x[n - 4]);
		}

		/* Alternative end conditions -- known slopes */
		if (end1 == 1)
		{
			b[0] = 2.0 * (x[1] - x[0]);
			c[0] = (y[1] - y[0]) / (x[1] - x[0]) - slope1;
		}
		if (end2 == 1)
		{
			b[nm1] = 2.0 * (x[nm1] - x[n - 2]);
			c[nm1] = slope2 - (y[nm1] - y[n - 2]) / (x[nm1] - x[n - 2]);
		}

		/* Forward elimination */
		for (i = 1; i < n; ++i)
		{
			t = d[i - 1] / b[i - 1];
			b[i] = b[i] - t * d[i - 1];
			c[i] = c[i] - t * c[i - 1];
		}

		/* Back substitution */
		c[nm1] = c[nm1] / b[nm1];
		for (ib = 0; ib < nm1; ++ib)
		{
			i = n - ib - 2;
			c[i] = (c[i] - d[i] * c[i + 1]) / b[i];
		}

		/* c[i] is now the sigma[i] of the text */

		/* Compute the polynomial coefficients */
		b[nm1] = (y[nm1] - y[n - 2]) / d[n - 2] + d[n - 2] * (c[n - 2] + 2.0 * c[nm1]);
		for (i = 0; i < nm1; ++i)
		{
			b[i] = (y[i + 1] - y[i]) / d[i] - d[i] * (c[i + 1] + 2.0 * c[i]);
			d[i] = (c[i + 1] - c[i]) / d[i];
			c[i] = 3.0 * c[i];
		}
		c[nm1] = 3.0 * c[nm1];
		d[nm1] = d[n - 2];

	}  /* at least quadratic */

	else  /* if n >= 3 */
	{  /* linear segment only  */
		b[0] = (y[1] - y[0]) / (x[1] - x[0]);
		c[0] = 0.0;
		d[0] = 0.0;
		b[1] = b[0];
		c[1] = 0.0;
		d[1] = 0.0;
	}

LeaveSpline:
	return 0;
}  /* end of spline() */
   /*-------------------------------------------------------------------*/


double seval(int n, double u,
	double x[], double y[],
	double b[], double c[], double d[],
	int *last)

{  /* begin function seval() */

	int    i, j, k;
	double w;

	i = *last;
	if (i >= n - 1) i = 0;
	if (i < 0)  i = 0;

	if ((x[i] > u) || (x[i + 1] < u))
	{  /* ---- perform a binary search ---- */
		i = 0;
		j = n;
		do
		{
			k = (i + j) / 2;         /* split the domain to search */
			if (u < x[k])  j = k;    /* move the upper bound */
			if (u >= x[k]) i = k;    /* move the lower bound */
		}                        /* there are no more segments to search */
		while (j > i + 1);
	}
	*last = i;

	/* ---- Evaluate the spline ---- */
	w = u - x[i];
	w = y[i] + w * (b[i] + w * (c[i] + w * d[i]));
	return (w);
}
/*-------------------------------------------------------------------*/

double deriv(int n, double u,
	double x[],
	double b[], double c[], double d[],
	int *last)

{  /* begin function deriv() */

	int    i, j, k;
	double w;

	i = *last;
	if (i >= n - 1) i = 0;
	if (i < 0) i = 0;

	if ((x[i] > u) || (x[i + 1] < u))
	{  /* ---- perform a binary search ---- */
		i = 0;
		j = n;
		do
		{
			k = (i + j) / 2;          /* split the domain to search */
			if (u < x[k])  j = k;     /* move the upper bound */
			if (u >= x[k]) i = k;     /* move the lower bound */
		}                         /* there are no more segments to search */
		while (j > i + 1);
	}
	*last = i;

	/* ---- Evaluate the derivative ---- */
	w = u - x[i];
	w = b[i] + w * (2.0 * c[i] + w * 3.0 * d[i]);
	return (w);

} /* end of deriv() */

  /*-------------------------------------------------------------------*/

double sinteg(int n, double u,
	double x[], double y[],
	double b[], double c[], double d[],
	int *last)

/*Purpose ...
-------
Integrate the cubic spline function

S(xx) = y[i] + b[i] * w + c[i] * w**2 + d[i] * w**3
where w = u - x[i]
and   x[i] <= u <= x[i+1]

The integral is zero at u = x[0].

If u < x[0]   then i = 0 segment is extrapolated.
If u > x[n-1] then i = n-1 segment is extrapolated.

Input :
-------
n       : The number of data points or knots (n >= 2)
u       : the abscissa at which the spline is to be evaluated
Last    : the segment that was last used to evaluate U
x[]     : the abscissas of the knots in strictly increasing order
y[]     : the ordinates of the knots
b, c, d : arrays of spline coefficients computed by spline().

Output :
--------
sinteg  : the value of the spline function at u
Last    : the segment in which u lies

Notes ...
-----
(1) If u is not in the same interval as the previous call then a
binary search is performed to determine the proper interval.

*/
/*-------------------------------------------------------------------*/

{  /* begin function sinteg() */

	int    i, j, k;
	double sum, dx;

	i = *last;
	if (i >= n - 1) i = 0;
	if (i < 0)  i = 0;

	if ((x[i] > u) || (x[i + 1] < u))
	{  /* ---- perform a binary search ---- */
		i = 0;
		j = n;
		do
		{
			k = (i + j) / 2;         /* split the domain to search */
			if (u < x[k])  j = k;    /* move the upper bound */
			if (u >= x[k]) i = k;    /* move the lower bound */
		}                        /* there are no more segments to search */
		while (j > i + 1);
	}
	*last = i;

	sum = 0.0;
	/* ---- Evaluate the integral for segments x < u ---- */
	for (j = 0; j < i; ++j)
	{
		dx = x[j + 1] - x[j];
		sum += dx *
			(y[j] + dx *
			(0.5 * b[j] + dx *
				(c[j] / 3.0 + dx * 0.25 * d[j])));
	}

	/* ---- Evaluate the integral fot this segment ---- */
	dx = u - x[i];
	sum += dx *
		(y[i] + dx *
		(0.5 * b[i] + dx *
			(c[i] / 3.0 + dx * 0.25 * d[i])));

	return (sum);
}
/*-------------------------------------------------------------------*/
