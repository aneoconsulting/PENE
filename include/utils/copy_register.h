#pragma once

#include <immintrin.h>
#include <pin.H>

namespace pene
{
  namespace utils
  {

    template<int N>
    static void PIN_FAST_ANALYSIS_CALL copy_register(char* dst_, char* const src_)
    {
      for (int i = 0; i < N; ++i)
      {
        dst_[i] = src_[i];
      }
    }
    template<>
    void PIN_FAST_ANALYSIS_CALL copy_register<1>(char* dst, char* const src)
    {
      dst[0] = src[0];
    }
    template<>
    void PIN_FAST_ANALYSIS_CALL copy_register<2>(char* dst_, char* const src_)
    {
      auto dst = reinterpret_cast<UINT16*>(dst_);
      auto src = reinterpret_cast<UINT16*>(src_);
      dst[0] = src[0];
    }
    template<>
    void PIN_FAST_ANALYSIS_CALL copy_register<4>(char* dst_, char* const src_)
    {
      auto dst = reinterpret_cast<UINT32*>(dst_);
      auto src = reinterpret_cast<UINT32*>(src_);
      dst[0] = src[0];
    }
    template<>
    void PIN_FAST_ANALYSIS_CALL copy_register<8>(char* dst_, char* const src_)
    {
      auto dst = reinterpret_cast<UINT64*>(dst_);
      auto src = reinterpret_cast<UINT64*>(src_);
      dst[0] = src[0];
    }
    template<>
    void PIN_FAST_ANALYSIS_CALL copy_register<16>(char* dst_, char* const src_)
    {
      auto dst = reinterpret_cast<__m128*>(dst_);
      auto src = reinterpret_cast<__m128*>(src_);
      dst[0] = src[0];
    }
    template<>
    void PIN_FAST_ANALYSIS_CALL copy_register<32>(char* dst_, char* const src_)
    {
      auto dst = reinterpret_cast<__m256*>(dst_);
      auto src = reinterpret_cast<__m256*>(src_);
      dst[0] = src[0];
    }
    template<>
    void PIN_FAST_ANALYSIS_CALL copy_register<64>(char* dst_, char* const src_)
    {
      auto dst = reinterpret_cast<__m512*>(dst_);
      auto src = reinterpret_cast<__m512*>(src_);
      dst[0] = src[0];
    }
  }
}