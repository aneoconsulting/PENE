#pragma once

#include "tls.h"

namespace pene {

  namespace pin_utils {
    template<class T>
    class tls_reg : public tls<T>
    {
      REG reg;
    public:

      tls_reg(typename life_cycle_manager& lcm)
        : tls<T>(lcm)
        , reg(PIN_ClaimToolRegister())
      {
        reg = PIN_ClaimToolRegister();
        if (!REG_valid(reg) || !REG_valid_for_iarg_reg_value(reg))
        {
          std::cerr << "Cannot allocate tls register" << std::endl;
          PIN_WriteErrorMessage("Cannot allocate tls register", 1000, PIN_ERR_SEVERITY_TYPE::PIN_ERR_FATAL, 0);
          PIN_ExitProcess(1);
        }
      }

      REG get_reg() const
      {
        return reg;
      }

    private:

    protected:
      virtual T* ThreadStart(THREADID threadid, CONTEXT* ctx, INT32 flags) override
      {S
        auto data = tls<T>::ThreadStart(threadid, ctx, flags);
        if (data)
        {
          PIN_SetContextReg(ctx, reg, (ADDRINT)data);
        }
        return data;
      }
    };
  }
}
