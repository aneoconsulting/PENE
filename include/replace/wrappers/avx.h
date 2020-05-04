#pragma once
#include <pin.h>
#include <xed-decoded-inst-api.h>


namespace pene
{
  namespace replace
  {
    namespace wrappers
    {
      namespace avx
      {
        template<typename T, void (*op)(T, T, T*, void*)>
        struct wrap {

          template <int N, int I = 0>
          static void apply(T* const a, T* const b, T* c, void* ctx)
          {
            // TODO : add something like : 
            // if constexpr(I==0) op_counter++
            if constexpr (I < N)
            {
              op(a[I], b[I], &c[I], ctx);
              // this call replaces a static for loop from 0 to N-1
              apply<N, I + 1>(a, b, c, ctx);
            }
          }
        };


        template<int size>
        static void DoLoad(ADDRINT* dst, ADDRINT* addr)
        {
          PIN_SafeCopy(dst, addr, size);
        }

        template <typename OPERATION_IMPL>
        void instrument(INS ins, xed_decoded_inst_t* xed, xed_iform_enum_t iform, REG tmp_reg)
        {
          AFUNPTR func = nullptr;
          bool is_memory = true;

          switch (iform)
          {
          case xed_iform_enum_t::XED_IFORM_VADDSS_XMMdq_XMMdq_XMMd:
            is_memory = false;
          case xed_iform_enum_t::XED_IFORM_VADDSS_XMMdq_XMMdq_MEMd:
            func = (AFUNPTR)wrap<float, OPERATION_IMPL::add_float>::apply<1>;
            break;

          case xed_iform_enum_t::XED_IFORM_VSUBSS_XMMdq_XMMdq_XMMd:
            is_memory = false;
          case xed_iform_enum_t::XED_IFORM_VSUBSS_XMMdq_XMMdq_MEMd:
            func = (AFUNPTR)wrap<float, OPERATION_IMPL::sub_float>::apply<1>;
            break;

          case xed_iform_enum_t::XED_IFORM_VMULSS_XMMdq_XMMdq_XMMd:
            is_memory = false;
          case xed_iform_enum_t::XED_IFORM_VMULSS_XMMdq_XMMdq_MEMd:
            func = (AFUNPTR)wrap<float, OPERATION_IMPL::mul_float>::apply<1>;
            break;

          case xed_iform_enum_t::XED_IFORM_VDIVSS_XMMdq_XMMdq_XMMd:
            is_memory = false;
          case xed_iform_enum_t::XED_IFORM_VDIVSS_XMMdq_XMMdq_MEMd:
            func = (AFUNPTR)wrap<float, OPERATION_IMPL::div_float>::apply<1>;
            break;



          case xed_iform_enum_t::XED_IFORM_VADDSD_XMMdq_XMMdq_XMMq:
            is_memory = false;
          case xed_iform_enum_t::XED_IFORM_VADDSD_XMMdq_XMMdq_MEMq:
            func = (AFUNPTR)wrap<double, OPERATION_IMPL::add_double>::apply<1>;
            break;

          case xed_iform_enum_t::XED_IFORM_VSUBSD_XMMdq_XMMdq_XMMq:
            is_memory = false;
          case xed_iform_enum_t::XED_IFORM_VSUBSD_XMMdq_XMMdq_MEMq:
            func = (AFUNPTR)wrap<double, OPERATION_IMPL::sub_double>::apply<1>;
            break;

          case xed_iform_enum_t::XED_IFORM_VMULSD_XMMdq_XMMdq_XMMq:
            is_memory = false;
          case xed_iform_enum_t::XED_IFORM_VMULSD_XMMdq_XMMdq_MEMq:
            func = (AFUNPTR)wrap<double, OPERATION_IMPL::mul_double>::apply<1>;
            break;

          case xed_iform_enum_t::XED_IFORM_VDIVSD_XMMdq_XMMdq_XMMq:
            is_memory = false;
          case xed_iform_enum_t::XED_IFORM_VDIVSD_XMMdq_XMMdq_MEMq:
            func = (AFUNPTR)wrap<double, OPERATION_IMPL::div_double>::apply<1>;
            break;





          case xed_iform_enum_t::XED_IFORM_VADDPS_XMMdq_XMMdq_XMMdq:
            is_memory = false;
          case xed_iform_enum_t::XED_IFORM_VADDPS_XMMdq_XMMdq_MEMdq:
            func = (AFUNPTR)wrap<float, OPERATION_IMPL::add_float>::apply<4>;
            break;

          case xed_iform_enum_t::XED_IFORM_VSUBPS_XMMdq_XMMdq_XMMdq:
            is_memory = false;
          case xed_iform_enum_t::XED_IFORM_VSUBPS_XMMdq_XMMdq_MEMdq:
            func = (AFUNPTR)wrap<float, OPERATION_IMPL::sub_float>::apply<4>;
            break;

          case xed_iform_enum_t::XED_IFORM_VMULPS_XMMdq_XMMdq_XMMdq:
            is_memory = false;
          case xed_iform_enum_t::XED_IFORM_VMULPS_XMMdq_XMMdq_MEMdq:
            func = (AFUNPTR)wrap<float, OPERATION_IMPL::mul_float>::apply<4>;
            break;

          case xed_iform_enum_t::XED_IFORM_VDIVPS_XMMdq_XMMdq_XMMdq:
            is_memory = false;
          case xed_iform_enum_t::XED_IFORM_VDIVPS_XMMdq_XMMdq_MEMdq:
            func = (AFUNPTR)wrap<float, OPERATION_IMPL::div_float>::apply<4>;
            break;



          case xed_iform_enum_t::XED_IFORM_VADDPD_XMMdq_XMMdq_XMMdq:
            is_memory = false;
          case xed_iform_enum_t::XED_IFORM_VADDPD_XMMdq_XMMdq_MEMdq:
            func = (AFUNPTR)wrap<double, OPERATION_IMPL::add_double>::apply<2>;
            break;

          case xed_iform_enum_t::XED_IFORM_VSUBPD_XMMdq_XMMdq_XMMdq:
            is_memory = false;
          case xed_iform_enum_t::XED_IFORM_VSUBPD_XMMdq_XMMdq_MEMdq:
            func = (AFUNPTR)wrap<double, OPERATION_IMPL::sub_double>::apply<2>;
            break;

          case xed_iform_enum_t::XED_IFORM_VMULPD_XMMdq_XMMdq_XMMdq:
            is_memory = false;
          case xed_iform_enum_t::XED_IFORM_VMULPD_XMMdq_XMMdq_MEMdq:
            func = (AFUNPTR)wrap<double, OPERATION_IMPL::mul_double>::apply<2>;
            break;

          case xed_iform_enum_t::XED_IFORM_VDIVPD_XMMdq_XMMdq_XMMdq:
            is_memory = false;
          case xed_iform_enum_t::XED_IFORM_VDIVPD_XMMdq_XMMdq_MEMdq:
            func = (AFUNPTR)wrap<double, OPERATION_IMPL::div_double>::apply<2>;
            break;





          case xed_iform_enum_t::XED_IFORM_VADDPS_YMMqq_YMMqq_YMMqq:
            is_memory = false;
          case xed_iform_enum_t::XED_IFORM_VADDPS_YMMqq_YMMqq_MEMqq:
            func = (AFUNPTR)wrap<float, OPERATION_IMPL::add_float>::apply<8>;
            break;

          case xed_iform_enum_t::XED_IFORM_VSUBPS_YMMqq_YMMqq_YMMqq:
            is_memory = false;
          case xed_iform_enum_t::XED_IFORM_VSUBPS_YMMqq_YMMqq_MEMqq:
            func = (AFUNPTR)wrap<float, OPERATION_IMPL::sub_float>::apply<8>;
            break;

          case xed_iform_enum_t::XED_IFORM_VMULPS_YMMqq_YMMqq_YMMqq:
            is_memory = false;
          case xed_iform_enum_t::XED_IFORM_VMULPS_YMMqq_YMMqq_MEMqq:
            func = (AFUNPTR)wrap<float, OPERATION_IMPL::mul_float>::apply<8>;
            break;

          case xed_iform_enum_t::XED_IFORM_VDIVPS_YMMqq_YMMqq_YMMqq:
            is_memory = false;
          case xed_iform_enum_t::XED_IFORM_VDIVPS_YMMqq_YMMqq_MEMqq:
            func = (AFUNPTR)wrap<float, OPERATION_IMPL::div_float>::apply<8>;
            break;



          case xed_iform_enum_t::XED_IFORM_VADDPD_YMMqq_YMMqq_YMMqq:
            is_memory = false;
          case xed_iform_enum_t::XED_IFORM_VADDPD_YMMqq_YMMqq_MEMqq:
            func = (AFUNPTR)wrap<double, OPERATION_IMPL::add_double>::apply<4>;
            break;

          case xed_iform_enum_t::XED_IFORM_VSUBPD_YMMqq_YMMqq_YMMqq:
            is_memory = false;
          case xed_iform_enum_t::XED_IFORM_VSUBPD_YMMqq_YMMqq_MEMqq:
            func = (AFUNPTR)wrap<double, OPERATION_IMPL::sub_double>::apply<4>;
            break;

          case xed_iform_enum_t::XED_IFORM_VMULPD_YMMqq_YMMqq_YMMqq:
            is_memory = false;
          case xed_iform_enum_t::XED_IFORM_VMULPD_YMMqq_YMMqq_MEMqq:
            func = (AFUNPTR)wrap<double, OPERATION_IMPL::mul_double>::apply<4>;
            break;

          case xed_iform_enum_t::XED_IFORM_VDIVPD_YMMqq_YMMqq_YMMqq:
            is_memory = false;
          case xed_iform_enum_t::XED_IFORM_VDIVPD_YMMqq_YMMqq_MEMqq:
            func = (AFUNPTR)wrap<double, OPERATION_IMPL::div_double>::apply<4>;
            break;
          default:
            return;
          }

          if (func != nullptr)
          {
            // real execution of instruction so that all side effects have occured
            // the call then only overload the required results
            INS ins2 = INS_Next(ins);
            if (is_memory)
            {
              INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)DoLoad<32>,
                IARG_REG_REFERENCE, tmp_reg,
                IARG_MEMORYREAD_EA,
                IARG_END);


              INS_InsertCall(ins2, IPOINT_BEFORE, func,
                IARG_REG_CONST_REFERENCE, INS_OperandReg(ins, 1),
                IARG_REG_CONST_REFERENCE, tmp_reg,
                IARG_REG_REFERENCE, INS_OperandReg(ins, 0),
                IARG_ADDRINT, nullptr,
                IARG_END);
            }
            else
            {
              INS_InsertCall(ins2, IPOINT_BEFORE, func,
                IARG_REG_CONST_REFERENCE, INS_OperandReg(ins, 1),
                IARG_REG_CONST_REFERENCE, INS_OperandReg(ins, 2),
                IARG_REG_REFERENCE, INS_OperandReg(ins, 0),
                IARG_ADDRINT, nullptr,
                IARG_END);
            }
          }
        }



      }
    }
  }
}