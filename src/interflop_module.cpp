
extern "C"
{
  // #include "argp/argp.h"
  #include <assert.h>
  #include <dlfcn.h>
  #include <err.h>
  #include <errno.h>
  #include <fcntl.h>
  #include <math.h>
  //#include <printf.h>
  #include <stdbool.h>
  #include <stdio.h>
  #include <stdlib.h>
  #include <string.h>
  #include <strings.h>
  #include <sys/stat.h>
  #include <sys/syscall.h>
  #include <sys/time.h>
  #include <sys/types.h>
  #include <sys/wait.h>
  #include <unistd.h>
}

#include <iostream>
#include "interflop_module.h"


#include "pin_utils/symbol_filters.h"

#define MAX_BACKENDS 16
#define MAX_ARGS 256

typedef enum
{
  ADD_1,
  ADD_4,
  ADD_8,
  ADD_16,
  SUB_1,
  SUB_4,
  SUB_8,
  SUB_16,
  MUL_1,
  MUL_4,
  MUL_8,
  MUL_16,
  DIV_1,
  DIV_4,
  DIV_8,
  DIV_16
} op_t;

#include "replace/wrappers/vfc_sse.h"
#include "replace/wrappers/vfc_avx.h"

/* Parse the different VFC_BACKENDS variables per priorty order */
/* 1- VFC_BACKENDS */
/* 2- VFC_BACKENDS_FROM_FILE */
/* Set the backends read in vfc_backends */
/* Set the name of the environment variable read in vfc_backends_env */
void parse_vfc_backends_env(char **vfc_backends, char **vfc_backends_env) {

  /* Parse VFC_BACKENDS */
  *vfc_backends_env = (char *)malloc(sizeof(char) * 256);
  *vfc_backends = (char *)malloc(sizeof(char) * 256);

  sprintf(*vfc_backends_env, "VFC_BACKENDS");
  *vfc_backends = getenv(*vfc_backends_env);

  /* Parse VFC_BACKENDS_FROM_FILE if VFC_BACKENDS is empty*/
  if (*vfc_backends == NULL) {
    sprintf(*vfc_backends_env, "VFC_BACKENDS_FROM_FILE");
    char *vfc_backends_fromfile_file = getenv(*vfc_backends_env);
    if (vfc_backends_fromfile_file != NULL) {
      FILE *fi = fopen(vfc_backends_fromfile_file, "r");
      if (fi == NULL) {
        std::cerr << "Error while opening file pointed by " <<*vfc_backends_env << " " << strerror(errno) << std::endl;
      } else {
        size_t len = 0;
        ssize_t nread;
        nread = getline(vfc_backends, &len, fi);
        if (nread == -1) {
          std::cerr << "Error while reading file pointed by " << *vfc_backends_env << " " << strerror(errno) << std::endl;
        } else {
          if ((*vfc_backends)[nread - 1] == '\n') {
            (*vfc_backends)[nread - 1] = '\0';
          }
        }
      }
    }
  }
}

typedef struct interflop_backend_interface_t (*interflop_init_t)(void *context);

typedef void (*interflop_pre_init_t)(interflop_panic_t panic, FILE *stream, 
                                     void **context);
typedef void (*interflop_cli_t)(int argc, char **argv, void *context);
typedef void (*interflop_panic_t)(const char *msg);

namespace pene {
  using namespace pin_utils;
  
  
  namespace replace_module_internals
  {
    class dynamic_backend_instrumenters final : public element_instrumenter
    {
      REG tmp_reg1;
      REG tmp_reg2;
      REG tmp_reg_output;
  
