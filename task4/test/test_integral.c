#include <criterion/criterion.h>
#include <criterion/internal/assert.h>
#include <math.h>
#include <stdio.h>

#include "integral.h"

#define EPS 0.1

double f1(double x) { return sin(x); }
#if 1
Test(integral1, computeDefiniteIntegral1) {
    double a = 0;
    double b = 10;

    double integral = computeDefiniteIntegral(a, b, f1);
    double expected = 1.8390715;

    printf("\n[integral = %g; expected = %g]\n\n", integral, expected);

    cr_expect_float_eq(integral, expected, EPS);
}
#endif

double f2(double x) { return cos(x); }
#if 1
Test(integral2, computeDefiniteIntegral2) {
    double a = 0;
    double b = 2 * M_PI;

    double integral = computeDefiniteIntegral(a, b, f2);
    double expected = 0;

    printf("\n[integral = %g; expected = %g]\n\n", integral, expected);

    cr_expect_float_eq(integral, expected, EPS);
}
#endif

double f3(double x) { return x * x; }
#if 1
Test(integral3, computeDefiniteIntegral3) {
    double a = 0;
    double b = 2;

    double integral = computeDefiniteIntegral(a, b, f3);
    double expected = 2.6666667;

    printf("\n[integral = %g; expected = %g]\n\n", integral, expected);

    cr_expect_float_eq(integral, expected, EPS);
}
#endif

double f4(double x) { return x * x * x; }
#if 1
Test(integral4, computeDefiniteIntegral4) {
    double a = -1;
    double b = 2;

    double integral = computeDefiniteIntegral(a, b, f4);
    double expected = 3.75;

    printf("\n[integral = %g; expected = %g]\n\n", integral, expected);

    cr_expect_float_eq(integral, expected, EPS);
}
#endif

double f5(double x) { return x < 0 ? -x : x; }
#if 1
Test(integral5, computeDefiniteIntegral5) {
    double a = -2;
    double b = 2;

    double integral = computeDefiniteIntegral(a, b, f5);
    double expected = 4;

    printf("\n[integral = %g; expected = %g]\n\n", integral, expected);

    cr_expect_float_eq(integral, expected, EPS);
}
#endif

double f6(double x) { return exp(x); }
#if 1
Test(integral6, computeDefiniteIntegral6) {
    double a = -40;
    double b = 0;

    double integral = computeDefiniteIntegral(a, b, f6);
    
    double expected = 1;

    cr_expect_float_eq(integral, expected, EPS);
}
#endif
