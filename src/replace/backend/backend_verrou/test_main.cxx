
#define DEBUG_PRINT_OP

#include "interflop_verrou.h"
#include <stdio.h>
#include <iostream>
#include <iomanip>
using namespace pene::replace::backend;


void print_debug(int nbArg, const char * op,const  double* a, const double* res){

  if(nbArg==1){
    std::cout << op << " : "<< a[0] << "->"<<&res << std::endl;
  }

  if(nbArg==2){
    std::cout << op << " : "<< a[0] << "," << a[1]<< "->"<<&res << std::endl;
  }

  if(nbArg==3){
    std::cout << op << " : "<< a[0] << "," << a[1]<< "," << a[2]<< "->"<<&res << std::endl;
  }

} ;


int main(int argc, char** argv){

  void* context;
  struct verrou ifverrou=init(&context);

  configure(VR_UPWARD, context);

  
  uint64_t seed=0;
  seed=1029933002;
  std::cout << "seed: " << seed<<std::endl;
  //seed=1000000000;
  // if(conf.seed==(unsigned int) -1){
  //   struct timeval t1;
  //   gettimeofday(&t1, NULL);
  //   conf.seed =  t1.tv_usec + getpid();
  // }
  verrou_set_seed (seed);
  

  verrou_set_debug_print_op(&print_debug);

  /*
  double step=0.1;
  double acc=0.;

  for(int i=0; i<10000; i++){
    add_double(acc,step,&acc,context);
  }

  //std::cout << "acc: "<< std::hexfloat << acc << std::endl;
  std::cout << std::setprecision(16);
  std::cout << "acc: "<<acc << std::endl;*/
  
  float y= 8000609;
  float z= 388000.1;
  float res=0.;
 
  add_float(y,z,&res,context);
  std::cout << std::hexfloat << res << std::endl;

  finalize(context);


  return 0;
}
