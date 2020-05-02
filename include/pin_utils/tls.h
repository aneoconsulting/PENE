#pragma once

#include <pin.h>
#include <functional>

namespace pene
{
  namespace pin_utils {
    template<class T>
    class tls
    {
    public:
      class life_cycle_manager {
      public:
        virtual T* create() = 0;
        virtual void destroy(T*) = 0;
      };

    protected:
      TLS_KEY key;
      life_cycle_manager& lcm;
    public:

      tls(life_cycle_manager& lcm)
        : key(PIN_CreateThreadDataKey(NULL))
        , lcm(lcm)
      {
        key = PIN_CreateThreadDataKey(NULL);
        if (key == INVALID_TLS_KEY)
        {
          std::cerr << "number of already allocated keys reached the MAX_CLIENT_TLS_KEYS limit(" << MAX_CLIENT_TLS_KEYS << ")" << std::endl;
          PIN_WriteErrorMessage("number of already allocated keys reached the MAX_CLIENT_TLS_KEYS limit", 1000, PIN_ERR_SEVERITY_TYPE::PIN_ERR_FATAL, 0);
          PIN_ExitProcess(1);
        }
        PIN_AddThreadStartFunction(
          [](THREADID threadid, CONTEXT* ctx, INT32 flags, VOID* voided_this) {reinterpret_cast<tls*>(voided_this)->ThreadStart(threadid, ctx, flags); }, this);
        PIN_AddThreadFiniFunction(
          [](THREADID threadid, const CONTEXT* ctx, INT32 flags, VOID* voided_this) {reinterpret_cast<tls*>(voided_this)->ThreadFini(threadid, ctx, flags); }, this);
      }

      bool is_tls_valid() const
      {
        return key != INVALID_TLS_KEY;
      }

      TLS_KEY get_key() const
      {
        return key;
      }

    protected:
      virtual T* ThreadStart(THREADID threadid, CONTEXT*, INT32)
      {
        if (key != INVALID_TLS_KEY)
        {
          T* tdata = lcm.create();
          if (PIN_SetThreadData(key, tdata, threadid) == FALSE)
          {
            std::cerr << "PIN_SetThreadData failed" << std::endl;
            PIN_WriteErrorMessage("PIN_SetThreadData failed", 1000, PIN_ERR_SEVERITY_TYPE::PIN_ERR_FATAL, 0);
            PIN_ExitProcess(1);
          }
          return tdata;
        }
        return nullptr;
      }

      virtual VOID ThreadFini(THREADID threadid, const CONTEXT*, INT32)
      {
        if (key != INVALID_TLS_KEY)
        {
          auto tls_val = reinterpret_cast<T*>(PIN_GetThreadData(key, threadid));
          lcm.destroy(tls_val);
        }
      }
    };
  }
}