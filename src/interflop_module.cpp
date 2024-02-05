#if defined(_WIN32)

#include <iostream>
#include "interflop_module.h"

namespace pene {
  using namespace pin_utils;
  interflop_module::interflop_module() : module(), 
    KnobInterflop(KNOB_MODE_WRITEONCE, "pintool", "interflop", "0", "use interflop dynamic backend (should not be used on windows target)"),
    KnobVectorInterflop(KNOB_MODE_WRITEONCE, "pintool", "vector-mode", "0", "use interflop dynamic backend (should not be used on windows target)"),
    data(nullptr)
  {}

  void interflop_module::init()
  {
    std::cerr << "Initialization: interflop module." << std::endl;
    std::cerr << "  Interflop verificarlo's dynamic backends : " << "disabled" << std::endl;
    std::cerr << "  Backends working mode : none" << std::endl;
    std::cerr << "  /!\\ Feature not supported on Windows" << std::endl;
  }

  const std::string& interflop_module::name() 
  {
    static const std::string name_("interflop_module");
    return name_;
  }
}

#else
extern "C"
{
  #include "argp/argp.h"
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

extern "C"{
#include "interflop.h"
#include "interflop_stdlib.h"
#include "iostream/logger.h"
}
// Part of Verificarlo project
// Taken from
// https://github.com/interflop/verificarlo/blob/master/src/vfcwrapper/main.c.in


#include "interflop/interflop.h"
#include "interflop/interflop_stdlib.h"

/* In delta-debug we retrieve the return address of
 * instrumented operations. Call op size allows us
 * to compute the previous instruction so that the
 * user sees the address of the actual operation */
#ifdef __x86_64__
#define CALL_OP_SIZE 5
#else
/* On other architectures we assume an instruction is
 * 4 bytes */
#define CALL_OP_SIZE 4
#endif

// #if @LLVM_VERSION_MAJOR@ <= 7
// #define ADDR2LINE_BIN "addr2line"
// #define ADDR2LINE_PATH "/usr/bin/addr2line"
// #else
// #define ADDR2LINE_BIN "@LLVM_BINDIR@/llvm-addr2line"
// #define ADDR2LINE_PATH "@LLVM_BINDIR@/llvm-addr2line"
// #endif

// typedef int int2 __attribute__((ext_vector_type(2)));
// typedef int int4 __attribute__((ext_vector_type(4)));
// typedef int int8 __attribute__((ext_vector_type(8)));
// typedef int int16 __attribute__((ext_vector_type(16)));

// typedef float float2 __attribute__((ext_vector_type(2)));
// typedef float float4 __attribute__((ext_vector_type(4)));
// typedef float float8 __attribute__((ext_vector_type(8)));
// typedef float float16 __attribute__((ext_vector_type(16)));

// typedef double double2 __attribute__((ext_vector_type(2)));
// typedef double double4 __attribute__((ext_vector_type(4)));
// typedef double double8 __attribute__((ext_vector_type(8)));
// typedef double double16 __attribute__((ext_vector_type(16)));

typedef struct interflop_backend_interface_t (*interflop_init_t)(void *context);

typedef void (*interflop_pre_init_t)(interflop_panic_t panic, FILE *stream, 
                                     void **context);
typedef void (*interflop_cli_t)(int argc, char **argv, void *context);
typedef void (*interflop_panic_t)(const char *msg);

#define MAX_BACKENDS 16
#define MAX_ARGS 256

#define XSTR(X) STR(X)
#define STR(X) #X

struct interflop_backend_interface_t backends[MAX_BACKENDS];
void *contexts[MAX_BACKENDS] = {NULL};
unsigned char loaded_backends = 0;
unsigned char already_initialized = 0;

/* Logger functions */

void logger_init(interflop_panic_t panic, File *stream, const char *name);
void logger_info(const char *fmt, ...);
void logger_warning(const char *fmt, ...);
void logger_error(const char *fmt, ...);

__attribute__((unused)) static char *dd_exclude_path = NULL;
__attribute__((unused)) static char *dd_include_path = NULL;
__attribute__((unused)) static char *dd_generate_path = NULL;

/* Function instrumentation prototypes */

void vfc_init_func_inst();

void vfc_quit_func_inst();

/* Hashmap header */

#define __VFC_HASHMAP_HEADER__

struct vfc_hashmap_st {
  size_t nbits;
  size_t mask;

  size_t capacity;
  size_t *items;
  size_t nitems;
  size_t n_deleted_items;
};

typedef struct vfc_hashmap_st *vfc_hashmap_t;
// allocate and initialize the map
vfc_hashmap_t vfc_hashmap_create();

// free the map
void vfc_hashmap_destroy(vfc_hashmap_t map);

// get the value at an index of a map
size_t get_value_at(size_t *items, size_t i);

// get the key at an index of a map
size_t get_key_at(size_t *items, size_t i);

// set the value at an index of a map
void set_value_at(size_t *items, size_t value, size_t i);

// set the key at an index of a map
void set_key_at(size_t *items, size_t key, size_t i);

// insert an element in the map
void vfc_hashmap_insert(vfc_hashmap_t map, size_t key, void *item);

// remove an element of the map
void vfc_hashmap_remove(vfc_hashmap_t map, size_t key);

// test if an element is in the map
char vfc_hashmap_have(vfc_hashmap_t map, size_t key);

// get an element of the map
void *vfc_hashmap_get(vfc_hashmap_t map, size_t key);

// get the number of elements in the map
size_t vfc_hashmap_num_items(vfc_hashmap_t map);

// Hash function for strings
size_t vfc_hashmap_str_function(const char *id);

// Free the hashmap
void vfc_hashmap_free(vfc_hashmap_t map);

/* dd_must_instrument is used to apply and generate include DD filters */
/* dd_mustnot_instrument is used to apply exclude DD filters */
vfc_hashmap_t dd_must_instrument;
vfc_hashmap_t dd_mustnot_instrument;

/*
void ddebug_generate_inclusion(char *dd_generate_path, vfc_hashmap_t map) {
  int output = open(dd_generate_path, O_WRONLY | O_CREAT, S_IWUSR | S_IRUSR);
  if (output == -1) {
    logger_error("cannot open DDEBUG_GEN file %s", dd_generate_path);
  }
  for (size_t i = 0; i < map->capacity; i++) {
    if (get_value_at(map->items, i) != 0 && get_value_at(map->items, i) != 1) {
      pid_t pid = fork();
      if (pid == 0) {
        char addr[19];
        char executable[64];
        snprintf(addr, 19, "%p",
                 (void *)(get_value_at(map->items, i) - CALL_OP_SIZE));
        snprintf(executable, 64, "/proc/%d/exe", getppid());
        dup2(output, 1);
        execlp(ADDR2LINE_BIN, ADDR2LINE_PATH,
               "-fpaCs", "-e", executable, addr, NULL);
        logger_error("error running " ADDR2LINE_BIN);
      } else {
        int status;
        wait(&status);
        assert(status == 0);
      }
    }
  }
  close(output);
}
*/

__attribute__((destructor(0))) static void vfc_atexit(void) {

  /* Send finalize message to backends */
  for (int i = 0; i < loaded_backends; i++)
    if (backends[i].interflop_finalize)
      backends[i].interflop_finalize(contexts[i]);

#ifdef DDEBUG
  if (dd_generate_path) {
    ddebug_generate_inclusion(dd_generate_path, dd_must_instrument);
    logger_info("ddebug: generated complete inclusion file at %s\n",
                dd_generate_path);
  }
  vfc_hashmap_destroy(dd_must_instrument);
  vfc_hashmap_destroy(dd_mustnot_instrument);
#endif

#ifdef INST_FUNC
  vfc_quit_func_inst();
#endif
}

/* Checks that a least one of the loaded backend implements the chosen
 * operation at a given precision */
#define check_backends_implements(precision, operation)                        \
  do {                                                                         \
    int res = 0;                                                               \
    for (unsigned char i = 0; i < loaded_backends; i++) {                      \
      if (backends[i].interflop_##operation##_##precision) {                   \
        res = 1;                                                               \
        break;                                                                 \
      }                                                                        \
    }                                                                          \
    if (res == 0)                                                              \
      logger_error("No backend instruments " #operation " for " #precision     \
                   ".\n"                                                       \
                   "Include one backend in VFC_BACKENDS that provides it");    \
  } while (0)

#if DDEBUG
/* vfc_read_filter_file reads an inclusion/exclusion ddebug file and returns
 * an address map */
static void vfc_read_filter_file(const char *dd_filter_path,
                                 vfc_hashmap_t map) {
  FILE *input = fopen(dd_filter_path, "r");
  if (input) {
    void *addr;
    char line[2048];
    int lineno = 0;
    while (fgets(line, sizeof line, input)) {
      lineno++;
      if (sscanf(line, "%p", &addr) == 1) {
        vfc_hashmap_insert(map, (size_t)addr + CALL_OP_SIZE,
                           addr + CALL_OP_SIZE);
      } else {
        logger_error(
            "ddebug: error parsing VFC_DDEBUG_[INCLUDE/EXCLUDE] %s at line %d",
            dd_filter_path, lineno);
      }
    }
  }
}
#endif

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
        logger_error("Error while opening file pointed by %s: %s",
                     *vfc_backends_env, strerror(errno));
      } else {
        size_t len = 0;
        ssize_t nread;
        nread = getline(vfc_backends, &len, fi);
        if (nread == -1) {
          logger_error("Error while reading file pointed by %s: %s",
                       *vfc_backends_env, strerror(errno));
        } else {
          if ((*vfc_backends)[nread - 1] == '\n') {
            (*vfc_backends)[nread - 1] = '\0';
          }
        }
      }
    }
  }
}

