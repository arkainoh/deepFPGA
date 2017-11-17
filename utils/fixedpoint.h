#ifndef FIXEDPOINT_H
#define FIXEDPOINT_H

#include <ieee754.h>
#include <stdio.h>

#define SIGN 1
#define EXPONENT 8
#define MANTISSA 23
#define EXP_BIAS 127
#define INT_SIZE 32

typedef short fix16;
typedef char fix8;

int fix(float f, int wl, int iwl);
void print_binary(int num, int len);
void printb_fix(int num, int wl, int iwl);
void printd_fix(unsigned int num, int wl, int iwl);
int mul_fix(int a, int b, int wl, int iwl);
int div_fix(int a, int b, int wl, int iwl);

#endif