    public:
      int loaded_backends;
      interflop_backend_interface_t backends[MAX_BACKENDS];
      void * contexts[MAX_BACKENDS];
      void* backend_ctx;
      dynamic_backend_instrumenters()
        : element_instrumenter()
        , tmp_reg1(PIN_ClaimToolRegister())
        , tmp_reg2(PIN_ClaimToolRegister())
        , tmp_reg_output(PIN_ClaimToolRegister())
        //, backend_ctx(OPERATION_IMPL::init())
      {
           //struct pene::replace::backend::verrou ifverrou=pene::replace::backend::init(&backend_ctx);
        if (!REG_valid(tmp_reg1) || !REG_valid(tmp_reg2) || !REG_valid(tmp_reg_output))
        {
          std::cerr << "Cannot allocate a scratch register.\n";
          std::cerr << std::flush;
          PIN_ExitApplication(1);
        }
      }
      void set_loaded_backends (int& _loaded_backends)
      {
        loaded_backends = _loaded_backends;
      }
      int& get_loaded_backends ()
      {
        return loaded_backends;
      }
      void set_backends_interface (interflop_backend_interface_t _backends[])
      {
      }
      interflop_backend_interface_t* get_backends_interface ()
      {
        return backends;
      }
      void set_backends_contexts (void * _contexts)
      {
      }
      void** get_backends_contexts ()
      {
        return contexts;
      }

      virtual void instrument(INS ins) override {
        switch (INS_Category(ins))
        {
        case xed_category_enum_t::XED_CATEGORY_SSE:
          replace::vfc_wrappers::sse::instrument(loaded_backends, backends, contexts, ins);
          break;
        case xed_category_enum_t::XED_CATEGORY_AVX:
          replace::vfc_wrappers::avx::instrument(loaded_backends, backends, contexts, ins);
          break;
        // case xed_category_enum_t::XED_CATEGORY_AVX512:
        //   replace::wrappers::avx512::instrument<OPERATION_IMPL>(backend_ctx, ins);
        //   break;
        // case xed_category_enum_t::XED_CATEGORY_VFMA:
        //   replace::wrappers::fma::instrument<OPERATION_IMPL>(backend_ctx, ins);
        //   break;
        default:
          break;
        }
      }
    };
  }

/* Logger functions */
  const char *interflop_module::backend_header = NULL;
  const char interflop_module::vfc_backends_logfile[] = "VFC_BACKENDS_LOGFILE";
  IBool interflop_module::logger_enabled = ITrue;
  IBool interflop_module::logger_colored = IFalse;
  File *interflop_module::logger_logfile = Null;
  File *interflop_module::logger_stderr = Null;

  interflop_module::interflop_module() : module(), 
    KnobInterflop(KNOB_MODE_WRITEONCE, "pintool", "interflop", "0", "use interflop dynamic backend (should not be used if using -fp-replace)"),
    loaded_backends(0),
    silent_load(false),
    data(nullptr)
  {
    // test();
  }