long _vfc_strtol(const char *nptr, char **endptr, int *error) {
  *error = 0;
  errno = 0;
  long val = strtoll(nptr, endptr, 10);
  if (errno != 0) {
    *error = 1;
  }
  return val;
}

double _vfc_strtod(const char *nptr, char **endptr, int *error) {
  *error = 0;
  errno = 0;
  double val = strtod(nptr, endptr);
  if (errno != 0) {
    *error = 1;
  }
  return val;
}

/* Load the function <function> in <handle> .so of name <token> */
void *load_function(const char *token, void *handle, const char *function) {
  /* reset dl errors */
  dlerror();
  /* get the address of the interflop_pre_init function */
  void *handler = dlsym(handle, function);
  const char *dlsym_error = dlerror();
  if (dlsym_error) {
    logger_error("No %s function in backend %s: %s", function, token,
                 strerror(errno));
  }
  /* reset dl errors */
  dlerror();
  return handler;
}

pid_t get_tid() { return syscall(__NR_gettid); }

void _vfc_inf_handler(void) {

}

void _vfc_nan_handler(void) {

}

void _vfc_cancellation_handler(int unused) {

}

void _vfc_denormal_handler(void) {

}

void _vfc_floatmax_handler(void) {

}

void vfc_set_handlers(const char *token, void *handle) {

  interflop_set_handler_t set_handler = (interflop_set_handler_t)load_function(
      token, handle, "interflop_set_handler");
  set_handler("getenv", (void*) getenv);
  set_handler("sprintf", (void*) sprintf);
  set_handler("strerror", (void*) strerror);
  set_handler("gettid", (void*) get_tid);
  set_handler("fopen", (void*) fopen);
  set_handler("strcasecmp", (void*) strcasecmp);
  set_handler("vwarnx", (void*) vwarnx);
  set_handler("fprintf", (void*) fprintf);
  set_handler("exit", (void*) exit);
  set_handler("vfprintf", (void*) vfprintf);
  set_handler("malloc", (void*) malloc);
  set_handler("strcmp", (void*) strcmp);
  set_handler("strtol", (void*) _vfc_strtol);
  set_handler("strtod", (void*) _vfc_strtod);
  set_handler("strcpy", (void*) strcpy);
  set_handler("fclose", (void*) fclose);
  set_handler("fgets", (void*) fgets);
  set_handler("strtok_r", (void*) strtok_r);
  set_handler("free", (void*) free);
  set_handler("calloc", (void*) calloc);
  set_handler("argp_parse", (void*) argp_parse);
  set_handler("gettimeofday", (void*) gettimeofday);
  // set_handler("register_printf_specifier", (void*) register_printf_specifier);
  set_handler("infHandler", (void*) _vfc_inf_handler);
  set_handler("nanHandler", (void*) _vfc_nan_handler);
  set_handler("cancellationHandler", (void*) _vfc_cancellation_handler);
  set_handler("denormalHandler", (void*) _vfc_denormal_handler);
  set_handler("maxHandler", (void*) _vfc_floatmax_handler);
}

