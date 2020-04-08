#pragma once
#include "../includes/defs.h"
#include <iostream>


namespace interflop
{
  namespace internals
  {

    template <typename real_t, void (* func)(real_t, real_t, real_t *), 
      bool scalar, int instr_width, int register_width = instr_width, bool no_mask = true, bool is_zeroing = true>
    VOID simd_op(const PIN_REGISTER* ra, const PIN_REGISTER* rb, PIN_REGISTER* rc, const PIN_REGISTER* rk)
    {
      constexpr int instr_iterations = instr_width / sizeof(real_t);
      constexpr int reg_iterations = register_width / sizeof(real_t);
      for (int i = 0; i < instr_iterations; ++i)
      {
        real_t res;
        if (i > 0 && scalar)
        {
          res = ((real_t*)ra->byte)[i];
        }
        else
        {
          func(((real_t*)ra->byte)[i], ((real_t*)rb->byte)[i], &res);
        }

        if constexpr (no_mask)
        {
          ((real_t*)rc->byte)[i] = res;
        }
        else
        {
          UINT64 mask = rk->qword[0];
          if (mask & 1 || (i > 0 && scalar))
          {
            ((real_t*)rc->byte)[i] = res;
          }
          else if constexpr (is_zeroing)
          {
            ((real_t*)rc->byte)[i] = real_t{ 0 };
          }
        }
      }
      for (int i = instr_iterations; i < reg_iterations; ++i)
      {
        ((real_t*)rc->byte)[i] = real_t{ 0 };
      }
    }


  }
}
