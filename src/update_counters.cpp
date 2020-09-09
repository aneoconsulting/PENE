#include "counters_utils.h"
#include "iostream"

namespace pene
{
  bool update_counters(INS ins, counters& counters)
  {
    auto oc = INS_Opcode(ins);
    //debug std::cout << oc << std::endl;
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

    //================================== CVT part
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
    //================================== oth part
    case XED_ICLASS_PABSB:
    case XED_ICLASS_PABSD:
    case XED_ICLASS_PABSW:
    case XED_ICLASS_PADDB:
    case XED_ICLASS_PADDD:
    case XED_ICLASS_PADDQ:
    case XED_ICLASS_PADDSB:
    case XED_ICLASS_PADDSW:
    case XED_ICLASS_PADDUSB:
    case XED_ICLASS_PADDUSW:
    case XED_ICLASS_PADDW:
    case XED_ICLASS_PSUBB:
    case XED_ICLASS_PSUBD:
    case XED_ICLASS_PSUBQ:
    case XED_ICLASS_PSUBW:
    case XED_ICLASS_PSUBSB:
    case XED_ICLASS_PSUBSW:
    case XED_ICLASS_PSUBUSB:
    case XED_ICLASS_PSUBUSW:
    case XED_ICLASS_PHADDW:
    case XED_ICLASS_PHADDD:
    case XED_ICLASS_PHADDSW:
    case XED_ICLASS_PHSUBW:
    case XED_ICLASS_PHSUBD:
    case XED_ICLASS_PHSUBSW:
    case XED_ICLASS_PMADDUBSW:
    case XED_ICLASS_PMADDWD:
    case XED_ICLASS_PMAXSB:
    case XED_ICLASS_PMAXSW:
    case XED_ICLASS_PMAXSD:
    case XED_ICLASS_PMAXUB:
    case XED_ICLASS_PMAXUW:
    case XED_ICLASS_PMAXUD:
    case XED_ICLASS_PMINSB:
    case XED_ICLASS_PMINSW:
    case XED_ICLASS_PMINSD:
    case XED_ICLASS_PMINUB:
    case XED_ICLASS_PMINUW:
    case XED_ICLASS_PMINUD:
    case XED_ICLASS_MPSADBW:
    case XED_ICLASS_PSADBW:
    case XED_ICLASS_PMULDQ:
    case XED_ICLASS_PMULHRW:
    case XED_ICLASS_PMULHRSW:
    case XED_ICLASS_PMULHUW:
    case XED_ICLASS_PMULHW:
    case XED_ICLASS_PMULLW:
    case XED_ICLASS_PMULLD:
    case XED_ICLASS_PMULUDQ:
    case XED_ICLASS_PSIGND:
    case XED_ICLASS_PSIGNB:
    case XED_ICLASS_PSIGNW:
        //counters[oth_int_simd_sse] += 1;
        return true;
    case XED_ICLASS_ROUNDPS:
    case XED_ICLASS_HADDPS:
    case XED_ICLASS_DPPS:
    case XED_ICLASS_ADDSUBPS:
    case XED_ICLASS_HSUBPS:
    case XED_ICLASS_MAXPS:
    case XED_ICLASS_MINPS:
    case XED_ICLASS_RCPPS:
    case XED_ICLASS_RSQRTPS:
    case XED_ICLASS_SQRTPS:
        counters[oth_float_simd_sse] += 1;
        return true;
    case XED_ICLASS_ROUNDPD:
    case XED_ICLASS_HADDPD:
    case XED_ICLASS_DPPD:
    case XED_ICLASS_ADDSUBPD:
    case XED_ICLASS_HSUBPD:
    case XED_ICLASS_MAXPD:
    case XED_ICLASS_MINPD:
    case XED_ICLASS_SQRTPD:
        counters[oth_double_simd_sse] += 1;
        return true;
    case XED_ICLASS_ROUNDSS:
    case XED_ICLASS_MAXSS:
    case XED_ICLASS_MINSS:
    case XED_ICLASS_RCPSS:
    case XED_ICLASS_RSQRTSS:
    case XED_ICLASS_SQRTSS:
        counters[oth_float_scalar_sse] += 1;
        return true;
    case XED_ICLASS_ROUNDSD:
    case XED_ICLASS_MAXSD:
    case XED_ICLASS_MINSD:
    case XED_ICLASS_SQRTSD:
        counters[oth_double_scalar_sse] += 1;
        return true;


    case XED_ICLASS_VPABSB:
    case XED_ICLASS_VPABSD:
    case XED_ICLASS_VPABSW:
    case XED_ICLASS_VPABSQ:
    case XED_ICLASS_VPADDB:
    case XED_ICLASS_VPADDD:
    case XED_ICLASS_VPADDQ:
    case XED_ICLASS_VPADDSB:
    case XED_ICLASS_VPADDSW:
    case XED_ICLASS_VPADDUSB:
    case XED_ICLASS_VPADDUSW:
    case XED_ICLASS_VPADDW:
    case XED_ICLASS_VPSUBB:
    case XED_ICLASS_VPSUBD:
    case XED_ICLASS_VPSUBQ:
    case XED_ICLASS_VPSUBW:
    case XED_ICLASS_VPSUBSB:
    case XED_ICLASS_VPSUBSW:
    case XED_ICLASS_VPSUBUSB:
    case XED_ICLASS_VPSUBUSW:
    case XED_ICLASS_VPHADDW:
    case XED_ICLASS_VPHADDD:
    case XED_ICLASS_VPHADDSW:
    case XED_ICLASS_VPHSUBW:
    case XED_ICLASS_VPHSUBD:
    case XED_ICLASS_VPHSUBSW:
    case XED_ICLASS_VPMADDUBSW:
    case XED_ICLASS_VPMADDWD:
    case XED_ICLASS_VPMAXSB:
    case XED_ICLASS_VPMAXSW:
    case XED_ICLASS_VPMAXSD:
    case XED_ICLASS_VPMAXUB:
    case XED_ICLASS_VPMAXUW:
    case XED_ICLASS_VPMAXUD:
    case XED_ICLASS_VPMINSB:
    case XED_ICLASS_VPMINSW:
    case XED_ICLASS_VPMINSD:
    case XED_ICLASS_VPMINUB:
    case XED_ICLASS_VPMINUW:
    case XED_ICLASS_VPMINUD:
    case XED_ICLASS_VMPSADBW:
    case XED_ICLASS_VPSADBW:
    case XED_ICLASS_VPMULDQ:
    case XED_ICLASS_VPMULHRSW:
    case XED_ICLASS_VPMULHUW:
    case XED_ICLASS_VPMULHW:
    case XED_ICLASS_VPMULLW:
    case XED_ICLASS_VPMULLD:
    case XED_ICLASS_VPMULUDQ:
    case XED_ICLASS_VPSIGND:
    case XED_ICLASS_VPSIGNB:
    case XED_ICLASS_VPSIGNW:
    case XED_ICLASS_VPAVGB:
    case XED_ICLASS_VPAVGW:
    case XED_ICLASS_VPDPBUSD:
    case XED_ICLASS_VPDPBUSDS:
    case XED_ICLASS_VPDPWSSD:
    case XED_ICLASS_VPDPWSSDS:
    case XED_ICLASS_VPMADD52HUQ:
    case XED_ICLASS_VPMADD52LUQ:
        if (INS_Category(ins) == xed_category_enum_t::XED_CATEGORY_AVX512) {
            //counters[oth_int_simd_avx512] += 1;
            return true;
        }
        else {
            //counters[oth_int_simd_avx] += 1;
            return true;
        }
    case XED_ICLASS_VROUNDPS:
    case XED_ICLASS_VHADDPS:
    case XED_ICLASS_VDPPS:
    case XED_ICLASS_VADDSUBPS:
    case XED_ICLASS_VHSUBPS:
    case XED_ICLASS_VMAXPS:
    case XED_ICLASS_VMINPS:
    case XED_ICLASS_VRCPPS:
    case XED_ICLASS_VRSQRTPS:
    case XED_ICLASS_VSQRTPS:
        if (INS_Category(ins) == xed_category_enum_t::XED_CATEGORY_AVX512) {
            counters[oth_float_simd_avx512] += 1;
            return true;
        }
        else {
            counters[oth_float_simd_avx] += 1;
            return true;
        }
    case XED_ICLASS_VROUNDPD:
    case XED_ICLASS_VHADDPD:
    case XED_ICLASS_VDPPD:
    case XED_ICLASS_VADDSUBPD:
    case XED_ICLASS_VHSUBPD:
    case XED_ICLASS_VMAXPD:
    case XED_ICLASS_VMINPD:
    case XED_ICLASS_VSQRTPD:
        if (INS_Category(ins) == xed_category_enum_t::XED_CATEGORY_AVX512) {
            counters[oth_double_simd_avx512] += 1;
            return true;
        }
        else {
            counters[oth_double_simd_avx] += 1;
            return true;
        }
    case XED_ICLASS_VROUNDSS:
    case XED_ICLASS_VMAXSS:
    case XED_ICLASS_VMINSS:
    case XED_ICLASS_VRCPSS:
    case XED_ICLASS_VRSQRTSS:
    case XED_ICLASS_VSQRTSS:
        if (INS_Category(ins) == xed_category_enum_t::XED_CATEGORY_AVX512) {
            counters[oth_float_scalar_avx512] += 1;
            return true;
        }
        else {
            counters[oth_float_scalar_avx] += 1;
            return true;
        }
    case XED_ICLASS_VROUNDSD:
    case XED_ICLASS_VMAXSD:
    case XED_ICLASS_VMINSD:
    case XED_ICLASS_VSQRTSD:
        if (INS_Category(ins) == xed_category_enum_t::XED_CATEGORY_AVX512) {
            counters[oth_double_scalar_avx512] += 1;
            return true;
        }
        else {
            counters[oth_double_scalar_avx] += 1;
            return true;
        }
    case XED_ICLASS_VP4DPWSSD:
    case XED_ICLASS_VP4DPWSSDS:
        //counters[oth_int_scalar_avx512] += 1;
        return true;
    case XED_ICLASS_V4FMADDSS:
    case XED_ICLASS_V4FNMADDSS:
    case XED_ICLASS_VRCP14SS:
    case XED_ICLASS_VRCP28SS:
    case XED_ICLASS_VRSQRT14SS:
    case XED_ICLASS_VRSQRT28SS:
        counters[oth_float_scalar_avx512] += 1;
        return true;
    case XED_ICLASS_V4FMADDPS:
    case XED_ICLASS_V4FNMADDPS:
    case XED_ICLASS_VPANDD:
    case XED_ICLASS_VEXP2PS:
    case XED_ICLASS_VRCP14PS:
    case XED_ICLASS_VRCP28PS:
    case XED_ICLASS_VRSQRT14PS:
    case XED_ICLASS_VRSQRT28PS:
        counters[oth_float_simd_avx512] += 1;
        return true;
    case XED_ICLASS_VRCP14SD:
    case XED_ICLASS_VRCP28SD:
    case XED_ICLASS_VRSQRT14SD:
    case XED_ICLASS_VRSQRT28SD:
        counters[oth_double_scalar_avx512] += 1;
        return true;
    case XED_ICLASS_VPANDQ:
    case XED_ICLASS_VEXP2PD:
    case XED_ICLASS_VRCP14PD:
    case XED_ICLASS_VRCP28PD:
    case XED_ICLASS_VRSQRT14PD:
    case XED_ICLASS_VRSQRT28PD:
        counters[oth_double_simd_avx512] += 1;
        return true;

    //================================== CMP part
    case XED_ICLASS_PCMPEQW:
    case XED_ICLASS_PCMPEQD:
    case XED_ICLASS_PCMPEQQ:
    case XED_ICLASS_PCMPEQB:
    case XED_ICLASS_PCMPGTW:
    case XED_ICLASS_PCMPGTD:
    case XED_ICLASS_PCMPGTQ:
    case XED_ICLASS_PCMPGTB:
        //counters[cmp_int_simd_sse] += 1;
        return true;
    case XED_ICLASS_CMPSS:
    case XED_ICLASS_COMISS:
    case XED_ICLASS_UCOMISS:
        counters[cmp_float_scalar_sse] += 1;
        return true;
    case XED_ICLASS_CMPPS:
        counters[cmp_float_simd_sse] += 1;
        return true;
    case XED_ICLASS_CMPSD:
    case XED_ICLASS_CMPSD_XMM:
    case XED_ICLASS_COMISD:
    case XED_ICLASS_UCOMISD:
        counters[cmp_double_scalar_sse] += 1;
        return true;
    case XED_ICLASS_CMPPD:
        counters[cmp_double_simd_sse] += 1;
        return true;

    case XED_ICLASS_VPCMPEQW:
    case XED_ICLASS_VPCMPEQD:
    case XED_ICLASS_VPCMPEQQ:
    case XED_ICLASS_VPCMPEQB:
    case XED_ICLASS_VPCMPGTW:
    case XED_ICLASS_VPCMPGTD:
    case XED_ICLASS_VPCMPGTQ:
    case XED_ICLASS_VPCMPGTB:
        if (INS_Category(ins) == xed_category_enum_t::XED_CATEGORY_AVX512) {
            //counters[cmp_int_simd_avx512] += 1;
            return true;
        }
        else {
            //counters[cmp_int_simd_avx] += 1;
            return true;
        }
    case XED_ICLASS_VPCMPW:
    case XED_ICLASS_VPCMPD:
    case XED_ICLASS_VPCMPQ:
    case XED_ICLASS_VPCMPB:
    case XED_ICLASS_VPCMPUW:
    case XED_ICLASS_VPCMPUD:
    case XED_ICLASS_VPCMPUQ:
    case XED_ICLASS_VPCMPUB:
    case XED_ICLASS_VPCONFLICTD:
    case XED_ICLASS_VPCONFLICTQ:
    case XED_ICLASS_VPTESTMB:
    case XED_ICLASS_VPTESTMD:
    case XED_ICLASS_VPTESTMQ:
    case XED_ICLASS_VPTESTMW:
    case XED_ICLASS_VPTESTNMB:
    case XED_ICLASS_VPTESTNMD:
    case XED_ICLASS_VPTESTNMQ:
    case XED_ICLASS_VPTESTNMW:
        //counters[cmp_int_simd_avx512] += 1;
        return true;
    case XED_ICLASS_VCMPSS:
        if (INS_Category(ins) == xed_category_enum_t::XED_CATEGORY_AVX512) {
            counters[cmp_float_scalar_avx512] += 1;
            return true;
        }
        else {
            counters[cmp_float_scalar_avx] += 1;
            return true;
        }
    case XED_ICLASS_VCMPPS:
        if (INS_Category(ins) == xed_category_enum_t::XED_CATEGORY_AVX512) {
            counters[cmp_float_simd_avx512] += 1;
            return true;
        }
        else {
            counters[cmp_float_simd_avx] += 1;
            return true;
        }
    case XED_ICLASS_VCMPSD:
    case XED_ICLASS_VCOMISD:
    case XED_ICLASS_VUCOMISD:
        if (INS_Category(ins) == xed_category_enum_t::XED_CATEGORY_AVX512) {
            counters[cmp_double_scalar_avx512] += 1;
            return true;
        }
        else {
            counters[cmp_double_scalar_avx] += 1;
            return true;
        }
    case XED_ICLASS_VCMPPD:
        if (INS_Category(ins) == xed_category_enum_t::XED_CATEGORY_AVX512) {
            counters[cmp_double_simd_avx512] += 1;
            return true;
        }
        else {
            counters[cmp_double_simd_avx] += 1;
            return true;
        }


    default:
      return false;
    }
  }



}