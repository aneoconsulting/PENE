#include <iostream>
#include <immintrin.h>


// Union used to commut between FP-Representation and Int representation of bitfield
typedef union
{
  double lf;
  long   ld;
} __mem64;

double call_addsd (double a, double b)
{
  __m128d _a = _mm_load_sd (&a);
  __m128d _b = _mm_load_sd (&b);

  __m128d _res = _mm_add_sd ( _a, _b);

  double res = 0.0f;
  _mm_store_sd ( &res, _res);

  return res;
}


 /////////////////////////////////////////////////////////////////////////////////////////////////////
 // This code contains one addsd instruction(reg mem), expected output is 4599075939470750515       //
 //   * The output corresponds to 0.1f + 0.2f = 0.3f                                                //
 //   * The result is interpreted as an integer to notice the perturbation on binary representation //

int main (void)
{
  double a = 0.1, b = 0.2;
  double c = call_addsd(a, b);

  __mem64 m ;
  m.lf =c;
  
  std::cout << m.ld << std::endl;
  
  return 0;
}
