
#include <immintrin.h>
#include<iostream>
#include <iomanip>

/////////////////////////////////////////////////////////////////////////////////
//"This code contains one vmulps instruction(reg mem ymm). Expected output is: 0x1p+5 , 0x1.ep+3 , 0x1.4p+5 , 0x1p+6 , 0x1p+3 , 0x1.cp+3 , 0x1.ep+5 , 0x1p+0 "// 

int main(int argc, char** argv) {

alignas(32) float array1[8]={16.0,1.0,5.0, 8.0,2.0,7.0,20.0,1.0};
alignas(32) float  array2[8]={2.0,15.0,8.0,8.0,4.0,2.0,3.0,1.0};

__m256 op1= _mm256_loadu_ps(array1);
__m256 op2= _mm256_loadu_ps(array2);
__m256 result=_mm256_mul_ps(op1,op2);
alignas(32) float res[8];
_mm256_store_ps(res,result);

 for(int i=0;i<8;i++)
 {
   std::cout << std::setprecision(23) << std::hexfloat << res[i] ;
   if(i!=7)
    std::cout <<" , ";
    else
    std::cout << std::endl;
 }
   
   return 0;
}
