#ifndef COMPLEX_H
#define COMPLEX_H

#define PI 3.14159265358979f

typedef struct {
	float re;
	float im;
} complex;

extern const struct complexFunctions {
	complex (*const Multiply)(complex, complex);
	complex (*const Add)(complex, complex);
	complex (*const Sub)(complex, complex);
	float (*const Abs)(complex);
	float (*const AbsInSquare)(complex);
} Complex;

#endif
