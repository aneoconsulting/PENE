#include <immintrin.h>
#include <iostream>
#include <iomanip>

/////////////////////////////////////////////////////////////////////////////////
// This code contains one addss instruction(reg mem), expected output is: 0x1p+0 //

int main(int argc, char** argv)
{   
    float x=1.0;
    float y=0.0;
    __m128 m1=_mm_load_ss(&x);
    __m128 m2=_mm_load_ss(&y);
    __m128 res1 = _mm_add_ss(m1,m2);
    float a=_mm_cvtss_f32(res1);
    std::cout << std::setprecision(23) << std::hexfloat <<  a<< std::endl;
}
