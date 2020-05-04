#pragma once
#include <pin.h>
#include <xed-decoded-inst-api.h>


namespace pene 
{
  namespace replace
  {
    namespace wrappers
    {
      namespace sse
      {
        template<typename T, void (*op)(T, T, T*, void*)>
        struct wrap {
          template <int N, int I=0>
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

        template <typename OPERATION_IMPL>
        void instrument(INS ins, xed_decoded_inst_t* xed, xed_iform_enum_t iform, REG tmp_reg)
        {
          switch (iform)
          {
          case xed_iform_enum_t::XED_IFORM_ADDSS_XMMss_XMMss:
            INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)wrap<float, OPERATION_IMPL::add_float>::apply<1>,
              IARG_REG_CONST_REFERENCE, INS_OperandReg(ins, 0),
              IARG_REG_CONST_REFERENCE, INS_OperandReg(ins, 1),
              IARG_REG_REFERENCE, INS_OperandReg(ins, 0),
              IARG_ADDRINT, nullptr,
              IARG_END);
            INS_Delete(ins);
            break;
          case xed_iform_enum_t::XED_IFORM_ADDSS_XMMss_MEMss:
            INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)wrap<float, OPERATION_IMPL::add_float>::apply<1>,
              IARG_REG_CONST_REFERENCE, INS_OperandReg(ins, 0),
              IARG_MEMORYREAD_EA,
              IARG_REG_REFERENCE, INS_OperandReg(ins, 0),
              IARG_ADDRINT, nullptr,
              IARG_END);
            INS_Delete(ins);
            break;


