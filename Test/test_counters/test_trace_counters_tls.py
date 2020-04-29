import testCounterGenerator
import unittest

class float_add_scalar_tests(testCounterGenerator.testsBase):
	def setUp(self):
		self.counterMode = "1"
		self.precision = "float"
		self.operation = "add"
		self.mode = "scalar"
		self.tls = True

class float_add_simd_tests(testCounterGenerator.testsBase):
	def setUp(self):
		self.counterMode = "1"
		self.precision = "float"
		self.operation = "add"
		self.mode = "simd"
		self.tls = True

class float_sub_scalar_tests(testCounterGenerator.testsBase):
	def setUp(self):
		self.counterMode = "1"
		self.precision = "float"
		self.operation = "sub"
		self.mode = "scalar"
		self.tls = True

class float_sub_simd_tests(testCounterGenerator.testsBase):
	def setUp(self):
		self.counterMode = "1"
		self.precision = "float"
		self.operation = "sub"
		self.mode = "simd"
		self.tls = True

class float_mul_scalar_tests(testCounterGenerator.testsBase):
	def setUp(self):
		self.counterMode = "1"
		self.precision = "float"
		self.operation = "mul"
		self.mode = "scalar"
		self.tls = True

class float_mul_simd_tests(testCounterGenerator.testsBase):
	def setUp(self):
		self.counterMode = "1"
		self.precision = "float"
		self.operation = "mul"
		self.mode = "simd"
		self.tls = True

class float_div_scalar_tests(testCounterGenerator.testsBase):
	def setUp(self):
		self.counterMode = "1"
		self.precision = "float"
		self.operation = "div"
		self.mode = "scalar"
		self.tls = True

class float_div_simd_tests(testCounterGenerator.testsBase):
	def setUp(self):
		self.counterMode = "1"
		self.precision = "float"
		self.operation = "div"
		self.mode = "simd"
		self.tls = True

class float_fma_scalar_tests(testCounterGenerator.testsBase):
	def setUp(self):
		self.counterMode = "1"
		self.precision = "float"
		self.operation = "fma"
		self.mode = "scalar"
		self.tls = True

class float_fma_simd_tests(testCounterGenerator.testsBase):
	def setUp(self):
		self.counterMode = "1"
		self.precision = "float"
		self.operation = "fma"
		self.mode = "simd"
		self.tls = True

class float_mix_scalar_tests(testCounterGenerator.testsBase):
	def setUp(self):
		self.counterMode = "1"
		self.precision = "float"
		self.operation = "mix"
		self.mode = "scalar"
		self.tls = True

class float_mix_simd_tests(testCounterGenerator.testsBase):
	def setUp(self):
		self.counterMode = "1"
		self.precision = "float"
		self.operation = "mix"
		self.mode = "simd"
		self.tls = True

class double_add_scalar_tests(testCounterGenerator.testsBase):
	def setUp(self):
		self.counterMode = "1"
		self.precision = "double"
		self.operation = "add"
		self.mode = "scalar"
		self.tls = True

class double_add_simd_tests(testCounterGenerator.testsBase):
	def setUp(self):
		self.counterMode = "1"
		self.precision = "double"
		self.operation = "add"
		self.mode = "simd"
		self.tls = True

class double_sub_scalar_tests(testCounterGenerator.testsBase):
	def setUp(self):
		self.counterMode = "1"
		self.precision = "double"
		self.operation = "sub"
		self.mode = "scalar"
		self.tls = True

class double_sub_simd_tests(testCounterGenerator.testsBase):
	def setUp(self):
		self.counterMode = "1"
		self.precision = "double"
		self.operation = "sub"
		self.mode = "simd"
		self.tls = True

class double_mul_scalar_tests(testCounterGenerator.testsBase):
	def setUp(self):
		self.counterMode = "1"
		self.precision = "double"
		self.operation = "mul"
		self.mode = "scalar"
		self.tls = True

class double_mul_simd_tests(testCounterGenerator.testsBase):
	def setUp(self):
		self.counterMode = "1"
		self.precision = "double"
		self.operation = "mul"
		self.mode = "simd"
		self.tls = True

class double_div_scalar_tests(testCounterGenerator.testsBase):
	def setUp(self):
		self.counterMode = "1"
		self.precision = "double"
		self.operation = "div"
		self.mode = "scalar"
		self.tls = True

class double_div_simd_tests(testCounterGenerator.testsBase):
	def setUp(self):
		self.counterMode = "1"
		self.precision = "double"
		self.operation = "div"
		self.mode = "simd"
		self.tls = True

class double_fma_scalar_tests(testCounterGenerator.testsBase):
	def setUp(self):
		self.counterMode = "1"
		self.precision = "double"
		self.operation = "fma"
		self.mode = "scalar"
		self.tls = True

class double_fma_simd_tests(testCounterGenerator.testsBase):
	def setUp(self):
		self.counterMode = "1"
		self.precision = "double"
		self.operation = "fma"
		self.mode = "simd"
		self.tls = True

class double_mix_scalar_tests(testCounterGenerator.testsBase):
	def setUp(self):
		self.counterMode = "1"
		self.precision = "double"
		self.operation = "mix"
		self.mode = "scalar"
		self.tls = True

class double_mix_simd_tests(testCounterGenerator.testsBase):
	def setUp(self):
		self.counterMode = "1"
		self.precision = "double"
		self.operation = "mix"
		self.mode = "simd"
		self.tls = True


