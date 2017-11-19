#include "fixedpoint.h"

int fix(float f, int wl, int iwl) {
	ieee754_float standard;
	standard.f = f;

	int ret = standard.ieee.mantissa | (1 << MANTISSA);
	int exp = standard.ieee.exponent - EXP_BIAS;
	int fwl = wl - iwl - 1;
	int fraction = MANTISSA - exp;
	int filter = (1 << wl) - 1;

	if(fraction > INT_SIZE) {
		ret = ret >> fraction - INT_SIZE;
		fraction = INT_SIZE;
	}
	
	ret = ret >> fraction - fwl;

	if(standard.ieee.negative) {
		ret = ~ret + 1;
	}
	
	return ret & filter;
}

void print_binary(int num, int len) {
	
	unsigned int mask = 1 << len - 1;

	while(len--) {
		if(num & mask) printf("1");
		else printf("0");

		mask = mask >> 1;
	}
}

void printb_fix(int num, int wl, int iwl) {
	int fwl = wl - 1 - iwl;
	int integer = num >> fwl;
	
	print_binary(integer, iwl + 1);
	printf(".");
	print_binary(num, fwl);
}

void printd_fix(unsigned int num, int wl, int iwl) {
	int fwl = wl - 1 - iwl;
	if(num >> wl - 1) {
		printf("-");
		num = ~(num - 1);
	}
	
	int integer = num >> fwl;

	float fraction = 0.0;
	float div = 0.5;
	unsigned int mask = 1 << fwl - 1;
	for(int i = 0; i < fwl; i++) {
		if(mask & num) {
			fraction += div;
		}
		mask = mask >> 1;
		div /= 2;
	}
	printf("%f", integer + fraction);

}

int mul_fix(int a, int b, int wl, int iwl) {
	int c = a * b;
	return c >> wl - 1 - iwl;
}

int div_fix(int a, int b, int wl, int iwl) {
	int c = a << wl - 1 - iwl;
	return c / b;
}

