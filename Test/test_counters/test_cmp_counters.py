import testCounterGenerator
import unittest

counterMode = "1"

class float_cmp_scalar_tests(testCounterGenerator.testsBase):
	def setUp(self):
		self.counterMode = counterMode
		self.precision = "float"
		self.operation = "cmp"
		self.mode = "scalar"
class double_cmp_scalar_tests(testCounterGenerator.testsBase):
	def setUp(self):
		self.counterMode = counterMode
		self.precision = "double"
		self.operation = "cmp"
		self.mode = "scalar"
class float_cmp_simd_tests(testCounterGenerator.testsBase):
	def setUp(self):
		self.counterMode = counterMode
		self.precision = "float"
		self.operation = "cmp"
		self.mode = "simd"
class double_cmp_simd_tests(testCounterGenerator.testsBase):
	def setUp(self):
		self.counterMode = counterMode
		self.precision = "double"
		self.operation = "cmp"
		self.mode = "simd"
		
