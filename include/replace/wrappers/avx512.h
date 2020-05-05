#pragma once
#include <pin.h>
#include <xed-decoded-inst-api.h>



namespace pene
{
  namespace replace
  {
    namespace wrappers
    {
      namespace avx512
      {
        template <int N, bool uint8 = N <= 8, bool uint16 = N <= 16, bool uint32 = N <= 32>
        struct get_mask
        {
          using type = UINT64;
        };

        template<int N> struct get_mask<N, true, true, true>
        {
          using type = UINT8;
        };

        template<int N> struct get_mask<N, false, true, true>
        {
          using type = UINT16;
        };

        template<int N> struct get_mask<N, false, false, true>
        {
          using type = UINT32;
        };

        template<int size>
        static void DoLoad(ADDRINT* dst, ADDRINT* addr)
        {
          PIN_SafeCopy(dst, addr, size);
        }

        template<int NB_ELTS_IN_REG, typename T, void (*op)(T, T, T*, void*)>
        struct wrap {
          using mask_t = typename get_mask<NB_ELTS_IN_REG>::type;

          template <int I>
          static bool check_mask(mask_t* const mask)
          {
            return ((*mask) & (1 << I)) != 0;
          }

          template <bool broadcast, int I>
          static void compute(T* const a, T* const b, T* c, void* ctx)
          {
            if constexpr (broadcast)
            {
              op(a[I], b[0], &c[I], ctx);
            }
            else
            {
              op(a[I], b[I], &c[I], ctx);
            }
          }

          template<int N = NB_ELTS_IN_REG, int I = 0>
          static void apply(INS ins, bool is_broadcast, bool is_memory, REG tmp_reg)
          {
            if constexpr (I < N)
            {
              INS ins2 = INS_Next(ins);


              if (is_memory)
              {
                if constexpr (I == 0)
                {
                  INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)DoLoad<64>,
                    IARG_REG_REFERENCE, tmp_reg,
                    IARG_MEMORYREAD_EA,
                    IARG_END);
                }

                AFUNPTR func = is_broadcast ? (AFUNPTR)compute<true, I> : (AFUNPTR)compute<false, I>;
                INS_InsertIfCall(ins2, IPOINT_BEFORE, (AFUNPTR)check_mask<I>,
                  IARG_REG_CONST_REFERENCE, INS_OperandReg(ins, 1),
                  IARG_END
                );

                INS_InsertThenCall(ins2, IPOINT_BEFORE, (AFUNPTR)func,
                  IARG_REG_CONST_REFERENCE, INS_OperandReg(ins, 2),
                  IARG_REG_CONST_REFERENCE, tmp_reg,
                  IARG_REG_REFERENCE, INS_OperandReg(ins, 0),
                  IARG_PTR, nullptr,
                  IARG_END);
              }
              else
              {

                INS_InsertIfCall(ins2, IPOINT_BEFORE, (AFUNPTR)check_mask<I>,
                  IARG_REG_CONST_REFERENCE, INS_OperandReg(ins, 1),
                  IARG_END
                );
                INS_InsertThenCall(ins2, IPOINT_BEFORE, (AFUNPTR)compute<false, I>,
                  IARG_REG_CONST_REFERENCE, INS_OperandReg(ins, 2),
                  IARG_REG_CONST_REFERENCE, INS_OperandReg(ins, 3),
                  IARG_REG_REFERENCE, INS_OperandReg(ins, 0),
                  IARG_PTR, nullptr,
                  IARG_END);
              }
              apply<N, I + 1>(ins, is_broadcast, is_memory, tmp_reg);
            }
          }
        };


