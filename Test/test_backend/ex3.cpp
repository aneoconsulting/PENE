
#include <immintrin.h>
#include<iostream>
#include <iomanip>

int main(int argc, char** argv) {

//"This code contains one vmulps instruction(reg mem ymm). Expected output is"
alignas(32) float array1[8]={16.0,1.0,5.0, 8.0,2.0,7.0,20.0,1.0};
alignas(32) float  array2[8]={2.0,15.0,8.0,8.0,4.0,2.0,3.0,1.0};

__m256 op1= _mm256_loadu_ps(array1);
__m256 op2= _mm256_loadu_ps(array2);
__m256 result=_mm256_mul_ps(op1,op2);


 for(int i=0;i<8;i++)
 {
   std::cout << std::setprecision(23) << std::hexfloat << result[i] ;
   if(i!=7)
    std::cout <<" , ";
    else
    std::cout << std::endl;
 }
   
   return 0;
}