          case xed_iform_enum_t::XED_IFORM_SUBSS_XMMss_XMMss:
            INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)wrap<float, OPERATION_IMPL::sub_float>::apply<1>,
              IARG_REG_CONST_REFERENCE, INS_OperandReg(ins, 0),
              IARG_REG_CONST_REFERENCE, INS_OperandReg(ins, 1),
              IARG_REG_REFERENCE, INS_OperandReg(ins, 0),
              IARG_ADDRINT, nullptr,
              IARG_END);
            INS_Delete(ins);
            break;
          case xed_iform_enum_t::XED_IFORM_SUBSS_XMMss_MEMss:
            INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)wrap<float, OPERATION_IMPL::sub_float>::apply<1>,
              IARG_REG_CONST_REFERENCE, INS_OperandReg(ins, 0),
              IARG_MEMORYREAD_EA,
              IARG_REG_REFERENCE, INS_OperandReg(ins, 0),
              IARG_ADDRINT, nullptr,
              IARG_END);
            INS_Delete(ins);
            break;


          case xed_iform_enum_t::XED_IFORM_MULSS_XMMss_XMMss:
            INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)wrap<float, OPERATION_IMPL::mul_float>::apply<1>,
              IARG_REG_CONST_REFERENCE, INS_OperandReg(ins, 0),
              IARG_REG_CONST_REFERENCE, INS_OperandReg(ins, 1),
              IARG_REG_REFERENCE, INS_OperandReg(ins, 0),
              IARG_ADDRINT, nullptr,
              IARG_END);
            INS_Delete(ins);
            break;
          case xed_iform_enum_t::XED_IFORM_MULSS_XMMss_MEMss:
            INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)wrap<float, OPERATION_IMPL::mul_float>::apply<1>,
              IARG_REG_CONST_REFERENCE, INS_OperandReg(ins, 0),
              IARG_MEMORYREAD_EA,
              IARG_REG_REFERENCE, INS_OperandReg(ins, 0),
              IARG_ADDRINT, nullptr,
              IARG_END);
            INS_Delete(ins);
            break;


          case xed_iform_enum_t::XED_IFORM_DIVSS_XMMss_XMMss:
            INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)wrap<float, OPERATION_IMPL::div_float>::apply<1>,
              IARG_REG_CONST_REFERENCE, INS_OperandReg(ins, 0),
              IARG_REG_CONST_REFERENCE, INS_OperandReg(ins, 1),
              IARG_REG_REFERENCE, INS_OperandReg(ins, 0),
              IARG_ADDRINT, nullptr,
              IARG_END);
            INS_Delete(ins);
            break;
          case xed_iform_enum_t::XED_IFORM_DIVSS_XMMss_MEMss:
            INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)wrap<float, OPERATION_IMPL::div_float>::apply<1>,
              IARG_REG_CONST_REFERENCE, INS_OperandReg(ins, 0),
              IARG_MEMORYREAD_EA,
              IARG_REG_REFERENCE, INS_OperandReg(ins, 0),
              IARG_ADDRINT, nullptr,
              IARG_END);
            INS_Delete(ins);
            break;




          case xed_iform_enum_t::XED_IFORM_ADDSD_XMMsd_XMMsd:
            INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)wrap<double, OPERATION_IMPL::add_double>::apply<1>,
              IARG_REG_CONST_REFERENCE, INS_OperandReg(ins, 0),
              IARG_REG_CONST_REFERENCE, INS_OperandReg(ins, 1),
              IARG_REG_REFERENCE, INS_OperandReg(ins, 0),
              IARG_ADDRINT, nullptr,
              IARG_END);
            INS_Delete(ins);
            break;
          case xed_iform_enum_t::XED_IFORM_ADDSD_XMMsd_MEMsd:
            INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)wrap<double, OPERATION_IMPL::add_double>::apply<1>,
              IARG_REG_CONST_REFERENCE, INS_OperandReg(ins, 0),
              IARG_MEMORYREAD_EA,
              IARG_REG_REFERENCE, INS_OperandReg(ins, 0),
              IARG_ADDRINT, nullptr,
              IARG_END);
            INS_Delete(ins);
            break;


          case xed_iform_enum_t::XED_IFORM_SUBSD_XMMsd_XMMsd:
            INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)wrap<double, OPERATION_IMPL::sub_double>::apply<1>,
              IARG_REG_CONST_REFERENCE, INS_OperandReg(ins, 0),
              IARG_REG_CONST_REFERENCE, INS_OperandReg(ins, 1),
              IARG_REG_REFERENCE, INS_OperandReg(ins, 0),
              IARG_ADDRINT, nullptr,
              IARG_END);
            INS_Delete(ins);
            break;
          case xed_iform_enum_t::XED_IFORM_SUBSD_XMMsd_MEMsd:
            INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)wrap<double, OPERATION_IMPL::sub_double>::apply<1>,
              IARG_REG_CONST_REFERENCE, INS_OperandReg(ins, 0),
              IARG_MEMORYREAD_EA,
              IARG_REG_REFERENCE, INS_OperandReg(ins, 0),
              IARG_ADDRINT, nullptr,
              IARG_END);
            INS_Delete(ins);
            break;


          case xed_iform_enum_t::XED_IFORM_MULSD_XMMsd_XMMsd:
            INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)wrap<double, OPERATION_IMPL::mul_double>::apply<1>,
              IARG_REG_CONST_REFERENCE, INS_OperandReg(ins, 0),
              IARG_REG_CONST_REFERENCE, INS_OperandReg(ins, 1),
              IARG_REG_REFERENCE, INS_OperandReg(ins, 0),
              IARG_ADDRINT, nullptr,
              IARG_END);
            INS_Delete(ins);
            break;
          case xed_iform_enum_t::XED_IFORM_MULSD_XMMsd_MEMsd:
            INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)wrap<double, OPERATION_IMPL::mul_double>::apply<1>,
              IARG_REG_CONST_REFERENCE, INS_OperandReg(ins, 0),
              IARG_MEMORYREAD_EA,
              IARG_REG_REFERENCE, INS_OperandReg(ins, 0),
              IARG_ADDRINT, nullptr,
              IARG_END);
            INS_Delete(ins);
            break;


          case xed_iform_enum_t::XED_IFORM_DIVSD_XMMsd_XMMsd:
            INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)wrap<double, OPERATION_IMPL::div_double>::apply<1>,
              IARG_REG_CONST_REFERENCE, INS_OperandReg(ins, 0),
              IARG_REG_CONST_REFERENCE, INS_OperandReg(ins, 1),
              IARG_REG_REFERENCE, INS_OperandReg(ins, 0),
              IARG_ADDRINT, nullptr,
              IARG_END);
            INS_Delete(ins);
            break;
          case xed_iform_enum_t::XED_IFORM_DIVSD_XMMsd_MEMsd:
            INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)wrap<double, OPERATION_IMPL::div_double>::apply<1>,
              IARG_REG_CONST_REFERENCE, INS_OperandReg(ins, 0),
              IARG_MEMORYREAD_EA,
              IARG_REG_REFERENCE, INS_OperandReg(ins, 0),
              IARG_ADDRINT, nullptr,
              IARG_END);
            INS_Delete(ins);
            break;








          case xed_iform_enum_t::XED_IFORM_ADDPS_XMMps_XMMps:
            INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)wrap<float, OPERATION_IMPL::add_float>::apply<4>,
              IARG_REG_CONST_REFERENCE, INS_OperandReg(ins, 0),
              IARG_REG_CONST_REFERENCE, INS_OperandReg(ins, 1),
              IARG_REG_REFERENCE, INS_OperandReg(ins, 0),
              IARG_ADDRINT, nullptr,
              IARG_END);
            INS_Delete(ins);
            break;
          case xed_iform_enum_t::XED_IFORM_ADDPS_XMMps_MEMps:
            INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)wrap<float, OPERATION_IMPL::add_float>::apply<4>,
              IARG_REG_CONST_REFERENCE, INS_OperandReg(ins, 0),
              IARG_MEMORYREAD_EA,
              IARG_REG_REFERENCE, INS_OperandReg(ins, 0),
              IARG_ADDRINT, nullptr,
              IARG_END);
            INS_Delete(ins);
            break;


          case xed_iform_enum_t::XED_IFORM_SUBPS_XMMps_XMMps:
            INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)wrap<float, OPERATION_IMPL::sub_float>::apply<4>,
              IARG_REG_CONST_REFERENCE, INS_OperandReg(ins, 0),
              IARG_REG_CONST_REFERENCE, INS_OperandReg(ins, 1),
              IARG_REG_REFERENCE, INS_OperandReg(ins, 0),
              IARG_ADDRINT, nullptr,
              IARG_END);
            INS_Delete(ins);
            break;
          case xed_iform_enum_t::XED_IFORM_SUBPS_XMMps_MEMps:
            INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)wrap<float, OPERATION_IMPL::sub_float>::apply<4>,
              IARG_REG_CONST_REFERENCE, INS_OperandReg(ins, 0),
              IARG_MEMORYREAD_EA,
              IARG_REG_REFERENCE, INS_OperandReg(ins, 0),
              IARG_ADDRINT, nullptr,
              IARG_END);
            INS_Delete(ins);
            break;


          case xed_iform_enum_t::XED_IFORM_MULPS_XMMps_XMMps:
            INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)wrap<float, OPERATION_IMPL::mul_float>::apply<4>,
              IARG_REG_CONST_REFERENCE, INS_OperandReg(ins, 0),
              IARG_REG_CONST_REFERENCE, INS_OperandReg(ins, 1),
              IARG_REG_REFERENCE, INS_OperandReg(ins, 0),
              IARG_ADDRINT, nullptr,
              IARG_END);
            INS_Delete(ins);
            break;
          case xed_iform_enum_t::XED_IFORM_MULPS_XMMps_MEMps:
            INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)wrap<float, OPERATION_IMPL::mul_float>::apply<4>,
              IARG_REG_CONST_REFERENCE, INS_OperandReg(ins, 0),
              IARG_MEMORYREAD_EA,
              IARG_REG_REFERENCE, INS_OperandReg(ins, 0),
              IARG_ADDRINT, nullptr,
              IARG_END);
            INS_Delete(ins);
            break;


          case xed_iform_enum_t::XED_IFORM_DIVPS_XMMps_XMMps:
            INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)wrap<float, OPERATION_IMPL::div_float>::apply<4>,
              IARG_REG_CONST_REFERENCE, INS_OperandReg(ins, 0),
              IARG_REG_CONST_REFERENCE, INS_OperandReg(ins, 1),
              IARG_REG_REFERENCE, INS_OperandReg(ins, 0),
              IARG_ADDRINT, nullptr,
              IARG_END);
            INS_Delete(ins);
            break;
          case xed_iform_enum_t::XED_IFORM_DIVPS_XMMps_MEMps:
            INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)wrap<float, OPERATION_IMPL::div_float>::apply<4>,
              IARG_REG_CONST_REFERENCE, INS_OperandReg(ins, 0),
              IARG_MEMORYREAD_EA,
              IARG_REG_REFERENCE, INS_OperandReg(ins, 0),
              IARG_ADDRINT, nullptr,
              IARG_END);
            INS_Delete(ins);
            break;




          case xed_iform_enum_t::XED_IFORM_ADDPD_XMMpd_XMMpd:
            INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)wrap<double, OPERATION_IMPL::add_double>::apply<2>,
              IARG_REG_CONST_REFERENCE, INS_OperandReg(ins, 0),
              IARG_REG_CONST_REFERENCE, INS_OperandReg(ins, 1),
              IARG_REG_REFERENCE, INS_OperandReg(ins, 0),
              IARG_ADDRINT, nullptr,
              IARG_END);
            INS_Delete(ins);
            break;
          case xed_iform_enum_t::XED_IFORM_ADDPD_XMMpd_MEMpd:
            INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)wrap<double, OPERATION_IMPL::add_double>::apply<2>,
              IARG_REG_CONST_REFERENCE, INS_OperandReg(ins, 0),
              IARG_MEMORYREAD_EA,
              IARG_REG_REFERENCE, INS_OperandReg(ins, 0),
              IARG_ADDRINT, nullptr,
              IARG_END);
            INS_Delete(ins);
            break;


          case xed_iform_enum_t::XED_IFORM_SUBPD_XMMpd_XMMpd:
            INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)wrap<double, OPERATION_IMPL::sub_double>::apply<2>,
              IARG_REG_CONST_REFERENCE, INS_OperandReg(ins, 0),
              IARG_REG_CONST_REFERENCE, INS_OperandReg(ins, 1),
              IARG_REG_REFERENCE, INS_OperandReg(ins, 0),
              IARG_ADDRINT, nullptr,
              IARG_END);
            INS_Delete(ins);
            break;
          case xed_iform_enum_t::XED_IFORM_SUBPD_XMMpd_MEMpd:
            INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)wrap<double, OPERATION_IMPL::sub_double>::apply<2>,
              IARG_REG_CONST_REFERENCE, INS_OperandReg(ins, 0),
              IARG_MEMORYREAD_EA,
              IARG_REG_REFERENCE, INS_OperandReg(ins, 0),
              IARG_ADDRINT, nullptr,
              IARG_END);
            INS_Delete(ins);
            break;


          case xed_iform_enum_t::XED_IFORM_MULPD_XMMpd_XMMpd:
            INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)wrap<double, OPERATION_IMPL::mul_double>::apply<2>,
              IARG_REG_CONST_REFERENCE, INS_OperandReg(ins, 0),
              IARG_REG_CONST_REFERENCE, INS_OperandReg(ins, 1),
              IARG_REG_REFERENCE, INS_OperandReg(ins, 0),
              IARG_ADDRINT, nullptr,
              IARG_END);
            INS_Delete(ins);
            break;
          case xed_iform_enum_t::XED_IFORM_MULPD_XMMpd_MEMpd:
            INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)wrap<double, OPERATION_IMPL::mul_double>::apply<2>,
              IARG_REG_CONST_REFERENCE, INS_OperandReg(ins, 0),
              IARG_MEMORYREAD_EA,
              IARG_REG_REFERENCE, INS_OperandReg(ins, 0),
              IARG_ADDRINT, nullptr,
              IARG_END);
            INS_Delete(ins);
            break;


          case xed_iform_enum_t::XED_IFORM_DIVPD_XMMpd_XMMpd:
            INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)wrap<double, OPERATION_IMPL::div_double>::apply<2>,
              IARG_REG_CONST_REFERENCE, INS_OperandReg(ins, 0),
              IARG_REG_CONST_REFERENCE, INS_OperandReg(ins, 1),
              IARG_REG_REFERENCE, INS_OperandReg(ins, 0),
              IARG_ADDRINT, nullptr,
              IARG_END);
            INS_Delete(ins);
            break;
          case xed_iform_enum_t::XED_IFORM_DIVPD_XMMpd_MEMpd:
            INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)wrap<double, OPERATION_IMPL::div_double>::apply<2>,
              IARG_REG_CONST_REFERENCE, INS_OperandReg(ins, 0),
              IARG_MEMORYREAD_EA,
              IARG_REG_REFERENCE, INS_OperandReg(ins, 0),
              IARG_ADDRINT, nullptr,
              IARG_END);
            INS_Delete(ins);
            break;

          default:
            break;
          }
        }

      }
    }
  }
}