        template <typename OPERATION_IMPL>
        void instrument(INS ins, REG tmp_reg)
        {
          auto xed = INS_XedDec(ins); // TODO check that pointer does not need to be freed
          auto iform = xed_decoded_inst_get_iform_enum(xed);
          switch (iform)
          {
          case xed_iform_enum_t::XED_IFORM_VADDSS_XMMf32_MASKmskw_XMMf32_XMMf32_AVX512:
            wrap<4, float, OPERATION_IMPL::add_float>::apply<1>(ins, xed_decoded_inst_uses_embedded_broadcast(xed), false, tmp_reg);
            break;
          case xed_iform_enum_t::XED_IFORM_VADDSS_XMMf32_MASKmskw_XMMf32_MEMf32_AVX512:
            wrap<4, float, OPERATION_IMPL::add_float>::apply<1>(ins, xed_decoded_inst_uses_embedded_broadcast(xed), true, tmp_reg);
            break;

          case xed_iform_enum_t::XED_IFORM_VADDPS_XMMf32_MASKmskw_XMMf32_XMMf32_AVX512:
            wrap<4, float, OPERATION_IMPL::add_float>::apply(ins, xed_decoded_inst_uses_embedded_broadcast(xed), false, tmp_reg);
            break;
          case xed_iform_enum_t::XED_IFORM_VADDPS_XMMf32_MASKmskw_XMMf32_MEMf32_AVX512:
            wrap<4, float, OPERATION_IMPL::add_float>::apply(ins, xed_decoded_inst_uses_embedded_broadcast(xed), true, tmp_reg);
            break;

          case xed_iform_enum_t::XED_IFORM_VADDPS_YMMf32_MASKmskw_YMMf32_YMMf32_AVX512:
            wrap<8, float, OPERATION_IMPL::add_float>::apply(ins, xed_decoded_inst_uses_embedded_broadcast(xed), false, tmp_reg);
            break;
          case xed_iform_enum_t::XED_IFORM_VADDPS_YMMf32_MASKmskw_YMMf32_MEMf32_AVX512:
            wrap<8, float, OPERATION_IMPL::add_float>::apply(ins, xed_decoded_inst_uses_embedded_broadcast(xed), true, tmp_reg);
            break;

          case xed_iform_enum_t::XED_IFORM_VADDPS_ZMMf32_MASKmskw_ZMMf32_ZMMf32_AVX512:
            wrap<16, float, OPERATION_IMPL::add_float>::apply(ins, xed_decoded_inst_uses_embedded_broadcast(xed), false, tmp_reg);
            break;
          case xed_iform_enum_t::XED_IFORM_VADDPS_ZMMf32_MASKmskw_ZMMf32_MEMf32_AVX512:
            wrap<16, float, OPERATION_IMPL::add_float>::apply(ins, xed_decoded_inst_uses_embedded_broadcast(xed), true, tmp_reg);
            break;


          case xed_iform_enum_t::XED_IFORM_VADDSD_XMMf64_MASKmskw_XMMf64_XMMf64_AVX512:
            wrap<2, double, OPERATION_IMPL::add_double>::apply<1>(ins, xed_decoded_inst_uses_embedded_broadcast(xed), false, tmp_reg);
            break;
          case xed_iform_enum_t::XED_IFORM_VADDSD_XMMf64_MASKmskw_XMMf64_MEMf64_AVX512:
            wrap<2, double, OPERATION_IMPL::add_double>::apply<1>(ins, xed_decoded_inst_uses_embedded_broadcast(xed), true, tmp_reg);
            break;

          case xed_iform_enum_t::XED_IFORM_VADDPD_XMMf64_MASKmskw_XMMf64_XMMf64_AVX512:
            wrap<2, double, OPERATION_IMPL::add_double>::apply(ins, xed_decoded_inst_uses_embedded_broadcast(xed), false, tmp_reg);
            break;
          case xed_iform_enum_t::XED_IFORM_VADDPD_XMMf64_MASKmskw_XMMf64_MEMf64_AVX512:
            wrap<2, double, OPERATION_IMPL::add_double>::apply(ins, xed_decoded_inst_uses_embedded_broadcast(xed), true, tmp_reg);
            break;

          case xed_iform_enum_t::XED_IFORM_VADDPD_YMMf64_MASKmskw_YMMf64_YMMf64_AVX512:
            wrap<4, double, OPERATION_IMPL::add_double>::apply(ins, xed_decoded_inst_uses_embedded_broadcast(xed), false, tmp_reg);
            break;
          case xed_iform_enum_t::XED_IFORM_VADDPD_YMMf64_MASKmskw_YMMf64_MEMf64_AVX512:
            wrap<4, double, OPERATION_IMPL::add_double>::apply(ins, xed_decoded_inst_uses_embedded_broadcast(xed), true, tmp_reg);
            break;

          case xed_iform_enum_t::XED_IFORM_VADDPD_ZMMf64_MASKmskw_ZMMf64_ZMMf64_AVX512:
            wrap<8, double, OPERATION_IMPL::add_double>::apply(ins, xed_decoded_inst_uses_embedded_broadcast(xed), false, tmp_reg);
            break;
          case xed_iform_enum_t::XED_IFORM_VADDPD_ZMMf64_MASKmskw_ZMMf64_MEMf64_AVX512:
            wrap<8, double, OPERATION_IMPL::add_double>::apply(ins, xed_decoded_inst_uses_embedded_broadcast(xed), true, tmp_reg);
            break;




          case xed_iform_enum_t::XED_IFORM_VSUBSS_XMMf32_MASKmskw_XMMf32_XMMf32_AVX512:
            wrap<4, float, OPERATION_IMPL::sub_float>::apply<1>(ins, xed_decoded_inst_uses_embedded_broadcast(xed), false, tmp_reg);
            break;
          case xed_iform_enum_t::XED_IFORM_VSUBSS_XMMf32_MASKmskw_XMMf32_MEMf32_AVX512:
            wrap<4, float, OPERATION_IMPL::sub_float>::apply<1>(ins, xed_decoded_inst_uses_embedded_broadcast(xed), true, tmp_reg);
            break;

          case xed_iform_enum_t::XED_IFORM_VSUBPS_XMMf32_MASKmskw_XMMf32_XMMf32_AVX512:
            wrap<4, float, OPERATION_IMPL::sub_float>::apply(ins, xed_decoded_inst_uses_embedded_broadcast(xed), false, tmp_reg);
            break;
          case xed_iform_enum_t::XED_IFORM_VSUBPS_XMMf32_MASKmskw_XMMf32_MEMf32_AVX512:
            wrap<4, float, OPERATION_IMPL::sub_float>::apply(ins, xed_decoded_inst_uses_embedded_broadcast(xed), true, tmp_reg);
            break;

          case xed_iform_enum_t::XED_IFORM_VSUBPS_YMMf32_MASKmskw_YMMf32_YMMf32_AVX512:
            wrap<8, float, OPERATION_IMPL::sub_float>::apply(ins, xed_decoded_inst_uses_embedded_broadcast(xed), false, tmp_reg);
            break;
          case xed_iform_enum_t::XED_IFORM_VSUBPS_YMMf32_MASKmskw_YMMf32_MEMf32_AVX512:
            wrap<8, float, OPERATION_IMPL::sub_float>::apply(ins, xed_decoded_inst_uses_embedded_broadcast(xed), true, tmp_reg);
            break;

          case xed_iform_enum_t::XED_IFORM_VSUBPS_ZMMf32_MASKmskw_ZMMf32_ZMMf32_AVX512:
            wrap<16, float, OPERATION_IMPL::sub_float>::apply(ins, xed_decoded_inst_uses_embedded_broadcast(xed), false, tmp_reg);
            break;
          case xed_iform_enum_t::XED_IFORM_VSUBPS_ZMMf32_MASKmskw_ZMMf32_MEMf32_AVX512:
            wrap<16, float, OPERATION_IMPL::sub_float>::apply(ins, xed_decoded_inst_uses_embedded_broadcast(xed), true, tmp_reg);
            break;


          case xed_iform_enum_t::XED_IFORM_VSUBSD_XMMf64_MASKmskw_XMMf64_XMMf64_AVX512:
            wrap<2, double, OPERATION_IMPL::sub_double>::apply<1>(ins, xed_decoded_inst_uses_embedded_broadcast(xed), false, tmp_reg);
            break;
          case xed_iform_enum_t::XED_IFORM_VSUBSD_XMMf64_MASKmskw_XMMf64_MEMf64_AVX512:
            wrap<2, double, OPERATION_IMPL::sub_double>::apply<1>(ins, xed_decoded_inst_uses_embedded_broadcast(xed), true, tmp_reg);
            break;

          case xed_iform_enum_t::XED_IFORM_VSUBPD_XMMf64_MASKmskw_XMMf64_XMMf64_AVX512:
            wrap<2, double, OPERATION_IMPL::sub_double>::apply(ins, xed_decoded_inst_uses_embedded_broadcast(xed), false, tmp_reg);
            break;
          case xed_iform_enum_t::XED_IFORM_VSUBPD_XMMf64_MASKmskw_XMMf64_MEMf64_AVX512:
            wrap<2, double, OPERATION_IMPL::sub_double>::apply(ins, xed_decoded_inst_uses_embedded_broadcast(xed), true, tmp_reg);
            break;

          case xed_iform_enum_t::XED_IFORM_VSUBPD_YMMf64_MASKmskw_YMMf64_YMMf64_AVX512:
            wrap<4, double, OPERATION_IMPL::sub_double>::apply(ins, xed_decoded_inst_uses_embedded_broadcast(xed), false, tmp_reg);
            break;
          case xed_iform_enum_t::XED_IFORM_VSUBPD_YMMf64_MASKmskw_YMMf64_MEMf64_AVX512:
            wrap<4, double, OPERATION_IMPL::sub_double>::apply(ins, xed_decoded_inst_uses_embedded_broadcast(xed), true, tmp_reg);
            break;

          case xed_iform_enum_t::XED_IFORM_VSUBPD_ZMMf64_MASKmskw_ZMMf64_ZMMf64_AVX512:
            wrap<8, double, OPERATION_IMPL::sub_double>::apply(ins, xed_decoded_inst_uses_embedded_broadcast(xed), false, tmp_reg);
            break;
          case xed_iform_enum_t::XED_IFORM_VSUBPD_ZMMf64_MASKmskw_ZMMf64_MEMf64_AVX512:
            wrap<8, double, OPERATION_IMPL::sub_double>::apply(ins, xed_decoded_inst_uses_embedded_broadcast(xed), true, tmp_reg);
            break;




          case xed_iform_enum_t::XED_IFORM_VMULSS_XMMf32_MASKmskw_XMMf32_XMMf32_AVX512:
            wrap<4, float, OPERATION_IMPL::mul_float>::apply<1>(ins, xed_decoded_inst_uses_embedded_broadcast(xed), false, tmp_reg);
            break;
          case xed_iform_enum_t::XED_IFORM_VMULSS_XMMf32_MASKmskw_XMMf32_MEMf32_AVX512:
            wrap<4, float, OPERATION_IMPL::mul_float>::apply<1>(ins, xed_decoded_inst_uses_embedded_broadcast(xed), true, tmp_reg);
            break;

          case xed_iform_enum_t::XED_IFORM_VMULPS_XMMf32_MASKmskw_XMMf32_XMMf32_AVX512:
            wrap<4, float, OPERATION_IMPL::mul_float>::apply(ins, xed_decoded_inst_uses_embedded_broadcast(xed), false, tmp_reg);
            break;
          case xed_iform_enum_t::XED_IFORM_VMULPS_XMMf32_MASKmskw_XMMf32_MEMf32_AVX512:
            wrap<4, float, OPERATION_IMPL::mul_float>::apply(ins, xed_decoded_inst_uses_embedded_broadcast(xed), true, tmp_reg);
            break;

          case xed_iform_enum_t::XED_IFORM_VMULPS_YMMf32_MASKmskw_YMMf32_YMMf32_AVX512:
            wrap<8, float, OPERATION_IMPL::mul_float>::apply(ins, xed_decoded_inst_uses_embedded_broadcast(xed), false, tmp_reg);
            break;
          case xed_iform_enum_t::XED_IFORM_VMULPS_YMMf32_MASKmskw_YMMf32_MEMf32_AVX512:
            wrap<8, float, OPERATION_IMPL::mul_float>::apply(ins, xed_decoded_inst_uses_embedded_broadcast(xed), true, tmp_reg);
            break;

          case xed_iform_enum_t::XED_IFORM_VMULPS_ZMMf32_MASKmskw_ZMMf32_ZMMf32_AVX512:
            wrap<16, float, OPERATION_IMPL::mul_float>::apply(ins, xed_decoded_inst_uses_embedded_broadcast(xed), false, tmp_reg);
            break;
          case xed_iform_enum_t::XED_IFORM_VMULPS_ZMMf32_MASKmskw_ZMMf32_MEMf32_AVX512:
            wrap<16, float, OPERATION_IMPL::mul_float>::apply(ins, xed_decoded_inst_uses_embedded_broadcast(xed), true, tmp_reg);
            break;


          case xed_iform_enum_t::XED_IFORM_VMULSD_XMMf64_MASKmskw_XMMf64_XMMf64_AVX512:
            wrap<2, double, OPERATION_IMPL::mul_double>::apply<1>(ins, xed_decoded_inst_uses_embedded_broadcast(xed), false, tmp_reg);
            break;
          case xed_iform_enum_t::XED_IFORM_VMULSD_XMMf64_MASKmskw_XMMf64_MEMf64_AVX512:
            wrap<2, double, OPERATION_IMPL::mul_double>::apply<1>(ins, xed_decoded_inst_uses_embedded_broadcast(xed), true, tmp_reg);
            break;

          case xed_iform_enum_t::XED_IFORM_VMULPD_XMMf64_MASKmskw_XMMf64_XMMf64_AVX512:
            wrap<2, double, OPERATION_IMPL::mul_double>::apply(ins, xed_decoded_inst_uses_embedded_broadcast(xed), false, tmp_reg);
            break;
          case xed_iform_enum_t::XED_IFORM_VMULPD_XMMf64_MASKmskw_XMMf64_MEMf64_AVX512:
            wrap<2, double, OPERATION_IMPL::mul_double>::apply(ins, xed_decoded_inst_uses_embedded_broadcast(xed), true, tmp_reg);
            break;

          case xed_iform_enum_t::XED_IFORM_VMULPD_YMMf64_MASKmskw_YMMf64_YMMf64_AVX512:
            wrap<4, double, OPERATION_IMPL::mul_double>::apply(ins, xed_decoded_inst_uses_embedded_broadcast(xed), false, tmp_reg);
            break;
          case xed_iform_enum_t::XED_IFORM_VMULPD_YMMf64_MASKmskw_YMMf64_MEMf64_AVX512:
            wrap<4, double, OPERATION_IMPL::mul_double>::apply(ins, xed_decoded_inst_uses_embedded_broadcast(xed), true, tmp_reg);
            break;

          case xed_iform_enum_t::XED_IFORM_VMULPD_ZMMf64_MASKmskw_ZMMf64_ZMMf64_AVX512:
            wrap<8, double, OPERATION_IMPL::mul_double>::apply(ins, xed_decoded_inst_uses_embedded_broadcast(xed), false, tmp_reg);
            break;
          case xed_iform_enum_t::XED_IFORM_VMULPD_ZMMf64_MASKmskw_ZMMf64_MEMf64_AVX512:
            wrap<8, double, OPERATION_IMPL::mul_double>::apply(ins, xed_decoded_inst_uses_embedded_broadcast(xed), true, tmp_reg);
            break;




          case xed_iform_enum_t::XED_IFORM_VDIVSS_XMMf32_MASKmskw_XMMf32_XMMf32_AVX512:
            wrap<4, float, OPERATION_IMPL::div_float>::apply<1>(ins, xed_decoded_inst_uses_embedded_broadcast(xed), false, tmp_reg);
            break;
          case xed_iform_enum_t::XED_IFORM_VDIVSS_XMMf32_MASKmskw_XMMf32_MEMf32_AVX512:
            wrap<4, float, OPERATION_IMPL::div_float>::apply<1>(ins, xed_decoded_inst_uses_embedded_broadcast(xed), true, tmp_reg);
            break;

          case xed_iform_enum_t::XED_IFORM_VDIVPS_XMMf32_MASKmskw_XMMf32_XMMf32_AVX512:
            wrap<4, float, OPERATION_IMPL::div_float>::apply(ins, xed_decoded_inst_uses_embedded_broadcast(xed), false, tmp_reg);
            break;
          case xed_iform_enum_t::XED_IFORM_VDIVPS_XMMf32_MASKmskw_XMMf32_MEMf32_AVX512:
            wrap<4, float, OPERATION_IMPL::div_float>::apply(ins, xed_decoded_inst_uses_embedded_broadcast(xed), true, tmp_reg);
            break;

          case xed_iform_enum_t::XED_IFORM_VDIVPS_YMMf32_MASKmskw_YMMf32_YMMf32_AVX512:
            wrap<8, float, OPERATION_IMPL::div_float>::apply(ins, xed_decoded_inst_uses_embedded_broadcast(xed), false, tmp_reg);
            break;
          case xed_iform_enum_t::XED_IFORM_VDIVPS_YMMf32_MASKmskw_YMMf32_MEMf32_AVX512:
            wrap<8, float, OPERATION_IMPL::div_float>::apply(ins, xed_decoded_inst_uses_embedded_broadcast(xed), true, tmp_reg);
            break;

          case xed_iform_enum_t::XED_IFORM_VDIVPS_ZMMf32_MASKmskw_ZMMf32_ZMMf32_AVX512:
            wrap<16, float, OPERATION_IMPL::div_float>::apply(ins, xed_decoded_inst_uses_embedded_broadcast(xed), false, tmp_reg);
            break;
          case xed_iform_enum_t::XED_IFORM_VDIVPS_ZMMf32_MASKmskw_ZMMf32_MEMf32_AVX512:
            wrap<16, float, OPERATION_IMPL::div_float>::apply(ins, xed_decoded_inst_uses_embedded_broadcast(xed), true, tmp_reg);
            break;


          case xed_iform_enum_t::XED_IFORM_VDIVSD_XMMf64_MASKmskw_XMMf64_XMMf64_AVX512:
            wrap<2, double, OPERATION_IMPL::div_double>::apply<1>(ins, xed_decoded_inst_uses_embedded_broadcast(xed), false, tmp_reg);
            break;
          case xed_iform_enum_t::XED_IFORM_VDIVSD_XMMf64_MASKmskw_XMMf64_MEMf64_AVX512:
            wrap<2, double, OPERATION_IMPL::div_double>::apply<1>(ins, xed_decoded_inst_uses_embedded_broadcast(xed), true, tmp_reg);
            break;

          case xed_iform_enum_t::XED_IFORM_VDIVPD_XMMf64_MASKmskw_XMMf64_XMMf64_AVX512:
            wrap<2, double, OPERATION_IMPL::div_double>::apply(ins, xed_decoded_inst_uses_embedded_broadcast(xed), false, tmp_reg);
            break;
          case xed_iform_enum_t::XED_IFORM_VDIVPD_XMMf64_MASKmskw_XMMf64_MEMf64_AVX512:
            wrap<2, double, OPERATION_IMPL::div_double>::apply(ins, xed_decoded_inst_uses_embedded_broadcast(xed), true, tmp_reg);
            break;

          case xed_iform_enum_t::XED_IFORM_VDIVPD_YMMf64_MASKmskw_YMMf64_YMMf64_AVX512:
            wrap<4, double, OPERATION_IMPL::div_double>::apply(ins, xed_decoded_inst_uses_embedded_broadcast(xed), false, tmp_reg);
            break;
          case xed_iform_enum_t::XED_IFORM_VDIVPD_YMMf64_MASKmskw_YMMf64_MEMf64_AVX512:
            wrap<4, double, OPERATION_IMPL::div_double>::apply(ins, xed_decoded_inst_uses_embedded_broadcast(xed), true, tmp_reg);
            break;

          case xed_iform_enum_t::XED_IFORM_VDIVPD_ZMMf64_MASKmskw_ZMMf64_ZMMf64_AVX512:
            wrap<8, double, OPERATION_IMPL::div_double>::apply(ins, xed_decoded_inst_uses_embedded_broadcast(xed), false, tmp_reg);
            break;
          case xed_iform_enum_t::XED_IFORM_VDIVPD_ZMMf64_MASKmskw_ZMMf64_MEMf64_AVX512:
            wrap<8, double, OPERATION_IMPL::div_double>::apply(ins, xed_decoded_inst_uses_embedded_broadcast(xed), true, tmp_reg);
            break;

          default:
            break;
          }
        }
      }
    }
  }
}