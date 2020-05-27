import subprocess
import re
import unittest

class testCounterGenerator(unittest.TestCase):
    executable = "${EXECUTABLE}"
    tool = "${PINTOOL}"
    pin = "${PIN}"

    counterModes = ["0", "1", "2"]
    precisions = ["float", "double"]
    operations = ["add", "sub", "mul", "div", "fma"]
    modes = ["scalar", "simd"]

    def launch(self, counterMode, tls, prec, op, mode, nLoop, a, b):
        cmdLine = ("{pin} -t {tool} -counter-mode {counterMode} -counter-tls {tls} -- " +
                   " {executable} {op} {prec} {mode} {nLoop} {a} {b}").format(pin = self.pin,
                                                                              tool = self.tool,
                                                                              counterMode = counterMode,
                                                                              tls = "1" if tls else "0",
                                                                              executable = self.executable,
                                                                              op = op,
                                                                              prec = prec,
                                                                              mode = mode,
                                                                              nLoop = str(nLoop), 
                                                                              a = str(a), 
                                                                              b = str(b))
        out = subprocess.run(cmdLine.split(" "), stdout=subprocess.PIPE, stderr=subprocess.STDOUT, shell = True)
        print("the commandline is {}".format(subprocess.list2cmdline(out.args)))
        #self.assertEqual(0, out.returncode)
        output = out.stdout.decode('utf-8')
        print(output)
        regex = r"\A=*[\r\n]{1,2}\s*?PENE: Pin Enabled Numerical Exploration\s*?[\r\n]{1,2}=*[\r\n]{1,2}"
        p = re.compile(regex)
        self.assertRegex(output, p)
        return output

    def expectResult(self, op, nLoop, a, b):
        switcher = {
            "add": lambda a1, b1:a1+b1,
            "sub": lambda a1, b1:a1-b1,
            "mul": lambda a1, b1:a1*b1,
            "div": lambda a1, b1:a1/b1,
            "fma": lambda a1, b1:a1+a1*b1,
            "mix": lambda a1, b1:a1
        }
        for i in range(int(nLoop)):
            a = switcher[op](a,b)
        return a

    def getNbOps(self, output, prec, op, mode):
        regex = prec + r" *?" + op + r" *?" + mode + r" *?(?P<value>\d+)"
        if op == "sub":
            regex = prec + r" *?add *?" + mode  + r" *?(?P<value>\d+)"
        p = re.compile(regex)
        self.assertRegex(output, p)
        m = p.search(output)
        return float(m.group("value"))

    def getResult(self, output, prec, op, mode):
        regex = r"Result : (?P<value>\d+)"
        p = re.compile(regex)
        self.assertRegex(output, p)
        m = p.search(output)
        return float(m.group("value"))

    def checkExec(self, counterMode, tls, prec, op, mode, nLoop, a, b):
        output = self.launch(counterMode, tls, prec, op, mode, nLoop, a, b)
        if counterMode == "1" or counterMode == "2":
            if tls:
                self.assertRegex(output, re.compile(r"TLS has been activated for counters."))
            expected = self.expectResult(op, nLoop, a, b)
            result = self.getResult(output, prec, op, mode)
            self.assertEqual(expected, result)
            if op!="mix":
                nbOps = self.getNbOps(output, prec, op, mode)
                if result == 0:
                    self.assertEqual(nLoop, nbOps)
                else:
                    self.assertGreaterEqual(nbOps, nLoop)
            else:
                nbOps = self.getNbOps(output, prec, "add", mode)
                if result == 0:
                    self.assertEqual(nLoop*6, nbOps)
                else:
                    self.assertGreaterEqual(nbOps, nLoop*6)

                nbOps = self.getNbOps(output, prec, "mul", mode)
                if result == 0:
                    self.assertEqual(nLoop*2, nbOps)
                else:
                    self.assertGreaterEqual(nbOps, nLoop*2)

                nbOps = self.getNbOps(output, prec, "div", mode)
                if result == 0:
                    self.assertEqual(nLoop*4, nbOps)
                else:
                    self.assertGreaterEqual(nbOps, nLoop*4)

                nbOps = self.getNbOps(output, prec, "fma", mode)
                if result == 0:
                    self.assertEqual(nLoop*2, nbOps)
                else:
                    self.assertGreaterEqual(nbOps, nLoop*2)
        elif counterMode == "0":
            regex = r"(?m)^Set counters to no instrumentation mode$"
            p = re.compile(regex)
            self.assertRegex(output, p)
            expected = self.expectResult(op, nLoop, a, b)
            result = self.getResult(output, prec, op, mode)
            self.assertEqual(expected, result)
        else:
            regex = r"(?m)^ERROR: -counter-mode option only accepts values [A-Za-z0-9 ]+.$"
            p = re.compile(regex)
            self.assertRegex(output, p)

    def checkCVT(self, counterMode, tls, prec, op, mode, nLoop, a, b):
        output = self.launch(counterMode, tls, prec, op, mode, nLoop, a, b)
        if counterMode == "1" or counterMode == "2":
            if tls:
                self.assertRegex(output, re.compile(r"TLS has been activated for counters."))
            nbOps = self.getNbOps(output, prec, op, mode)
            self.assertGreaterEqual(nbOps, nLoop)
        elif counterMode == "0":
            regex = r"(?m)^Set counters to no instrumentation mode$"
            p = re.compile(regex)
            self.assertRegex(output, p)
        else:
            regex = r"(?m)^ERROR: -counter-mode option only accepts values [A-Za-z0-9 ]+.$"
            p = re.compile(regex)
            self.assertRegex(output, p)


