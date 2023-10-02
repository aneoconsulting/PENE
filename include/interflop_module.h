#pragma once

#include <pin.H>

#include "module.h"
#include "pin_utils/instrumenter.h"

extern "C"{
#include "interflop.h"
#include "interflop_stdlib.h"
#include "iostream/logger.h"
}

#define MAX_BACKENDS 16
#define MAX_ARGS 256

namespace pene {
  class interflop_module : public module
  {
    KNOB<bool> KnobInterflop;
    bool silent_load;
    int loaded_backends;
    interflop_backend_interface_t backends[MAX_BACKENDS];
    void * contexts[MAX_BACKENDS];
    pin_utils::instrumenter* data;

  public:
    static const char *backend_header;
    static const char vfc_backends_logfile[];// = "VFC_BACKENDS_LOGFILE";
    static IBool logger_enabled;
    static IBool logger_colored;
    static File *logger_logfile;
    static File *logger_stderr;
    interflop_module();
    void init() override;
    // virtual bool validate() override;
    const std::string& name() override;
    pid_t get_tid();

    void test();
    long pene_strtol(const char *nptr, char **endptr, int *error);
    double pene_strtod(const char *nptr, char **endptr, int *error);
    void pene_inf_handler(void);
    void pene_nan_handler(void);
    void pene_cancellation_handler(int unused);
    void pene_denormal_handler(void);
    void pene_floatmax_handler(void);
    void pene_set_handlers(const char *token, void *handle);
    void * load_function(const char *token, void *handle, const char *function);
    void set_handlers( char  * token, char * name);
    static void panic (const char *msg);
    static void pene_verrou_panic (const char *msg);
    void set_logger_logfile();

    static void _interflop_err(int eval, const char *fmt, ...);    
  private:
  };
}