#include <immintrin.h>
#include <iostream>
#include <iomanip>

/////////////////////////////////////////////////////////////////////////////////
/* This code contains one subps instruction, expected output is:
    0x1p+2
    0x1.8p+1
    0x1p+1
    0x1p+0  " */ 

int main(int argc, char** argv)
{   

    float* array1=new float[4];
    array1[0]=5.0;
    array1[1]=4.0;
    array1[2] =3.0;
    array1[3]=2.0;

    __m128 op1= _mm_load_ps(array1);
    float* array2=new float[4];
    array2[0]=1.0;
    array2[1]=1.0;
    array2[2] =1.0;
    array2[3]=1.0;
    __m128 op2= _mm_load_ps(array2);
    __m128 res= _mm_sub_ps(op1,op2);
    float result[4];
    _mm_store_ps(result,res);
    for(int i=0;i<4;i++)
      std::cout << std::setprecision(23) << std::hexfloat << result[i]<< std::endl;

}
