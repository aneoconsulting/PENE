import unittest
import argparse
import os

def dir_path(string):
    if os.path.isdir(string):
        return string
    else:
        raise NotADirectoryError(string)


def printTestIds(test_case):
    if hasattr(test_case, '__iter__'):
        for x in test_case:
            printTestIds(x)
    else:
        print(test_case.id())

if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument('path', type=dir_path)
    printTestIds(unittest.defaultTestLoader.discover(parser.path))    