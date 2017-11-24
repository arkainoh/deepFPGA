#include "fixedpoint.h"

int float2fix(float f, int wl, int iwl) {
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

float fix2float(int f, int wl, int iwl) {
	int fwl = wl - 1 - iwl;
	int div = 1 << fwl;
	int sign_filter = 1 << wl - 1;
	bool neg = false;
	int filter = (1 << wl) - 1;

	if(f & sign_filter) {
		f = ~f + 1;
		neg = true;
	}
	
	float ret = ((float) (f & filter)) / div;
	
	if(neg) ret *= (-1);
	
	return ret;
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

void printd_fix(int num, int wl, int iwl) {
	int fwl = wl - 1 - iwl;
	int sign_filter = 1 << wl - 1;
	int int_filter = (1 << iwl) - 1;

	if(num & sign_filter) {
		printf("-");
		num = ~(num - 1);
	}

	int integer = (num >> fwl) & int_filter;
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

int optiwl(float floats[], int len) {
	int max, iwl = 0;

	for(int i = 0; i < len; i++) {
		int n = floats[i];

		if(n < 0) n *= (-1);
		if(max < n) max = n;
	}

	if(max) iwl = log2(max) + 1;

	return iwl;
}

