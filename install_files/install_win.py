import shutil
import os
import sys
import re


# install script for windows. Its takes the installation
# path as first argument, also, the compiler can be passed
# as the second argument (by default GCC), and the config
# file generator can be passed as the third argument
# (by default MinGW Makefiles) builds the project and
# installs the project in the passed directory
# example: python.exe install_win.py C:\path\to\install
# or: python.exe install_win.py C:\path\to\install clang
# or: python.exe install_win.py C:\path\to\install clang "Unix Makefiles"


def check_installed(cmd):
    return not (shutil.which(cmd) is None)


if len(sys.argv) < 2:
    print("Installation path required")
    exit()

if not (check_installed("cmake")):
    print("Could not find cmake")
    exit(1)

install_path = sys.argv[1]
c_compiler = "gcc"
cxx_compiler = "g++"
generator = "MinGW Makefiles"

if len(sys.argv) > 2:
    re.match(r"clang", sys.argv[2], re.IGNORECASE)
    c_compiler = "clang"
    cxx_compiler = "clang"
if len(sys.argv) > 3:
    generator = sys.argv[3]

if not(check_installed(c_compiler)):
    print("Could not find " + c_compiler)
    exit(1)

if not(check_installed(cxx_compiler)):
    print("Could not find " + cxx_compiler)
    exit(1)

if not(check_installed("make")):
    print("Could not find make")
    exit(1)

os.chdir("../")
if os.path.exists("build") and os.path.isdir("build"):
    shutil.rmtree("build")
os.mkdir("build")
os.chdir("build")

result = os.system("cmake -G \""                   + generator    + "\""     # generator, you may change it
                   " -DINSTALL_GMOCK:BOOL=OFF"                               # do not install tests
                   " -DCMAKE_C_COMPILER="          + c_compiler   +          # compilers, explicitly
                   " -DCMAKE_CXX_COMPILER="        + cxx_compiler +
                   " -DCMAKE_INSTALL_PREFIX:PATH=" + install_path + " ../")  # writes install path to cmakecache

if result:
    print("Error while generating cmake project's files")
    exit(2)
result = os.system("cmake --build . -j 6")  # builds all targets, '6' - number of threads, may change it
if result:
    print("Error while building the project")
    exit(3)
os.system("make install/local")
print("Project installed to " + install_path)
