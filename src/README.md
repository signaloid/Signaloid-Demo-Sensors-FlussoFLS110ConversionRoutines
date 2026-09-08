# Source code:

## main.c
Top-level driver: parses command-line arguments, dispatches to the UxHw and
Monte Carlo evaluation paths in `kernel.c`, and reports/writes the selected
output(s).

## kernel.c/h
Defines the FLS110 output selection (`FlussoFLS110OutputVariableIndex`) and
input variable indices, and the top-level kernel entry points that choose
between the UxHw (`flusso-fls110-uxhw.c`) and Monte Carlo
(`flusso-fls110-monte-carlo.c`) evaluation paths.

## flusso-fls110-uxhw.c/h
Calculates the distributional value of the selected FLS110 output using UxHw
distributional arithmetic, from a single distributional evaluation of the
sensor's inputs.

## flusso-fls110-monte-carlo.c/h
Runs repeated independent evaluations of the FLS110 conversion routine, one
per Monte Carlo sample, for the native Monte Carlo build.

## utilities.c/h
These contain utility methods for parsing, setting, and reporting
the usage of demo-specific command-line arguments of C/C++ demo applications.
These methods call similar methods from `common.c` for handling
command-line arguments common to all of our C/C++ demo applications.

## common.c/h
These contain utility methods for parsing, setting, and reporting
the usage of command-line arguments common to all of our C/C++ demo applications,
as well as other methods that we commonly make use across our
C/C++ demo applications, e.g., standard methods for I/O handling. These
source files are symlinks to the original files contained in the repository
[Signaloid-Demo-CommonUtilityRoutines](https://github.com/signaloid/Signaloid-Demo-CommonUtilityRoutines)
which is included as a submodule in `submodules/common`.

## uxhw.c/h
These contain methods that implement the probabilistic versions of the methods
in the UxHw API (e.g., `UxHwDoubleGaussDist`) and uses the GNU Scientific Library (GSL)
random number generators to achieve that. This allows building our C/C++ demo applications
natively (i.e., on conventional architectures) and running native Monte Carlo evaluations
of our C/C++ demo applications without modifying the source code.
These source files are symlinks to the original files and are contained in the repository
[Signaloid-Demo-UxHwCompatibilityForNativeExecution](https://github.com/signaloid/Signaloid-Demo-UxHwCompatibilityForNativeExecution)
which is included as a submodule in `submodules/compat`.

## config.mk
Signaloid cores use this file to identify the source codes they will use when
building the C/C++ demo application.

# To Build Natively on Non-Signaloid Platforms

From the repository root, run:
```
make local-build
```
This builds the `demo-native-mc` executable at the repository root, using the
host `gcc` toolchain and the UxHw compatibility shim in `submodules/compat`.
See the [Prerequisites](../README.md#prerequisites) section of the root README for installing GSL and the other build dependencies.
