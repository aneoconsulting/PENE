# PENE

PENE (Pin Enabled Numerical Exploration) is a dynamic instrumentation tool based on [Intel Pin](https://www.intel.com/content/www/us/en/developer/articles/tool/pin-a-dynamic-binary-instrumentation-tool.html) that instruments floating-point operations. It is compatible for Linux and Windows systems.

PENE allows to change the rounding mode for floating-point operations by choosing one of the IEEE rounding mode or  random rounding mode.

Changing the rounding mode was possible thanks to the implementation of the library that allows to change rounding mode by [Verrou](https://github.com/edf-hpc/verrou) developers.

## Requirements

- cmake version >= 3.20
- make
- g++ compiler for Linux and MSVC compiler for  Windows
- python3
- [jinja2](https://jinja.palletsprojects.com/en/2.10.x/)
- [pytest](https://docs.pytest.org/en/7.2.x/)

## Installation

First clone the repository. 

Then, go inside the repository then configure and build the project and finally run the tests.

To do so, run via your command line on Linux or your powershell on Windows:

```shell
cd [PENE repository] 
cmake . 
cmake --build .
ctest -C Debug
``` 
On Linux, you can create a folder, then generate and build the project inside it. 

You can also install the tool via a code editor.

## How to execute your code with PENE?

To use PENE to execute your application, run: 

```shell
path/to/pin -t path/to/pene.so/or/pene.dll [OPTIONS] -- path/to/executable
```

- The `path/to/pin` should be in your PENE repository under `/Pin/Linux/pin` if you are on Linux machine and `/Pin/Windows/pin` if you are on Windows machine.
- The path to the PENE tool is the path to the shared object file that corresponds to PENE tool, which was created during the build.
Without specifying any options, this command will execute your code under PENE without any instrumentation. 

# How to instrument your code with PENE?

To instrument your code, you can use the **fp-replace** option:

```shell
path/to/pin -t path/to/pene.so/or/pene.dll -fp-replace [0 or 1] -- path/to/executable
```

**-fp-replace 0** is the default mode when no instrumentation is done 
**-fp-replace 1** corresponds to instrumenting the application and replacing floating-point operations by operations that use also IEEE default rounding mode.

To be able to change the rounding mode for floating-point operations while instrumenting, you should use:

```shell
path/to/pin -t path/to/pene.so/or/pene.dll -fp-replace 3 -rounding-mode [0 to 5] -- path/to/executable
```

where:

- if rounding-mode is not specified or set to 1 : nearest rounding mode 
- rounding-mode = 2 : upward rounding 
- rounding-mode = 3 : downward rounding
- rounding-mode = 4 : rounding to zero
- rounding-mode = 5 : random rounding

## How to exclude parts of the code from instrumentation?

You can filter which parts of the code your want to instrument using the option `-exclude` along with an exclusion file containing a list of the functions you want to exclude.

The exclusion file should follow the following format:

- one exclusion rule per line
- each line should be of the form `pathsymbol fctname` , where `pathsymbol` is the path for the symbol: the shared library or the executable, and `fctname` is the function name. 

To generate the list of symbols with floating-point instructions present in your code, you can use the option `-gen-sym-list`:

```shell
path/to/pin -t path/to/pene.so/or/pene.dll -gen-sym-list list.lst -- path/to/executable
```

The list of symbols will be saved in `list.lst` in that case. You can rename this file.

There is also an option `-gen-sym-list` that sets the mode for symbols generation:

```shell
path/to/pin -t path/to/pene.so/or/pene.dll -gen-sym-list list.txt -gen-sym-mode [0 or 1] -- path/to/executable
```

When the mode is set to 0, it will list all the symbols with floating-point instructions that have been loaded during execution. If set to 1, it will list all the symbols with floating-point instructions that have been executed. Default mode is 0.
