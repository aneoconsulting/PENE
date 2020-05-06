#pragma once

#include <pin.H>

namespace pene {
  namespace pin_utils {

    class base_instrumenter
    {
    public:
      virtual void INS_AddInstrumentFunction() = 0;
      virtual void TRACE_AddInstrumentFunction() = 0;
    };

    template <class BASE_INSTRUMENTER>
    class instrumenter : public base_instrumenter
    {
    public:
      using base = BASE_INSTRUMENTER;

      void instrument_callback(TRACE trace_)
      {
        init_instrument_(trace_);
        for (auto bbl_ = TRACE_BblHead(trace_); BBL_Valid(bbl_); bbl_ = BBL_Next(bbl_))
        {
          init_instrument_(bbl);
          for (auto ins_ = BBL_InsHead(bbl_); INS_Valid(ins_); ins_ = INS_Next(ins_))
          {
            instrument_(ins_);
          }
          end_instrument_(bbl_);
        }
        end_instrument_(trace_);
      }

      void instrument_callback(INS ins_)
      {
        init_instrument_(nullptr);
        init_instrument_(MAKE_BBL(0));
        instrument_(ins_);
        end_instrument_(MAKE_BBL(0));
        end_instrument_(nullptr);
      }

      virtual void INS_AddInstrumentFunction() override
      {
        ::INS_AddInstrumentFunction([](INS ins_, void* voided_instrumenter) { reinterpret_cast<instrumenter*>(voided_instrumenter)->instrument_callback(ins_); }, this);
      }

      virtual void TRACE_AddInstrumentFunction() override
      {
        ::TRACE_AddInstrumentFunction([](TRACE trace_, void* voided_instrumenter) { reinterpret_cast<instrumenter*>(voided_instrumenter)->instrument_callback(trace_); }, this);
      }

    protected:
      base& derived()
      { 
        return static_cast<base&>(*this); 
      }

      TRACE trace;
      BBL bbl;
      INS ins;

      instrumenter()
        : trace(nullptr)
        , bbl(MAKE_BBL(0))
        , ins(MAKE_INS(0))
      {}

      template<class ... Args>
      void dispatch_insertCall_bbl_ins(Args... args)
      {
        if (trace != nullptr)
        {
          insertCall(trace, IPOINT_ANYWHERE, args...);
        }
        else if (bbl.is_valid())
        {
          insertCall(bbl, IPOINT_ANYWHERE, args...);
        }
        else
        {
          insertCall(ins, IPOINT_BEFORE, args...);
        }
      }

      void init_instrument(TRACE trace_) {  }
      void init_instrument(BBL bbl_) {  }
      void instrument(INS ins_) {  }
      void end_instrument(BBL bbl_) {  }
      void end_instrument(TRACE trace_) {  }

    private:
      void init_instrument_(TRACE trace_) { derived().init_instrument(trace_); }
      void init_instrument_(BBL bbl_) { derived().init_instrument(bbl_); }
      void instrument_(INS ins_) { derived().instrument(ins_); }
      void end_instrument_(BBL bbl_) { derived().end_instrument(bbl_); }
      void end_instrument_(TRACE trace_) { derived().end_instrument(trace_); }
    };
  }
}
