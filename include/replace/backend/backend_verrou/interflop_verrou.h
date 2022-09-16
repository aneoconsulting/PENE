
/*--------------------------------------------------------------------*/
/*--- Verrou: a FPU instrumentation tool.                          ---*/
/*--- Interface for floatin-point operations overloading.          ---*/
/*---                                           interflop_verrou.h ---*/
/*--------------------------------------------------------------------*/

/*
   This file is part of Verrou, a FPU instrumentation tool.

   Copyright (C) 2014-2021 EDF
     F. Févotte     <francois.fevotte@edf.fr>
     B. Lathuilière <bruno.lathuiliere@edf.fr>

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public License as
   published by the Free Software Foundation; either version 2.1 of the
   License, or (at your option) any later version.

   This program is distributed in the hope that it will be useful, but
   WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   You should have received a copy of the GNU Lesser General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
   02111-1307, USA.

   The GNU Lesser General Public License is contained in the file COPYING.
*/


#ifndef __INTERFLOP_VERROU_H
#define __INTERFLOP_VERROU_H

//#define DEBUG_PRINT_OP

#ifdef __cplusplus
extern "C" {
#endif
//#define IFV_FCTNAME(FCT) interflop_verrou_##FCT



namespace pene
{
  namespace replace
  {
    namespace backend
    {

  enum vr_RoundingMode {
    VR_NEAREST,
    VR_UPWARD,
    VR_DOWNWARD,
    VR_ZERO,
    VR_RANDOM, // Must be immediately after standard rounding modes
    VR_RANDOM_DET,
    VR_RANDOM_COMDET,
    VR_AVERAGE,
    VR_AVERAGE_DET,
    VR_AVERAGE_COMDET,
    VR_FARTHEST,
    VR_FLOAT,
    VR_NATIVE,
    VR_FTZ
  };
  
  struct verrou {
  const char* (*backend_name)(void);
  const char* (*backend_version)(void);

  static void add_float(float a, float b, float *c, void *context);
  static void sub_float(float a, float b, float *c, void *context);
  static void mul_float(float a, float b, float *c, void *context);
  static void div_float(float a, float b, float *c, void *context);
  //void (*cmp_float)(enum FCMP_PREDICATE p, float a, float b, int *c,
                              //void *context);

  static void add_double(double a, double b, double *c, void *context);
  static void sub_double(double a, double b, double *c, void *context);
  static void mul_double(double a, double b, double *c, void *context);
  static void div_double(double a, double b, double *c, void *context);
  //void (*cmp_double)(enum FCMP_PREDICATE p, double a, double b,
                               //int *c, void *context);

  static void cast_double_to_float(double a, float *b, void *context);
  static void madd_double(double a, double b, double c, double *res, void *context);
  static void madd_float(float a, float b, float c, float *res, void *context);

  /* interflop_finalize: called at the end of the instrumented program
   * execution */
  static void finalize(void *context);
  struct verrou init(void ** context);
};


  void configure(enum vr_RoundingMode mode,void* context);
  //void finalize(void* context);

  //const char* get_backend_name(void);
  //const char* get_backend_version(void);


  const char* verrou_rounding_mode_name (enum vr_RoundingMode mode);

  void verrou_begin_instr(void);
  void verrou_end_instr(void);

  void verrou_set_seed (unsigned int seed);
  void verrou_set_random_seed (void);

  void verrou_init_profiling_exact(void);
  void verrou_get_profiling_exact(unsigned int* num, unsigned int* numExact);

  extern void (*vr_panicHandler)(const char*);
  void verrou_set_panic_handler(void (*)(const char*));
  extern void (*vr_nanHandler)(void);
  void verrou_set_nan_handler(void (*nanHandler)(void));

  extern void (*vr_infHandler)(void);
  void verrou_set_inf_handler(void (*infHandler)(void));


  extern void (*vr_debug_print_op)(int,const char*, const double* args, const double* res);
  void verrou_set_debug_print_op(void (*)(int nbArg, const char* name, const double* args, const double* res));

  //struct interflop_backend_interface_t init(void ** context);
  /*
  void add_double (double a, double b, double* res, void* context);    
  void add_float  (float a,  float b,  float*  res, void* context);
  void sub_double (double a, double b, double* res, void* context);
  void sub_float (float a,  float b,  float*  res, void* context);
  void mul_double(double a, double b, double* res, void* context);
  void mul_float(float a,  float b,  float*  res, void* context);
  void div_double(double a, double b, double* res, void* context);
  void div_float(float a,  float b,  float*  res, void* context);

  void cast_double_to_float(double a, float* b, void* context);

  void madd_double(double a, double b, double c, double* res, void* context);
  void madd_float(float a,  float b,  float c,  float*  res, void* context);
  */
    }
    }
    }

#ifdef __cplusplus
}
#endif

#endif /* ndef __INTERFLOP_VERROU_H */
