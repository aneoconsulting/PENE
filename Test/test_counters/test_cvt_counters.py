import testCounterGenerator
import unittest

counterMode = "1"

class i2d_cvt_scalar_tests(testCounterGenerator.testsBase):
	def setUp(self):
		self.counterMode = counterMode
		self.precision = "i2d"
		self.operation = "cvt"
		self.mode = "scalar"
class i2f_cvt_scalar_tests(testCounterGenerator.testsBase):
	def setUp(self):
		self.counterMode = counterMode
		self.precision = "i2f"
		self.operation = "cvt"
		self.mode = "scalar"
class f2d_cvt_scalar_tests(testCounterGenerator.testsBase):
	def setUp(self):
		self.counterMode = counterMode
		self.precision = "f2d"
		self.operation = "cvt"
		self.mode = "scalar"
class f2i_cvt_scalar_tests(testCounterGenerator.testsBase):
	def setUp(self):
		self.counterMode = counterMode
		self.precision = "f2i"
		self.operation = "cvt"
		self.mode = "scalar"
class d2i_cvt_scalar_tests(testCounterGenerator.testsBase):
	def setUp(self):
		self.counterMode = counterMode
		self.precision = "d2i"
		self.operation = "cvt"
		self.mode = "scalar"
class d2f_cvt_scalar_tests(testCounterGenerator.testsBase):
	def setUp(self):
		self.counterMode = counterMode
		self.precision = "d2f"
		self.operation = "cvt"
		self.mode = "scalar"
		
class i2d_cvt_simd_tests(testCounterGenerator.testsBase):
	def setUp(self):
		self.counterMode = counterMode
		self.precision = "i2d"
		self.operation = "cvt"
		self.mode = "simd"
class i2f_cvt_simd_tests(testCounterGenerator.testsBase):
	def setUp(self):
		self.counterMode = counterMode
		self.precision = "i2f"
		self.operation = "cvt"
		self.mode = "simd"
class f2d_cvt_simd_tests(testCounterGenerator.testsBase):
	def setUp(self):
		self.counterMode = counterMode
		self.precision = "f2d"
		self.operation = "cvt"
		self.mode = "simd"
class f2i_cvt_simd_tests(testCounterGenerator.testsBase):
	def setUp(self):
		self.counterMode = counterMode
		self.precision = "f2i"
		self.operation = "cvt"
		self.mode = "simd"
class d2i_cvt_simd_tests(testCounterGenerator.testsBase):
	def setUp(self):
		self.counterMode = counterMode
		self.precision = "d2i"
		self.operation = "cvt"
		self.mode = "simd"
class d2f_cvt_simd_tests(testCounterGenerator.testsBase):
	def setUp(self):
		self.counterMode = counterMode
		self.precision = "d2f"
		self.operation = "cvt"
		self.mode = "simd"