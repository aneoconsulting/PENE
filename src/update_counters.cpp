#include "counters_utils.h"

namespace pene
{
  bool update_counters(OPCODE oc, counters& counters)
  {
    switch (oc)
    {
    case XED_ICLASS_ADDSD:
    case XED_ICLASS_SUBSD:
    case XED_ICLASS_VADDSD:
    case XED_ICLASS_VSUBSD:
      counters[add_double_scalar] += 1;
      return true;
    case XED_ICLASS_ADDPD:
    case XED_ICLASS_SUBPD:
    case XED_ICLASS_VADDPD:
    case XED_ICLASS_VSUBPD:
      counters[add_double_simd] += 1;
      return true;
    case XED_ICLASS_ADDSS:
    case XED_ICLASS_SUBSS:
    case XED_ICLASS_VADDSS:
    case XED_ICLASS_VSUBSS:
      counters[add_float_scalar] += 1;
      return true;
    case XED_ICLASS_ADDPS:
    case XED_ICLASS_SUBPS:
    case XED_ICLASS_VADDPS:
    case XED_ICLASS_VSUBPS:
      counters[add_float_simd] += 1;
      return true;
    case XED_ICLASS_DIVSD:
    case XED_ICLASS_VDIVSD:
      counters[div_double_scalar] += 1;
      return true;
    case XED_ICLASS_DIVPD:
    case XED_ICLASS_VDIVPD:
      counters[div_double_simd] += 1;
      return true;
    case XED_ICLASS_DIVSS:
    case XED_ICLASS_VDIVSS:
      counters[div_float_scalar] += 1;
      return true;
    case XED_ICLASS_DIVPS:
    case XED_ICLASS_VDIVPS:
      counters[div_float_simd] += 1;
      return true;
    case XED_ICLASS_MULSS:
    case XED_ICLASS_VMULSS:
      counters[mul_float_scalar] += 1;
      return true;
    case XED_ICLASS_MULPS:
    case XED_ICLASS_VMULPS:
      counters[mul_float_simd] += 1;
      return true;
    case XED_ICLASS_MULSD:
    case XED_ICLASS_VMULSD:
      counters[mul_double_scalar] += 1;
      return true;
    case XED_ICLASS_MULPD:
    case XED_ICLASS_VMULPD:
      counters[mul_double_simd] += 1;
      return true;
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
      counters[fma_float_scalar] += 1;
      return true;
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
      counters[fma_double_scalar] += 1;
      return true;
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
      counters[fma_float_simd] += 1;
      return true;
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
      counters[fma_double_simd] += 1;
      return true;
    default:
      return false;
    }
  }



}