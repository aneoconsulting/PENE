#pragma once
#include <immintrin.h>
#include <pin.H>
extern "C"
{
# include <xed-decoded-inst-api.h>
}
#include "utils/copy_register.h"


namespace pene
{
  namespace replace
  {
    namespace wrappers
    {
      namespace avx
      {
        using namespace utils;

        template<typename T, void (*op)(T, T, T*, void*)>
        struct wrap {

          template <int N, int I = 0>
          static void PIN_FAST_ANALYSIS_CALL compute(T* const a, T* const b, T* c, void* ctx)
          {
            // TODO : add something like : 
            // if constexpr(I==0) op_counter++
            if constexpr (I < N)
            {
              op(a[I], b[I], &c[I], ctx);
              // this call replaces a static for loop from 0 to N-1
              compute<N, I + 1>(a, b, c, ctx);
            }
            /*
            else // To remove C4100 warning with Visual Studio 2019
            {
              a;
              b;
              c;
              ctx;
            }
            */
          }

          template <int SIZE>
          static auto save_registers(INS ins, bool is_memory, REG tmp_reg1, REG tmp_reg2)
          {
            REG reg1 = INS_OperandReg(ins, 1);
            if (INS_OperandReg(ins, 0) == reg1)
            {
              INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)copy_register<SIZE>,
                IARG_FAST_ANALYSIS_CALL,
                IARG_REG_REFERENCE, tmp_reg1,
                IARG_REG_CONST_REFERENCE, INS_OperandReg(ins, 1),
                IARG_END);
              reg1 = tmp_reg1;
            }