  void interflop_module::test()
  {
    std::cerr << "Test : " << std::endl;
    char *token = "libinterflop_verrou.so";
    void *handle = dlopen("/home/akli_h/Self/PENE/build/extern/libinterflop_verrou.so", RTLD_NOW);

    std::cerr << "---- load pre_init handler " << std::endl;
    interflop_pre_init_t handle_pre_init = (interflop_pre_init_t)load_function( token, handle, "interflop_pre_init");
    
    std::cerr << "---- load init handler " << std::endl;
    interflop_init_t handle_init         = (interflop_init_t)    load_function( token, handle, "interflop_init");
    
    std::cerr << "---- set handlers " << std::endl;
    pene_set_handlers(token, handle);
    
    std::cerr << "---- init backend " << std::endl;
    void * context = NULL;
    handle_pre_init(interflop_module::panic, stderr, &context);
    interflop_backend_interface_t backends = handle_init(context);
    // float a = 0.1f, b = 0.2f, c = 0.0f;
    // fprintf (stderr, "Scalar : \n");
    // for (int i = 0; i < loaded_backends; i++) {
    //   backends[i].interflop_div_float(a, b, &c, contexts[i]);
    //   fprintf(stderr,  "----Res : %.23f\n", c);
    // }
    // fprintf (stderr, "Vector : \n");
    // float va[16] = {0.1f,0.03f,0.012f,0.1f, 0.001f, 0.002f, 0.003f, 0.004f, 0.005f, 0.006f, 0.007f, 0.008f, 0.009f, 0.010f, 0.011f, 0.012f},
    //       vb[16] = {0.2f,0.07f,-0.011f,0.2f,0.001f,0.001f,0.001f,0.001f,0.001f,0.001f,0.001f,0.001f,0.001f,0.001f,0.001f,0.001f},
    //       vc[16] = {0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f};
    // for (int i = 0; i < loaded_backends; i++) {
    //   backends[i].vbackend.vector128.div.op_vector_float_4(va, vb, vc, contexts[i]);
    //   fprintf(stderr,  "----Res 128 - 4:\n");
    //   for(int j = 0; j < 4; j++)
    //     fprintf (stderr, " %.23f ", vc[j]);
    //   fprintf (stderr, "\n");
    //   backends[i].vbackend.vector256.div.op_vector_float_8(va, vb, vc, contexts[i]);
    //   fprintf(stderr,  "----Res 256 - 8:\n");
    //   for(int j = 0; j < 8; j++)
    //     fprintf (stderr, " %.5f ", vc[j]);
    //   fprintf (stderr, "\n");
    //   backends[i].vbackend.vector512.div.op_vector_float_16(va, vb, vc, contexts[i]);
    //   fprintf(stderr,  "----Res 512 - 16:\n");
    //   for(int j = 0; j < 16; j++)
    //     fprintf (stderr, " %.5f ", vc[j]);
    //   fprintf (stderr, "\n");
    // }
    std::cerr << "---- backend loaded " << std::endl;
  }

