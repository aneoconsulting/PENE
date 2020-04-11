#include <iostream>
#include <stdio.h>
#include <string.h>
#include <cmath>
static constexpr double pi_const = 3.14159265358979323846;

#include "../../Addcount/version.h"

double sin_integral(double start, double stop, double precision) {
	//suggest for start = 0; stop = pi/2; precision = 0,1; 
	//through stop - start > precision, precision need to be small
	double sum = 0.0;
	for (double b = start; b < stop;b+=precision) {
		double sinb = sin(b);
		double temp = precision * sinb;
		double newsum = sum + temp;
		sum = newsum;
	}
	return sum;
}


int main()
{
	double stop = pi_const / 2;
	double start = 0.0;
	int i = 8;
	double precision = std::pow((double)10, -i);
	double result = sin_integral(start, stop, precision);
	printf("10^%d : %.16lf\n", -i, result);
	/*for(i=2;i<=11;i++){
		double result = sin_integral(start, stop, precision);
		printf("10^%d : %.16lf\n", -i, result);
		precision = pow(10);
	}*/
	std::cout << "Hello world! ";
	std::cout << pene_version << std::endl;
	//printf("%s \n", pene_version);
}