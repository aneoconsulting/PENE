import testCounterGenerator 
import unittest

class instruction_count_flag_tests(testCounterGenerator.testCounterGenerator):
	def test_disable_instruction_count(self):
		self.checkExec("0", "0", "float", "add", "scalar", 1, 1, 1)

	def wrong_instruction_count_flag_tests(self):
		with self.subTest("Instrumentation mode:-1"):
			self.checkExec("-1", "0", "float", "add", "scalar", 1, 1, 1)