  void interflop_module::init()
  {
    std::cerr << "Initialization: interflop module." << std::endl;
    if (!KnobInterflop.Value())
    {
      std::cerr << "Interflop verificarlo's dynamic backends : disabled" << std::endl;
      return;
    }
    /* function required by vfcwrapper.c */
    interflop_set_handler("getenv", (void *)getenv);
    interflop_set_handler("sprintf", (void *)sprintf);
    interflop_set_handler("strerror", (void *)strerror);
    interflop_set_handler("gettid", (void*) &interflop_module::get_tid);
    interflop_set_handler("fopen", (void *)fopen);
    interflop_set_handler("strcasecmp", (void *)strcasecmp);
    interflop_set_handler("vwarnx", (void *)vwarnx);
    interflop_set_handler("fprintf", (void *)fprintf);
    interflop_set_handler("exit", (void *)exit);
    interflop_set_handler("vfprintf", (void *)vfprintf);
    interflop_set_handler("malloc", (void *)malloc);
    interflop_set_handler("strcmp", (void *)strcmp);
    interflop_set_handler("strtol", (void *) &interflop_module::pene_strtol);
    interflop_set_handler("strtod", (void *) &interflop_module::pene_strtod);
    // interflop_set_handler("strtol", (void *)_vfc_strtol);
    // interflop_set_handler("strtod", (void *)_vfc_strtod);
    interflop_set_handler("strcpy", (void *)strcpy);
    interflop_set_handler("fclose", (void *)fclose);
    interflop_set_handler("fgets", (void *)fgets);
    interflop_set_handler("strtok_r", (void *)strtok_r);
    interflop_set_handler("free", (void *)free);
    interflop_set_handler("calloc", (void *)calloc);
    interflop_set_handler("gettimeofday", (void *)gettimeofday);
    // interflop_set_handler("register_printf_specifier", (void *)register_printf_specifier);

    /* Initialize the logger */
    logger_init(&interflop_module::panic, stderr, "pene");    
    char *vfc_backends = NULL, *vfc_backends_env = NULL;
    parse_vfc_backends_env(&vfc_backends, &vfc_backends_env);

    if (vfc_backends == NULL) {
    logger_error("At least one backend should be provided "
                 "by defining VFC_BACKENDS or VFC_BACKENDS_FROM_FILE "
                 "environment variables",
                 vfc_backends_env);
    }
    /* For each backend, load and register the backend vtable interface
      Backends .so are separated by semi-colons in the VFC_BACKENDS
      env variable */
    char *semicolonptr;
    char *token = strtok_r(vfc_backends, ";", &semicolonptr);
  
    char *interflop_prefix = getenv("INTERFLOP_LIBRARY_PATH");

    filter* filter = new null_filter();
    replace_module_internals::dynamic_backend_instrumenters * dyn_backends = new replace_module_internals::dynamic_backend_instrumenters();

    interflop_backend_interface_t * _backends = dyn_backends->get_backends_interface();
    void ** _contexts = dyn_backends->get_backends_contexts();
    while (token) {
      /* Parse each backend arguments, argv[0] is the backend name */
      int backend_argc = 0;
      char *backend_argv[MAX_ARGS];
      char *spaceptr;
      char *arg = strtok_r(token, " ", &spaceptr);
      while (arg) {
        if (backend_argc >= MAX_ARGS) {
          logger_error("%s syntax error: too many arguments", vfc_backends_env);
        }
        backend_argv[backend_argc++] = arg;
        arg = strtok_r(NULL, " ", &spaceptr);
      }
      backend_argv[backend_argc] = NULL;

      std::string backend_path = std::string(interflop_prefix) + "/" + std::string(backend_argv[0]);
      /* load the backend .so */
      void *handle = dlopen(backend_path.c_str(), RTLD_NOW);
      if (handle == NULL) {
        logger_error("Cannot load backend %s: dlopen error\n%s", token,
                    dlerror());
      }
      if (!silent_load)
        logger_info("loaded backend %s\n", token);

      /* get the addresses of the interflop_init, interflop_pre_init, and
      * interflop_cli functions */
      interflop_pre_init_t handle_pre_init = (interflop_pre_init_t)load_function(
          token, handle, "interflop_pre_init");
      interflop_cli_t handle_cli =
          (interflop_cli_t)load_function(token, handle, "interflop_cli");
      interflop_init_t handle_init =
          (interflop_init_t)load_function(token, handle, "interflop_init");

      pene_set_handlers(token, handle);
      /* Register backend */
      if (loaded_backends == MAX_BACKENDS) {
        logger_error("No more than %d backends can be used simultaneously",
                    MAX_BACKENDS);
      }
      handle_pre_init(&interflop_module::panic, stderr, &_contexts[loaded_backends]);
      // handle_cli(backend_argc, backend_argv, contexts[loaded_backends]);
      _backends[loaded_backends] = handle_init(_contexts[loaded_backends]);
      loaded_backends++;

      /* parse next backend token */
      token = strtok_r(NULL, ";", &semicolonptr);
    }
    dyn_backends->set_loaded_backends(loaded_backends);
    if (loaded_backends == 0) {
      logger_error("%s syntax error: at least one backend should be provided",
                  vfc_backends_env);
    }

    float a = 0.1f, b = 0.2f, c = 0.0f;
    fprintf (stderr, "Scalar : \n");
    for (int i = 0; i < loaded_backends; i++) {
      _backends[i].interflop_add_float(a, b, &c, _contexts[i]);
      fprintf(stderr,  "----Res : %.23f\n", c);
    }
    fprintf (stderr, "Vector : \n");
    float va[16] = {0.1f,0.03f,0.012f,0.1f, 0.001f, 0.002f, 0.003f, 0.004f, 0.005f, 0.006f, 0.007f, 0.008f, 0.009f, 0.010f, 0.011f, 0.012f},
          vb[16] = {0.2f,0.07f,-0.011f,0.2f,0.001f,0.001f,0.001f,0.001f,0.001f,0.001f,0.001f,0.001f,0.001f,0.001f,0.001f,0.001f},
          vc[16] = {0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f};
    for (int i = 0; i < loaded_backends; i++) {
      fprintf(stderr,  "----func : %p \n", _backends[i].vbackend.scalar.add.op_vector_float_4);
      _backends[i].vbackend.scalar.add.op_vector_float_4(va, vb, vc, _contexts[i]);
      fprintf(stderr,  "----Res 128 - 4:\n");
      for(int j = 0; j < 4; j++)
        fprintf (stderr, " %.23f ", vc[j]);
      fprintf (stderr, "\n");
    }
    data = new instrumenter(dyn_backends, filter);
    data->TRACE_AddInstrumentFunction();
  }

