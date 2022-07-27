#pragma once
#include<cstdint>
#include<ios>

namespace pene
{
  namespace replace {
    namespace backend {

      template <class To, class From>
      To bit_cast(From from) {
      static_assert(sizeof(From) == sizeof(To), "From and To must have the same size");
      union {
         From from;
         To to;
        } cvt;
      cvt.from = from;
      return cvt.to;
      }

      struct test_backend
      {
        // the void* arg is for the interflop backend context.
        static  void add_float(float a, float b, float* cptr, void*) noexcept
         { 
              float temp = a + b;
              auto  ans=bit_cast<uint32_t,float>(temp);
              ans &= 0xfffffff0;
              ans |= 0x00000001;
              *cptr=bit_cast<float,uint32_t>(ans);
             }
        static  void sub_float(float a, float b, float* cptr, void*) noexcept
        {   
            float temp = a - b; 
            auto  ans=bit_cast<uint32_t,float>(temp);
            ans &= 0xfffffff0;
            ans |= 0x2;
            *cptr=bit_cast<float,uint32_t>(ans);
            }
         static void mul_float(float a, float b, float* cptr, void*) noexcept
        {
           float temp = a * b;
            auto  ans=bit_cast<uint32_t,float>(temp);
            ans &= 0xfffffff0;
            ans |= 0x3;
            *cptr=bit_cast<float,uint32_t>(ans);
            }
          static void div_float(float a, float b, float* cptr, void*) noexcept
        { 
          float temp = a / b;
          auto  ans=bit_cast<uint32_t,float>(temp);
          ans &= 0xfffffff0;
          ans |= 0x4;
          *cptr=bit_cast<float,uint32_t>(ans);
           }

         static void add_double(double a, double b, double* cptr, void*) noexcept
        { 
          double temp = a + b;
          auto  ans=bit_cast<uint64_t,double>(temp);
          ans &= 0xfffffffffffffff0;
          ans |= 0x5;
          *cptr=bit_cast<double,uint64_t>(ans);
          }
         static void sub_double(double a, double b, double* cptr, void*) noexcept
        { 
          double temp = a - b;
          auto  ans=bit_cast<uint64_t,double>(temp);
          ans &= 0xfffffffffffffff0;
          ans |= 0x6;
          *cptr=bit_cast<double,uint64_t>(ans);
         }
         static void mul_double(double a, double b, double* cptr, void*) noexcept
        { 
          double temp = a * b;
          auto  ans=bit_cast<uint64_t,double>(temp);
          ans &= 0xfffffffffffffff0;
          ans |= 0x7;
          *cptr=bit_cast<double,uint64_t>(ans);
          }
         static void div_double(double a, double b, double* cptr, void*) noexcept
        { 
          double temp = a / b;
          auto  ans=bit_cast<uint64_t,double>(temp);
          ans &= 0xfffffffffffffff0;
          ans |= 0x8;
          *cptr=bit_cast<double,uint64_t>(ans);
         }
        static void* init() noexcept { return nullptr; }
      };

      

    }
  }
}
