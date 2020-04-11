#include <iostream>
#include <cstdio>
#include <cstring>
#include <cmath>

static constexpr double pi_const = 3.14159265358979323846;

#include "../../Addcount/version.h"

double sin_integral(double start, double stop, double precision) {
	//suggest for start = 0; stop = pi/2; precision = 0,1; 
	//through stop - start > precision, precision need to be small
	double sum = 0.0;
	for (double b = start; b < stop;b+=precision) {
		const double sinb = std::sin(b);
		const double temp = precision * sinb;
		const double newsum = sum + temp;
		sum = newsum;
	}
	return sum;
}


int main()
{
	const double stop = pi_const / 2;
	const double start = 0.0;
	const int i = 8;
	const double precision = std::pow(10, -i);
	const double result = sin_integral(start, stop, precision);
	std::printf("10^%d : %.16lf\n", -i, result);
	std::cout << "Hello world! ";
	std::cout << pene_version << std::endl;
}