class testsBase(testCounterGenerator):
    counterMode = None
    precision = None
    operation = None
    mode = None
    tls = False
    
    def test_result(self):
        """Checks that instruction behavior is not modified by instrumentation"""
        with self.subTest("nLoop:4  a:400   b:2"):
            self.checkExec(self.counterMode, self.tls, self.precision, self.operation, self.mode, 4, 400, 2)
        with self.subTest("nLoop:3  a:81   b:3"):
            self.checkExec(self.counterMode, self.tls, self.precision, self.operation, self.mode, 3, 81, 3)

    def test_count(self):
        """Checks that instruction instruction count is correct"""
        if self.operation == "cvt":
            with self.subTest("nLoop:2  a:0   b:1"):
                self.checkCVT(self.counterMode, self.tls, self.precision, self.operation, self.mode,  2, 0, 1)
            with self.subTest("nLoop:9  a:0   b:1"):
                self.checkCVT(self.counterMode, self.tls, self.precision, self.operation, self.mode, 9, 0, 1)
        elif self.operation == "div":
            with self.subTest("nLoop:2  a:0   b:1"):
                self.checkExec(self.counterMode, self.tls, self.precision, self.operation, self.mode,  2, 0, 1)
            with self.subTest("nLoop:9  a:0   b:1"):
                self.checkExec(self.counterMode, self.tls, self.precision, self.operation, self.mode, 9, 0, 1)
        elif self.operation == 'add' or self.operation == 'sub' or self.operation == 'mul' or self.operation == 'fma':
            with self.subTest("nLoop:2  a:0   b:0"):
                self.checkExec(self.counterMode, self.tls, self.precision, self.operation, self.mode,  2, 0, 0)
            with self.subTest("nLoop:9  a:0   b:0"):
                self.checkExec(self.counterMode, self.tls, self.precision, self.operation, self.mode, 9, 0, 0)
        elif self.operation == "mix":
            with self.subTest("nLoop:217  a:0   b:1"):
                self.checkExec(self.counterMode, self.tls, self.precision, self.operation, self.mode,  217, 0, 1)
        else:
            self.fail()

