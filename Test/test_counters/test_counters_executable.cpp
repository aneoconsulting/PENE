#include <iostream>
#include <emmintrin.h>
#include <string>

void main(int argc, char* argv[])
{
  std::cout << "This program is used for tests purposes only. "
    << "If used to check the ability of the pintool to count operations, "
    << "input have to be chosen so that the result is 0 to avoid fp->string conversion."
    << std::endl;

  if (argc == 1)
  {
    argv = new char* [7]{ "main", "add", "double", "scalar", "100", "0", "0" };
  }
  std::string operation{ argv[1] };
  std::string precision{ argv[2] };
  std::string mode{ argv[3] };
  auto nb_loop = std::stoi(argv[4]);

  if (precision.compare("float") == 0)
  {
    auto a = std::stof(argv[5]);
    auto b = std::stof(argv[6]);
    auto accu = _mm_set1_ps(a);
    auto b_ = _mm_set1_ps(b);

    if (operation.compare("add") == 0)
    {
      if (mode.compare("scalar") == 0)
      {
        for (auto i = 0; i < nb_loop; ++i)
          accu = _mm_add_ss(accu, b_);
      }
      else if (mode.compare("simd") == 0)
      {
        for (auto i = 0; i < nb_loop; ++i)
          accu = _mm_add_ps(accu, b_);
      }
    }
    else if (operation.compare("sub") == 0)
    {
      if (mode.compare("scalar") == 0)
      {
        for (auto i = 0; i < nb_loop; ++i)
          accu = _mm_sub_ss(accu, b_);
      }
      else if (mode.compare("simd") == 0)
      {
        for (auto i = 0; i < nb_loop; ++i)
          accu = _mm_sub_ps(accu, b_);
      }
    }
    else if (operation.compare("mul") == 0)
    {

      if (mode.compare("scalar") == 0)
      {
        for (auto i = 0; i < nb_loop; ++i)
          accu = _mm_mul_ss(accu, b_);
      }
      else if (mode.compare("simd") == 0)
      {
        for (auto i = 0; i < nb_loop; ++i)
          accu = _mm_mul_ps(accu, b_);
      }
    }
    else if (operation.compare("div") == 0)
    {
      if (mode.compare("scalar") == 0)
      {
        for (auto i = 0; i < nb_loop; ++i)
          accu = _mm_div_ss(accu, b_);
      }
      else if (mode.compare("simd") == 0)
      {
        for (auto i = 0; i < nb_loop; ++i)
          accu = _mm_div_ps(accu, b_);
      }
    }
    std::cout << "Testing information :" << std::endl;
    std::cout << "Operation : " << argv[1] << std::endl;
    std::cout << "Precision : " << argv[2] << std::endl;
    std::cout << "Vectorization : " << argv[3] << std::endl;
    std::cout << "Loop : " << argv[4] << std::endl;
    std::cout << "Test number : " << argv[5] << "  " << argv[6] << std::endl;
    std::cout << "result : " << reinterpret_cast<float*>(&accu)[0] << std::endl;
  }
  else if (precision.compare("double") == 0)
  {
    auto a = std::stod(argv[5]);
    auto b = std::stod(argv[6]);
    auto accu = _mm_set1_pd(a);
    auto b_ = _mm_set1_pd(b);
    if (operation.compare("add") == 0)
    {
      if (mode.compare("scalar") == 0)
      {
        for (auto i = 0; i < nb_loop; ++i)
          accu = _mm_add_sd(accu, b_);
      }
      else if (mode.compare("simd") == 0)
      {
        for (auto i = 0; i < nb_loop; ++i)
          accu = _mm_add_pd(accu, b_);
      }
    }
    else if (operation.compare("sub") == 0)
    {

      if (mode.compare("scalar") == 0)
      {
        for (auto i = 0; i < nb_loop; ++i)
          accu = _mm_sub_sd(accu, b_);
      }
      else if (mode.compare("simd") == 0)
      {
        for (auto i = 0; i < nb_loop; ++i)
          accu = _mm_sub_pd(accu, b_);
      }
    }
    else if (operation.compare("mul") == 0)
    {

      if (mode.compare("scalar") == 0)
      {
        for (auto i = 0; i < nb_loop; ++i)
          accu = _mm_mul_sd(accu, b_);
      }
      else if (mode.compare("simd") == 0)
      {
        for (auto i = 0; i < nb_loop; ++i)
          accu = _mm_mul_pd(accu, b_);
      }
    }
    else if (operation.compare("div") == 0)
    {

      if (mode.compare("scalar") == 0)
      {
        for (auto i = 0; i < nb_loop; ++i)
          accu = _mm_div_sd(accu, b_);
      }
      else if (mode.compare("simd") == 0)
      {
        for (auto i = 0; i < nb_loop; ++i)
          accu = _mm_div_pd(accu, b_);
      }
    }
    std::cout << "Testing information :" << std::endl;
    std::cout << "Operation : " << argv[1] << std::endl;
    std::cout << "Precision : " << argv[2] << std::endl;
    std::cout << "Vectorization : " << argv[3] << std::endl;
    std::cout << "Loop : " << argv[4] << std::endl;
    std::cout << "Test number : " << argv[5] << "  " << argv[6] << std::endl;
    std::cout << "result : " << reinterpret_cast<float*>(&accu)[0] << std::endl;
  }
}