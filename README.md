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
git submodule update --init
git submodule foreach 'git checkout main'
cmake -DUSE_FMA_INTRINSIC=1 . 
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

PENE offers two ways to instrument code. A cross-plateforme based on backend [Verrou](https://github.com/edf-hpc/verrou) named **fp-replace** and on based one [Interflop](https://github.com/interflop), available on Linux only.

## **FP-REPLACE** mode
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

## **INTERFLOP** mode (Linux only)

The interflop mode allows to dynamicaly load up to 16 backends and make them work separately. Available backends are :

| Backend                                                                   | Description                                                                                                                          |
|---------------------------------------------------------------------------|--------------------------------------------------------------------------------------------------------------------------------------|
|[backend ieee](https://github.com/a-hamitouche/interflop-backend-ieee)     | The IEEE backend implements straighforward IEEE-754 arithmetic. It should have no effect on the output and behavior of your program. |
|[backend verrou](https://github.com/a-hamitouche/interflop-backend-verrou) | The Verrou backend implements software couterpart of all IEEE-754 standard rounding modes                                            |
|[backend vprec](https://github.com/a-hamitouche/interflop-backend-vprec)   | The VPREC backend simulates any floating-point formats that can fit into the IEEE-754 double precision format with a round to the nearest.|

Each of these backends accepts different arguments and exhibits distinct behavior. To learn more, please consult the README.md files of each backend (located in the external subfolder or on their GitHub page).
With this mode, backends can be used with a scalar mode (by devectorizing operations) or in vector mode to reduce instrumentation overhead.

Usage :

```bash
VFC_BACKENDS="libbackend.so [option]" path/to/pin -t path/to/pene.so/or/pene.so -interflop -- path/to/executable
```

Exemple of usage with Verrou :


```bash
#Scalar mode
VFC_BACKENDS="libinterflop_verrou.so --rounding-mode=upward" path/to/pin -t path/to/pene.so/or/pene.so -interflop -- path/to/executable

#Vector mode
VFC_BACKENDS="libinterflop_verrou.so --rounding-mode=downward" path/to/pin -t path/to/pene.so/or/pene.so -interflop -vector-mode -- path/to/executable

#Multiple backends
VFC_BACKENDS="libinterflop_verrou.so --rounding-mode=upward; libinterflop_vprec.so --mode=full --precision-binary32=23 --debug" path/to/pin -t path/to/pene.so/or/pene.so -interflop -- path/to/executable
```

/!\ Note :
- Proposed backends fully work only in scalar mode
- For backend Verrou, only nearest, upward and downward modes are available on vector mode

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