void _vfc_panic(const char *msg) {
  fprintf(stderr, "%s", msg);
  exit(1);
}

#include "pin_utils/symbol_filters.h"
typedef enum
{
  ADD_1=0,
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
#include "replace/wrappers/vfc_wrapper_sse.h"
#include "replace/wrappers/vfc_avx.h"

namespace pene {
  using namespace pin_utils;
  
  
  namespace replace_module_internals
  {
    template<bool mode>
    class dynamic_backend_instrumenters final : public element_instrumenter
    {
      REG tmp_reg1;
      REG tmp_reg2;
      REG tmp_reg_output;
  
    public:
      //~ int loaded_backends;
      //~ interflop_backend_interface_t backends[MAX_BACKENDS];
      //~ void * contexts[MAX_BACKENDS];
      //~ void* backend_ctx;
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

      virtual void instrument(INS ins) override {
        switch (INS_Category(ins))
        {
        case xed_category_enum_t::XED_CATEGORY_SSE:
          replace::vfc_wrappers::devectorized_sse::instrument(loaded_backends, backends, contexts, ins);
          break;
        //~ case xed_category_enum_t::XED_CATEGORY_AVX:
          //~ replace::vfc_wrappers::avx::instrument(loaded_backends, backends, contexts, ins);
          //~ break;
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
    
    template<>
    void dynamic_backend_instrumenters<true>::instrument(INS ins) {
        switch (INS_Category(ins))
        {
        case xed_category_enum_t::XED_CATEGORY_SSE:
          replace::vfc_wrappers::sse::instrument(loaded_backends, backends, contexts, ins);
          break;
        //~ case xed_category_enum_t::XED_CATEGORY_AVX:
          //~ replace::vfc_wrappers::avx::instrument(loaded_backends, backends, contexts, ins);
          //~ break;
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
  }

  interflop_module::interflop_module() : module(), 
    KnobInterflop(KNOB_MODE_WRITEONCE, "pintool", "interflop", "0", "use interflop dynamic backend (should not be used if using -fp-replace)"),
    KnobVectorInterflop(KNOB_MODE_WRITEONCE, "pintool", "vector-mode", "0", "use interflop dynamic backend (should not be used if using -fp-replace)"),
    data(nullptr)
  {}

  void interflop_module::init()
  {
    std::cerr << "Initialization: interflop module." << std::endl;
    std::cerr << "  Interflop verificarlo's dynamic backends : " << (KnobInterflop.Value() ? "enabled" : "disabled") << std::endl;
    std::cerr << "  Backends working mode : " << (KnobVectorInterflop.Value() ? "vector" : "scalar") << std::endl;
    if (!KnobInterflop.Value())
    {
      return;
    }
    //~ return;
    /* function required by vfcwrapper.c */
    interflop_set_handler("getenv", (void *)getenv);
    interflop_set_handler("sprintf", (void *)sprintf);
    interflop_set_handler("strerror", (void *)strerror);
    interflop_set_handler("gettid", (void*) get_tid);
    interflop_set_handler("fopen", (void *)fopen);
    interflop_set_handler("strcasecmp", (void *)strcasecmp);
    interflop_set_handler("vwarnx", (void *)vwarnx);
    interflop_set_handler("fprintf", (void *)fprintf);
    interflop_set_handler("exit", (void *)exit);
    interflop_set_handler("vfprintf", (void *)vfprintf);
    interflop_set_handler("malloc", (void *)malloc);
    interflop_set_handler("strcmp", (void *)strcmp);
    interflop_set_handler("strtod", (void *)_vfc_strtol);
    interflop_set_handler("strtod", (void *)_vfc_strtod);
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

    /* Environnement variable to disable loading message */
    char *silent_load_env = getenv("VFC_BACKENDS_SILENT_LOAD");
    bool silent_load =
        ((silent_load_env == NULL) || (strcasecmp(silent_load_env, "True") != 0))
            ? false
            : true;

    /* For each backend, load and register the backend vtable interface
      Backends .so are separated by semi-colons in the VFC_BACKENDS
      env variable */
    char *semicolonptr;
    char *token = strtok_r(vfc_backends, ";", &semicolonptr);

    //~ interflop_backend_interface_t * _backends = dyn_backends->get_backends_interface();
    //~ void ** _contexts = dyn_backends->get_backends_contexts();
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
      /* load the backend .so */
      void *handle = dlopen(backend_argv[0], RTLD_NOW);
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


      vfc_set_handlers(token, handle);

      /* Register backend */
      if (loaded_backends == MAX_BACKENDS) {
        logger_error("No more than %d backends can be used simultaneously",
                    MAX_BACKENDS);
      }

      handle_pre_init(&interflop_module::panic, stderr, &contexts[loaded_backends]);
      handle_cli(backend_argc, backend_argv, contexts[loaded_backends]);
      backends[loaded_backends] = handle_init(contexts[loaded_backends]);
      loaded_backends++;

      /* parse next backend token */
      token = strtok_r(NULL, ";", &semicolonptr);
    }
    //~ dyn_backends->set_loaded_backends(loaded_backends);
    if (loaded_backends == 0) {
      logger_error("%s syntax error: at least one backend should be provided",
                  vfc_backends_env);
    }

    /* Check that at least one backend implements each required operation */
    check_backends_implements(float, add);
    check_backends_implements(float, sub);
    check_backends_implements(float, mul);
    check_backends_implements(float, div);
    check_backends_implements(double, add);
    check_backends_implements(double, sub);
    check_backends_implements(double, mul);
    check_backends_implements(double, div);

    filter* filter = new null_filter();
    if (KnobVectorInterflop.Value())
    {
      data = new instrumenter(new replace_module_internals::dynamic_backend_instrumenters<true>(), filter);
    }
    else
    {
      data = new instrumenter(new replace_module_internals::dynamic_backend_instrumenters<false>(), filter);
    }
    data->TRACE_AddInstrumentFunction();
  }

  void interflop_module::panic (const char *msg)
  {
    std::cerr << msg;
    exit(1);
  }


  const std::string& interflop_module::name() 
  {
    static const std::string name_("interflop_module");
    return name_;
  }

}

#endif
