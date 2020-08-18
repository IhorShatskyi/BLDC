#include "complex.h"
#include "math.h"

static complex complexMultiply(complex, complex);
static complex complexAdd(complex, complex);
static complex complexSub(complex, complex);
static float complexAbs(complex);
static float complexAbsInSquare(complex);

const struct complexFunctions Complex = {
	complexMultiply,
	complexAdd,
	complexSub,
	complexAbs,
	complexAbsInSquare
};

static complex complexMultiply(complex a, complex b){return (complex){a.re * b.re - a.im * b.im, a.re * b.im + a.im * b.re};}
static complex complexAdd(complex a, complex b){return (complex){a.re + b.re, a.im + b.im};}
static complex complexSub(complex a, complex b){return (complex){a.re - b.re, a.im - b.im};}
static float complexAbs(complex a){return sqrtf(a.re * a.re + a.im * a.im);}
static float complexAbsInSquare(complex a){return a.re * a.re + a.im * a.im;}
