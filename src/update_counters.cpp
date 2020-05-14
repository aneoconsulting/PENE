#include "counters_utils.h"

namespace pene
{
  bool update_counters(INS ins, counters& counters)
  {
    auto oc = INS_Opcode(ins);
    switch (oc)
    {
    case XED_ICLASS_ADDSD:
    case XED_ICLASS_SUBSD:
        counters[add_double_scalar_sse] += 1;
        return true;
    case XED_ICLASS_VADDSD:
    case XED_ICLASS_VSUBSD:
        if(INS_Category(ins) == xed_category_enum_t::XED_CATEGORY_AVX512){
            counters[add_double_scalar_avx512] += 1;
            return true;
        }else{
            counters[add_double_scalar_avx] += 1;
            return true;
        }
    case XED_ICLASS_ADDPD:
    case XED_ICLASS_SUBPD:
        counters[add_double_simd_sse] += 1;
        return true;
    case XED_ICLASS_VADDPD:
    case XED_ICLASS_VSUBPD:
        if (INS_Category(ins) == xed_category_enum_t::XED_CATEGORY_AVX512) {
            counters[add_double_simd_avx512] += 1;
            return true;
        }
        else {
            counters[add_double_simd_avx] += 1;
            return true;
        }
    case XED_ICLASS_ADDSS:
    case XED_ICLASS_SUBSS:
        counters[add_float_scalar_sse] += 1;
        return true;
    case XED_ICLASS_VADDSS:
    case XED_ICLASS_VSUBSS:
        if (INS_Category(ins) == xed_category_enum_t::XED_CATEGORY_AVX512) {
            counters[add_float_scalar_avx512] += 1;
            return true;
        }
        else {
            counters[add_float_scalar_avx] += 1;
            return true;
        }
    case XED_ICLASS_ADDPS:
    case XED_ICLASS_SUBPS:
        counters[add_float_simd_sse] += 1;
        return true;
    case XED_ICLASS_VADDPS:
    case XED_ICLASS_VSUBPS:
        if (INS_Category(ins) == xed_category_enum_t::XED_CATEGORY_AVX512) {
            counters[add_float_simd_avx512] += 1;
            return true;
        }
        else {
            counters[add_float_simd_avx] += 1;
            return true;
        }
    case XED_ICLASS_DIVSD:
        counters[div_double_scalar_sse] += 1;
        return true;
    case XED_ICLASS_VDIVSD:
        if (INS_Category(ins) == xed_category_enum_t::XED_CATEGORY_AVX512) {
            counters[div_double_scalar_avx512] += 1;
            return true;
        }
        else {
            counters[div_double_scalar_avx] += 1;
            return true;
        }
    case XED_ICLASS_DIVPD:
        counters[div_double_simd_sse] += 1;
        return true;
    case XED_ICLASS_VDIVPD:
        if (INS_Category(ins) == xed_category_enum_t::XED_CATEGORY_AVX512) {
            counters[div_double_simd_avx512] += 1;
            return true;
        }
        else {
            counters[div_double_simd_avx] += 1;
            return true;
        }
    case XED_ICLASS_DIVSS:
        counters[div_float_scalar_sse] += 1;
        return true;
    case XED_ICLASS_VDIVSS:
        if (INS_Category(ins) == xed_category_enum_t::XED_CATEGORY_AVX512) {
            counters[div_float_scalar_avx512] += 1;
            return true;
        }
        else {
            counters[div_float_scalar_avx] += 1;
            return true;
        }
    case XED_ICLASS_DIVPS:
        counters[div_float_simd_sse] += 1;
        return true;
    case XED_ICLASS_VDIVPS:
        if (INS_Category(ins) == xed_category_enum_t::XED_CATEGORY_AVX512) {
            counters[div_float_simd_avx512] += 1;
            return true;
        }
        else {
            counters[div_float_simd_avx] += 1;
            return true;
        }
    case XED_ICLASS_MULSS:
        counters[mul_float_scalar_sse] += 1;
        return true;
    case XED_ICLASS_VMULSS:
        if (INS_Category(ins) == xed_category_enum_t::XED_CATEGORY_AVX512) {
            counters[mul_float_scalar_avx512] += 1;
            return true;
        }
        else {
            counters[mul_float_scalar_avx] += 1;
            return true;
        }
    case XED_ICLASS_MULPS:
        counters[mul_float_simd_sse] += 1;
        return true;
    case XED_ICLASS_VMULPS:
        if (INS_Category(ins) == xed_category_enum_t::XED_CATEGORY_AVX512) {
            counters[mul_float_simd_avx512] += 1;
            return true;
        }
        else {
            counters[mul_float_simd_avx] += 1;
            return true;
        }
    case XED_ICLASS_MULSD:
        counters[mul_double_scalar_sse] += 1;
        return true;
    case XED_ICLASS_VMULSD:
        if (INS_Category(ins) == xed_category_enum_t::XED_CATEGORY_AVX512) {
            counters[mul_double_scalar_avx512] += 1;
            return true;
        }
        else {
            counters[div_double_scalar_avx] += 1;
            return true;
        }
    case XED_ICLASS_MULPD:
        counters[mul_double_simd_sse] += 1;
        return true;
    case XED_ICLASS_VMULPD:
        if (INS_Category(ins) == xed_category_enum_t::XED_CATEGORY_AVX512) {
            counters[mul_double_simd_avx512] += 1;
            return true;
        }
        else {
            counters[mul_double_simd_avx] += 1;
            return true;
        }
    case XED_ICLASS_VFMADD132SS:
    case XED_ICLASS_VFMADD213SS:
    case XED_ICLASS_VFMADD231SS:
    case XED_ICLASS_VFMADDSS:
    case XED_ICLASS_VFMSUB132SS:
    case XED_ICLASS_VFMSUB213SS:
    case XED_ICLASS_VFMSUB231SS:
    case XED_ICLASS_VFMSUBSS:
    case XED_ICLASS_VFNMADD132SS:
    case XED_ICLASS_VFNMADD213SS:
    case XED_ICLASS_VFNMADD231SS:
    case XED_ICLASS_VFNMADDSS:
    case XED_ICLASS_VFNMSUB132SS:
    case XED_ICLASS_VFNMSUB213SS:
    case XED_ICLASS_VFNMSUB231SS:
    case XED_ICLASS_VFNMSUBSS:
        if (INS_Category(ins) == xed_category_enum_t::XED_CATEGORY_AVX512) {
            counters[fma_float_scalar_avx512] += 1;
            return true;
        }
        else {
            counters[fma_float_scalar_avx] += 1;
            return true;
        }
    case XED_ICLASS_VFMADD132SD:
    case XED_ICLASS_VFMADD213SD:
    case XED_ICLASS_VFMADD231SD:
    case XED_ICLASS_VFMADDSD:
    case XED_ICLASS_VFMSUB132SD:
    case XED_ICLASS_VFMSUB213SD:
    case XED_ICLASS_VFMSUB231SD:
    case XED_ICLASS_VFMSUBSD:
    case XED_ICLASS_VFNMADD132SD:
    case XED_ICLASS_VFNMADD213SD:
    case XED_ICLASS_VFNMADD231SD:
    case XED_ICLASS_VFNMADDSD:
    case XED_ICLASS_VFNMSUB132SD:
    case XED_ICLASS_VFNMSUB213SD:
    case XED_ICLASS_VFNMSUB231SD:
    case XED_ICLASS_VFNMSUBSD:
        if (INS_Category(ins) == xed_category_enum_t::XED_CATEGORY_AVX512) {
            counters[fma_double_scalar_avx512] += 1;
            return true;
        }
        else {
            counters[fma_double_scalar_avx] += 1;
            return true;
        }
    case XED_ICLASS_VFMADD132PS:
    case XED_ICLASS_VFMADD213PS:
    case XED_ICLASS_VFMADD231PS:
    case XED_ICLASS_VFMADDPS:
    case XED_ICLASS_VFMADDSUB132PS:
    case XED_ICLASS_VFMADDSUB213PS:
    case XED_ICLASS_VFMADDSUB231PS:
    case XED_ICLASS_VFMADDSUBPS:
    case XED_ICLASS_VFMSUB132PS:
    case XED_ICLASS_VFMSUB213PS:
    case XED_ICLASS_VFMSUB231PS:
    case XED_ICLASS_VFMSUBPS:
    case XED_ICLASS_VFMSUBADD132PS:
    case XED_ICLASS_VFMSUBADD213PS:
    case XED_ICLASS_VFMSUBADD231PS:
    case XED_ICLASS_VFMSUBADDPS:
    case XED_ICLASS_VFNMADD132PS:
    case XED_ICLASS_VFNMADD213PS:
    case XED_ICLASS_VFNMADD231PS:
    case XED_ICLASS_VFNMADDPS:
    case XED_ICLASS_VFNMSUB132PS:
    case XED_ICLASS_VFNMSUB213PS:
    case XED_ICLASS_VFNMSUB231PS:
    case XED_ICLASS_VFNMSUBPS:
        if (INS_Category(ins) == xed_category_enum_t::XED_CATEGORY_AVX512) {
            counters[fma_float_simd_avx512] += 1;
            return true;
        }
        else {
            counters[fma_float_simd_avx] += 1;
            return true;
        }
    case XED_ICLASS_VFMADD132PD:
    case XED_ICLASS_VFMADD213PD:
    case XED_ICLASS_VFMADD231PD:
    case XED_ICLASS_VFMADDPD:
    case XED_ICLASS_VFMADDSUB132PD:
    case XED_ICLASS_VFMADDSUB213PD:
    case XED_ICLASS_VFMADDSUB231PD:
    case XED_ICLASS_VFMADDSUBPD:
    case XED_ICLASS_VFMSUB132PD:
    case XED_ICLASS_VFMSUB213PD:
    case XED_ICLASS_VFMSUB231PD:
    case XED_ICLASS_VFMSUBPD:
    case XED_ICLASS_VFMSUBADD132PD:
    case XED_ICLASS_VFMSUBADD213PD:
    case XED_ICLASS_VFMSUBADD231PD:
    case XED_ICLASS_VFMSUBADDPD:
    case XED_ICLASS_VFNMADD132PD:
    case XED_ICLASS_VFNMADD213PD:
    case XED_ICLASS_VFNMADD231PD:
    case XED_ICLASS_VFNMADDPD:
    case XED_ICLASS_VFNMSUB132PD:
    case XED_ICLASS_VFNMSUB213PD:
    case XED_ICLASS_VFNMSUB231PD:
    case XED_ICLASS_VFNMSUBPD:
        if (INS_Category(ins) == xed_category_enum_t::XED_CATEGORY_AVX512) {
            counters[fma_double_simd_avx512] += 1;
            return true;
        }
        else {
            counters[fma_double_simd_avx] += 1;
            return true;
        }
    case XED_ICLASS_CVTDQ2PS:
    case XED_ICLASS_CVTPI2PS:
        counters[cvt_i2f_simd_sse] += 1;
        return true;
    case XED_ICLASS_CVTSI2SS:
        counters[cvt_i2f_scalar_sse] += 1;
        return true;
    case XED_ICLASS_CVTDQ2PD:
    case XED_ICLASS_CVTPI2PD:
        counters[cvt_i2d_simd_sse] += 1;
        return true;
    case XED_ICLASS_CVTSI2SD:
        counters[cvt_i2d_scalar_sse] += 1;
        return true;
    case XED_ICLASS_CVTPS2DQ:
    case XED_ICLASS_CVTTPS2DQ:
    case XED_ICLASS_CVTPS2PI:
    case XED_ICLASS_CVTTPS2PI:
        counters[cvt_f2i_simd_sse] += 1;
        return true;
    case XED_ICLASS_CVTSS2SI:
    case XED_ICLASS_CVTTSS2SI:
        counters[cvt_f2i_scalar_sse] += 1;
        return true;
    case XED_ICLASS_CVTPS2PD:
        counters[cvt_f2d_simd_sse] += 1;
        return true;
    case XED_ICLASS_CVTSS2SD:
        counters[cvt_f2d_scalar_sse] += 1;
        return true;
    case XED_ICLASS_CVTPD2DQ:
    case XED_ICLASS_CVTTPD2DQ:
    case XED_ICLASS_CVTPD2PI:
    case XED_ICLASS_CVTTPD2PI:
        counters[cvt_d2i_simd_sse] += 1;
        return true;
    case XED_ICLASS_CVTSD2SI:
    case XED_ICLASS_CVTTSD2SI:
        counters[cvt_d2i_scalar_sse] += 1;
        return true;
    case XED_ICLASS_CVTPD2PS:
        counters[cvt_d2f_simd_sse] += 1;
        return true;
    case XED_ICLASS_CVTSD2SS:
        counters[cvt_d2f_scalar_sse] += 1;
        return true;
    case XED_ICLASS_VCVTDQ2PD:
    case XED_ICLASS_VCVTQQ2PD:
    case XED_ICLASS_VCVTUQQ2PD:
    case XED_ICLASS_VCVTUDQ2PD:
        if (INS_Category(ins) == xed_category_enum_t::XED_CATEGORY_AVX512) {
            counters[cvt_i2d_simd_avx512] += 1;
            return true;
        }
        else {
            counters[cvt_i2d_simd_avx] += 1;
            return true;
        }
    case XED_ICLASS_VCVTDQ2PS:
    case XED_ICLASS_VCVTQQ2PS:
    case XED_ICLASS_VCVTUDQ2PS:
    case XED_ICLASS_VCVTUQQ2PS:
        if (INS_Category(ins) == xed_category_enum_t::XED_CATEGORY_AVX512) {
            counters[cvt_i2f_simd_avx512] += 1;
            return true;
        }
        else {
            counters[cvt_i2f_simd_avx] += 1;
            return true;
        }
    case XED_ICLASS_VCVTUSI2SD:
    case XED_ICLASS_VCVTSI2SD:
        if (INS_Category(ins) == xed_category_enum_t::XED_CATEGORY_AVX512) {
            counters[cvt_i2d_scalar_avx512] += 1;
            return true;
        }
        else {
            counters[cvt_i2d_scalar_avx] += 1;
            return true;
        }
    case XED_ICLASS_VCVTUSI2SS:
    case XED_ICLASS_VCVTSI2SS:
        if (INS_Category(ins) == xed_category_enum_t::XED_CATEGORY_AVX512) {
            counters[cvt_i2f_scalar_avx512] += 1;
            return true;
        }
        else {
            counters[cvt_i2f_scalar_avx] += 1;
            return true;
        }
    case XED_ICLASS_VCVTTPD2DQ:
    case XED_ICLASS_VCVTTPD2QQ:
    case XED_ICLASS_VCVTTPD2UDQ:
    case XED_ICLASS_VCVTTPD2UQQ:
    case XED_ICLASS_VCVTPD2DQ:
    case XED_ICLASS_VCVTPD2QQ:
    case XED_ICLASS_VCVTPD2UDQ:
    case XED_ICLASS_VCVTPD2UQQ:
        if (INS_Category(ins) == xed_category_enum_t::XED_CATEGORY_AVX512) {
            counters[cvt_d2i_simd_avx512] += 1;
            return true;
        }
        else {
            counters[cvt_d2i_simd_avx] += 1;
            return true;
        }
    case XED_ICLASS_VCVTPD2PS:
        if (INS_Category(ins) == xed_category_enum_t::XED_CATEGORY_AVX512) {
            counters[cvt_d2f_simd_avx512] += 1;
            return true;
        }
        else {
            counters[cvt_d2f_simd_avx] += 1;
            return true;
        }
    case XED_ICLASS_VCVTSD2USI:
    case XED_ICLASS_VCVTSD2SI:
    case XED_ICLASS_VCVTTSD2SI:
    case XED_ICLASS_VCVTTSD2USI:
        if (INS_Category(ins) == xed_category_enum_t::XED_CATEGORY_AVX512) {
            counters[cvt_d2i_scalar_avx512] += 1;
            return true;
        }
        else {
            counters[cvt_d2i_scalar_avx] += 1;
            return true;
        }
    case XED_ICLASS_VCVTSD2SS:
        if (INS_Category(ins) == xed_category_enum_t::XED_CATEGORY_AVX512) {
            counters[cvt_d2f_scalar_avx512] += 1;
            return true;
        }
        else {
            counters[cvt_d2f_scalar_avx] += 1;
            return true;
        }
    case XED_ICLASS_VCVTTPS2DQ:
    case XED_ICLASS_VCVTTPS2QQ:
    case XED_ICLASS_VCVTTPS2UDQ:
    case XED_ICLASS_VCVTTPS2UQQ:
    case XED_ICLASS_VCVTPS2DQ:
    case XED_ICLASS_VCVTPS2QQ:
    case XED_ICLASS_VCVTPS2UDQ:
    case XED_ICLASS_VCVTPS2UQQ:
        if (INS_Category(ins) == xed_category_enum_t::XED_CATEGORY_AVX512) {
            counters[cvt_f2i_simd_avx512] += 1;
            return true;
        }
        else {
            counters[cvt_f2i_simd_avx] += 1;
            return true;
        }
    case XED_ICLASS_VCVTPS2PD:
        if (INS_Category(ins) == xed_category_enum_t::XED_CATEGORY_AVX512) {
            counters[cvt_f2d_simd_avx512] += 1;
            return true;
        }
        else {
            counters[cvt_f2d_simd_avx] += 1;
            return true;
        }
    case XED_ICLASS_VCVTSS2USI:
    case XED_ICLASS_VCVTSS2SI:
    case XED_ICLASS_VCVTTSS2USI:
    case XED_ICLASS_VCVTTSS2SI:
        if (INS_Category(ins) == xed_category_enum_t::XED_CATEGORY_AVX512) {
            counters[cvt_f2i_scalar_avx512] += 1;
            return true;
        }
        else {
            counters[cvt_f2i_scalar_avx] += 1;
            return true;
        }
    case XED_ICLASS_VCVTSS2SD:
        if (INS_Category(ins) == xed_category_enum_t::XED_CATEGORY_AVX512) {
            counters[cvt_f2d_scalar_avx512] += 1;
            return true;
        }
        else {
            counters[cvt_f2d_scalar_avx] += 1;
            return true;
        }
    case XED_ICLASS_VCVTPH2PS:
        if (INS_Category(ins) == xed_category_enum_t::XED_CATEGORY_AVX512) {
            counters[cvt_h2f_simd_avx512] += 1;
            return true;
        }
        else {
            counters[cvt_h2f_simd_avx] += 1;
            return true;
        }
    case XED_ICLASS_VCVTPS2PH:
        if (INS_Category(ins) == xed_category_enum_t::XED_CATEGORY_AVX512) {
            counters[cvt_f2h_simd_avx512] += 1;
            return true;
        }
        else {
            counters[cvt_f2h_simd_avx] += 1;
            return true;
        }

    default:
      return false;
    }
  }



}