  pid_t interflop_module::get_tid()
  {
    return syscall(__NR_gettid);
  }
  void interflop_module::panic (const char *msg)
  {
    std::cerr << msg;
    // exit(1);
  }
  long interflop_module::pene_strtol(const char *nptr, char **endptr, int *error) {
    *error = 0;
    errno = 0;
    long val = strtoll(nptr, endptr, 10);
    if (errno != 0) {
      *error = 1;
    }
    return val;
  }

  double interflop_module::pene_strtod(const char *nptr, char **endptr, int *error) {
    *error = 0;
    errno = 0;
    double val = strtod(nptr, endptr);
    if (errno != 0) {
      *error = 1;
    }
    return val;
  }
  void interflop_module::pene_inf_handler(void) {}

  void interflop_module::pene_nan_handler(void) {}

  void interflop_module::pene_cancellation_handler(int unused) {}

  void interflop_module::pene_denormal_handler(void) {}

  void interflop_module::pene_floatmax_handler(void) {}

  void interflop_module::pene_verrou_panic (const char *msg)
  {
    std::cerr << msg;
    exit(1);
  }

  /* Load the function <function> in <handle> .so of name <token> */
  void *interflop_module::load_function(const char *token, void *handle, const char *function) {
    /* reset dl errors */
    dlerror();
    /* get the address of the interflop_pre_init function */
    void *handler = dlsym(handle, function);
    const char *dlsym_error = dlerror();
    if (dlsym_error) {
      fprintf( stderr, "No %s function in backend %s: %s", function, token,
                  strerror(errno));
    }
    /* reset dl errors */
    dlerror();
    return handler;
  }

  void interflop_module::pene_set_handlers(const char *token, void *handle) {

    interflop_set_handler_t set_handler = (interflop_set_handler_t)load_function(
        token, handle, "interflop_set_handler");
    set_handler("getenv",(void *) getenv);
    set_handler("sprintf", (void*) sprintf);
    set_handler("strerror", (void*) strerror);
    set_handler("gettid", (void*) &interflop_module::get_tid);
    set_handler("fopen", (void*) fopen);
    set_handler("strcasecmp", (void*) strcasecmp);
    set_handler("vwarnx", (void*) vwarnx);
    set_handler("fprintf", (void*) fprintf);
    set_handler("exit", (void*) exit);
    set_handler("vfprintf", (void*) vfprintf);
    set_handler("malloc", (void*) malloc);
    set_handler("strcmp", (void*) strcmp);
    set_handler("strtol", (void*) &interflop_module::pene_strtol);
    set_handler("strtod", (void*) &interflop_module::pene_strtod);
    set_handler("strcpy", (void*) strcpy);
    set_handler("fclose", (void*) fclose);
    set_handler("fgets", (void*) fgets);
    set_handler("strtok_r", (void*) strtok_r);
    set_handler("free", (void*) free);
    set_handler("calloc", (void*) calloc);
    // set_handler("argp_parse", (void*) argp_parse);
    set_handler("gettimeofday", (void*) gettimeofday);
    // set_handler("register_printf_specifier", (void*) register_printf_specifier);
    set_handler("infHandler", (void*) &interflop_module::pene_inf_handler);
    set_handler("nanHandler", (void*) &interflop_module::pene_nan_handler);
    set_handler("cancellationHandler", (void*) &interflop_module::pene_cancellation_handler);
    set_handler("denormalHandler", (void*) &interflop_module::pene_denormal_handler);
    set_handler("maxHandler", (void*) &interflop_module::pene_floatmax_handler);
  }

  void interflop_module::set_handlers( char  * token, char * name)
  {
  }

  const std::string& interflop_module::name() 
  {
    static const std::string name_("interflop_module");
    return name_;
  }

}