            REG reg2;
            if (is_memory)
            {
              INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)copy_register<SIZE>,
                IARG_FAST_ANALYSIS_CALL,
                IARG_REG_REFERENCE, tmp_reg2,
                IARG_MEMORYREAD_EA,
                IARG_END);
              reg2 = tmp_reg2;
            }
            else
            {
              reg2 = INS_OperandReg(ins, 2);
              // if result is overwritten in operand, save the operand
              if (INS_OperandReg(ins, 0) == reg2)
              {
                if (INS_OperandReg(ins, 1) == reg2)
                {
                  reg2 = reg1;
                }
                else
                {
                  INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)copy_register<SIZE>,
                    IARG_FAST_ANALYSIS_CALL,
                    IARG_REG_REFERENCE, tmp_reg2,
                    IARG_REG_CONST_REFERENCE, INS_OperandReg(ins, 2),
                    IARG_END);
                  reg2 = tmp_reg2;
                }
              }
            }
            struct RetVals
            {
              REG r1;
              REG r2;
            };
            return RetVals { reg1, reg2 };
          }

          template <int N>
          static void apply(void* backend_ctx, INS ins, bool is_memory, REG tmp_reg1, REG tmp_reg2)
          {
            // real execution of instruction so that all side effects occurs
            // the call then only overload the required results


            // if result is overwritten in operand, save the operand
            auto [reg1, reg2] = save_registers<N * sizeof(T)>(ins, is_memory, tmp_reg1, tmp_reg2);

            INS_InsertCall(ins, IPOINT_AFTER, (AFUNPTR)compute<N>,
              IARG_FAST_ANALYSIS_CALL,
              IARG_REG_CONST_REFERENCE, reg1,
              IARG_REG_CONST_REFERENCE, reg2,
              IARG_REG_REFERENCE, INS_OperandReg(ins, 0),
              IARG_PTR, backend_ctx,
              IARG_END);
          }
        };


        template <typename OPERATION_IMPL>
        void instrument(void* backend_ctx, INS ins, REG tmp_reg1, REG tmp_reg2)
        {
          auto xed = INS_XedDec(ins); // TODO check that pointer does not need to be freed
          auto iform = xed_decoded_inst_get_iform_enum(xed);
          switch (iform)
          {
          case xed_iform_enum_t::XED_IFORM_VADDSS_XMMdq_XMMdq_XMMd:
            wrap<float, OPERATION_IMPL::add_float>::template apply<1>(backend_ctx, ins,false, tmp_reg1, tmp_reg2);
            break;
          case xed_iform_enum_t::XED_IFORM_VADDSS_XMMdq_XMMdq_MEMd:
            wrap<float, OPERATION_IMPL::add_float>::template apply<1>(backend_ctx, ins,true, tmp_reg1, tmp_reg2);
            break;

          case xed_iform_enum_t::XED_IFORM_VADDPS_XMMdq_XMMdq_XMMdq:
            wrap<float, OPERATION_IMPL::add_float>::template apply<4>(backend_ctx, ins,false, tmp_reg1, tmp_reg2);
            break;
          case xed_iform_enum_t::XED_IFORM_VADDPS_XMMdq_XMMdq_MEMdq:
            wrap<float, OPERATION_IMPL::add_float>::template apply<4>(backend_ctx, ins,true, tmp_reg1, tmp_reg2);
            break;

          case xed_iform_enum_t::XED_IFORM_VADDPS_YMMqq_YMMqq_YMMqq:
            wrap<float, OPERATION_IMPL::add_float>::template apply<8>(backend_ctx, ins,false, tmp_reg1, tmp_reg2);
            break;
          case xed_iform_enum_t::XED_IFORM_VADDPS_YMMqq_YMMqq_MEMqq:
            wrap<float, OPERATION_IMPL::add_float>::template apply<8>(backend_ctx, ins,true, tmp_reg1, tmp_reg2);
            break;



          case xed_iform_enum_t::XED_IFORM_VADDSD_XMMdq_XMMdq_XMMq:
            wrap<double, OPERATION_IMPL::add_double>::template apply<1>(backend_ctx, ins,false, tmp_reg1, tmp_reg2);
            break;
          case xed_iform_enum_t::XED_IFORM_VADDSD_XMMdq_XMMdq_MEMq:
            wrap<double, OPERATION_IMPL::add_double>::template apply<1>(backend_ctx, ins,true, tmp_reg1, tmp_reg2);
            break;

          case xed_iform_enum_t::XED_IFORM_VADDPD_XMMdq_XMMdq_XMMdq:
            wrap<double, OPERATION_IMPL::add_double>::template apply<2>(backend_ctx, ins,false, tmp_reg1, tmp_reg2);
            break;
          case xed_iform_enum_t::XED_IFORM_VADDPD_XMMdq_XMMdq_MEMdq:
            wrap<double, OPERATION_IMPL::add_double>::template apply<2>(backend_ctx, ins,true, tmp_reg1, tmp_reg2);
            break;

          case xed_iform_enum_t::XED_IFORM_VADDPD_YMMqq_YMMqq_YMMqq:
            wrap<double, OPERATION_IMPL::add_double>::template apply<4>(backend_ctx, ins,false, tmp_reg1, tmp_reg2);
            break;
          case xed_iform_enum_t::XED_IFORM_VADDPD_YMMqq_YMMqq_MEMqq:
            wrap<double, OPERATION_IMPL::add_double>::template apply<4>(backend_ctx, ins,true, tmp_reg1, tmp_reg2);
            break;





          case xed_iform_enum_t::XED_IFORM_VSUBSS_XMMdq_XMMdq_XMMd:
            wrap<float, OPERATION_IMPL::sub_float>::template apply<1>(backend_ctx, ins,false, tmp_reg1, tmp_reg2);
            break;
          case xed_iform_enum_t::XED_IFORM_VSUBSS_XMMdq_XMMdq_MEMd:
            wrap<float, OPERATION_IMPL::sub_float>::template apply<1>(backend_ctx, ins,true, tmp_reg1, tmp_reg2);
            break;

          case xed_iform_enum_t::XED_IFORM_VSUBPS_XMMdq_XMMdq_XMMdq:
            wrap<float, OPERATION_IMPL::sub_float>::template apply<4>(backend_ctx, ins,false, tmp_reg1, tmp_reg2);
            break;
          case xed_iform_enum_t::XED_IFORM_VSUBPS_XMMdq_XMMdq_MEMdq:
            wrap<float, OPERATION_IMPL::sub_float>::template apply<4>(backend_ctx, ins,true, tmp_reg1, tmp_reg2);
            break;

          case xed_iform_enum_t::XED_IFORM_VSUBPS_YMMqq_YMMqq_YMMqq:
            wrap<float, OPERATION_IMPL::sub_float>::template apply<8>(backend_ctx, ins,false, tmp_reg1, tmp_reg2);
            break;
          case xed_iform_enum_t::XED_IFORM_VSUBPS_YMMqq_YMMqq_MEMqq:
            wrap<float, OPERATION_IMPL::sub_float>::template apply<8>(backend_ctx, ins,true, tmp_reg1, tmp_reg2);
            break;



          case xed_iform_enum_t::XED_IFORM_VSUBSD_XMMdq_XMMdq_XMMq:
            wrap<double, OPERATION_IMPL::sub_double>::template apply<1>(backend_ctx, ins,false, tmp_reg1, tmp_reg2);
            break;
          case xed_iform_enum_t::XED_IFORM_VSUBSD_XMMdq_XMMdq_MEMq:
            wrap<double, OPERATION_IMPL::sub_double>::template apply<1>(backend_ctx, ins,true, tmp_reg1, tmp_reg2);
            break;

          case xed_iform_enum_t::XED_IFORM_VSUBPD_XMMdq_XMMdq_XMMdq:
            wrap<double, OPERATION_IMPL::sub_double>::template apply<2>(backend_ctx, ins,false, tmp_reg1, tmp_reg2);
            break;
          case xed_iform_enum_t::XED_IFORM_VSUBPD_XMMdq_XMMdq_MEMdq:
            wrap<double, OPERATION_IMPL::sub_double>::template apply<2>(backend_ctx, ins,true, tmp_reg1, tmp_reg2);
            break;

          case xed_iform_enum_t::XED_IFORM_VSUBPD_YMMqq_YMMqq_YMMqq:
            wrap<double, OPERATION_IMPL::sub_double>::template apply<4>(backend_ctx, ins,false, tmp_reg1, tmp_reg2);
            break;
          case xed_iform_enum_t::XED_IFORM_VSUBPD_YMMqq_YMMqq_MEMqq:
            wrap<double, OPERATION_IMPL::sub_double>::template apply<4>(backend_ctx, ins,true, tmp_reg1, tmp_reg2);
            break;




          case xed_iform_enum_t::XED_IFORM_VMULSS_XMMdq_XMMdq_XMMd:
            wrap<float, OPERATION_IMPL::mul_float>::template apply<1>(backend_ctx, ins,false, tmp_reg1, tmp_reg2);
            break;
          case xed_iform_enum_t::XED_IFORM_VMULSS_XMMdq_XMMdq_MEMd:
            wrap<float, OPERATION_IMPL::mul_float>::template apply<1>(backend_ctx, ins,true, tmp_reg1, tmp_reg2);
            break;

          case xed_iform_enum_t::XED_IFORM_VMULPS_XMMdq_XMMdq_XMMdq:
            wrap<float, OPERATION_IMPL::mul_float>::template apply<4>(backend_ctx, ins,false, tmp_reg1, tmp_reg2);
            break;
          case xed_iform_enum_t::XED_IFORM_VMULPS_XMMdq_XMMdq_MEMdq:
            wrap<float, OPERATION_IMPL::mul_float>::template apply<4>(backend_ctx, ins,true, tmp_reg1, tmp_reg2);
            break;

          case xed_iform_enum_t::XED_IFORM_VMULPS_YMMqq_YMMqq_YMMqq:
            wrap<float, OPERATION_IMPL::mul_float>::template apply<8>(backend_ctx, ins,false, tmp_reg1, tmp_reg2);
            break;
          case xed_iform_enum_t::XED_IFORM_VMULPS_YMMqq_YMMqq_MEMqq:
            wrap<float, OPERATION_IMPL::mul_float>::template apply<8>(backend_ctx, ins,true, tmp_reg1, tmp_reg2);
            break;



          case xed_iform_enum_t::XED_IFORM_VMULSD_XMMdq_XMMdq_XMMq:
            wrap<double, OPERATION_IMPL::mul_double>::template apply<1>(backend_ctx, ins,false, tmp_reg1, tmp_reg2);
            break;
          case xed_iform_enum_t::XED_IFORM_VMULSD_XMMdq_XMMdq_MEMq:
            wrap<double, OPERATION_IMPL::mul_double>::template apply<1>(backend_ctx, ins,true, tmp_reg1, tmp_reg2);
            break;

          case xed_iform_enum_t::XED_IFORM_VMULPD_XMMdq_XMMdq_XMMdq:
            wrap<double, OPERATION_IMPL::mul_double>::template apply<2>(backend_ctx, ins,false, tmp_reg1, tmp_reg2);
            break;
          case xed_iform_enum_t::XED_IFORM_VMULPD_XMMdq_XMMdq_MEMdq:
            wrap<double, OPERATION_IMPL::mul_double>::template apply<2>(backend_ctx, ins,true, tmp_reg1, tmp_reg2);
            break;

          case xed_iform_enum_t::XED_IFORM_VMULPD_YMMqq_YMMqq_YMMqq:
            wrap<double, OPERATION_IMPL::mul_double>::template apply<4>(backend_ctx, ins,false, tmp_reg1, tmp_reg2);
            break;
          case xed_iform_enum_t::XED_IFORM_VMULPD_YMMqq_YMMqq_MEMqq:
            wrap<double, OPERATION_IMPL::mul_double>::template apply<4>(backend_ctx, ins,true, tmp_reg1, tmp_reg2);
            break;




          case xed_iform_enum_t::XED_IFORM_VDIVSS_XMMdq_XMMdq_XMMd:
            wrap<float, OPERATION_IMPL::div_float>::template apply<1>(backend_ctx, ins,false, tmp_reg1, tmp_reg2);
            break;
          case xed_iform_enum_t::XED_IFORM_VDIVSS_XMMdq_XMMdq_MEMd:
            wrap<float, OPERATION_IMPL::div_float>::template apply<1>(backend_ctx, ins,true, tmp_reg1, tmp_reg2);
            break;

          case xed_iform_enum_t::XED_IFORM_VDIVPS_XMMdq_XMMdq_XMMdq:
            wrap<float, OPERATION_IMPL::div_float>::template apply<4>(backend_ctx, ins,false, tmp_reg1, tmp_reg2);
            break;
          case xed_iform_enum_t::XED_IFORM_VDIVPS_XMMdq_XMMdq_MEMdq:
            wrap<float, OPERATION_IMPL::div_float>::template apply<4>(backend_ctx, ins,true, tmp_reg1, tmp_reg2);
            break;

          case xed_iform_enum_t::XED_IFORM_VDIVPS_YMMqq_YMMqq_YMMqq:
            wrap<float, OPERATION_IMPL::div_float>::template apply<8>(backend_ctx, ins,false, tmp_reg1, tmp_reg2);
            break;
          case xed_iform_enum_t::XED_IFORM_VDIVPS_YMMqq_YMMqq_MEMqq:
            wrap<float, OPERATION_IMPL::div_float>::template apply<8>(backend_ctx, ins,true, tmp_reg1, tmp_reg2);
            break;



          case xed_iform_enum_t::XED_IFORM_VDIVSD_XMMdq_XMMdq_XMMq:
            wrap<double, OPERATION_IMPL::div_double>::template apply<1>(backend_ctx, ins,false, tmp_reg1, tmp_reg2);
            break;
          case xed_iform_enum_t::XED_IFORM_VDIVSD_XMMdq_XMMdq_MEMq:
            wrap<double, OPERATION_IMPL::div_double>::template apply<1>(backend_ctx, ins,true, tmp_reg1, tmp_reg2);
            break;

          case xed_iform_enum_t::XED_IFORM_VDIVPD_XMMdq_XMMdq_XMMdq:
            wrap<double, OPERATION_IMPL::div_double>::template apply<2>(backend_ctx, ins,false, tmp_reg1, tmp_reg2);
            break;
          case xed_iform_enum_t::XED_IFORM_VDIVPD_XMMdq_XMMdq_MEMdq:
            wrap<double, OPERATION_IMPL::div_double>::template apply<2>(backend_ctx, ins,true, tmp_reg1, tmp_reg2);
            break;

          case xed_iform_enum_t::XED_IFORM_VDIVPD_YMMqq_YMMqq_YMMqq:
            wrap<double, OPERATION_IMPL::div_double>::template apply<4>(backend_ctx, ins,false, tmp_reg1, tmp_reg2);
            break;
          case xed_iform_enum_t::XED_IFORM_VDIVPD_YMMqq_YMMqq_MEMqq:
            wrap<double, OPERATION_IMPL::div_double>::template apply<4>(backend_ctx, ins,true, tmp_reg1, tmp_reg2);
            break;

          default:
            return;
          }
        }
      }
    }
  }
}
