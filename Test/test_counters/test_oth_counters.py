import testCounterGenerator
import unittest

counterMode = "1"

class float_oth_scalar_tests(testCounterGenerator.testsBase):
	def setUp(self):
		self.counterMode = counterMode
		self.precision = "float"
		self.operation = "oth"
		self.mode = "scalar"
class double_oth_scalar_tests(testCounterGenerator.testsBase):
	def setUp(self):
		self.counterMode = counterMode
		self.precision = "double"
		self.operation = "oth"
		self.mode = "scalar"
class float_oth_simd_tests(testCounterGenerator.testsBase):
	def setUp(self):
		self.counterMode = counterMode
		self.precision = "float"
		self.operation = "oth"
		self.mode = "simd"
class double_oth_simd_tests(testCounterGenerator.testsBase):
	def setUp(self):
		self.counterMode = counterMode
		self.precision = "double"
		self.operation = "oth"
		self.mode = "simd"
		
