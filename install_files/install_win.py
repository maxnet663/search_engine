import shutil
import os
import sys


def check_installed(cmd):
    return not(shutil.which(cmd) is None)


if len(sys.argv) < 2:
    print("Installation path required")
    exit()

install_path = sys.argv[1]

if not(check_installed("cmake")):
    print("Could not find cmake")
    exit()
if not(check_installed("g++")):
    print("Could not find g++")
    exit()
if not(check_installed("make")):
    print("Could not find make")
    exit()

os.chdir("../")
if os.path.exists("build") and os.path.isdir("build"):
    print("build directory already exists")
    exit()

os.mkdir("build")
os.chdir("build")
os.system("cmake -G \"MinGW Makefiles\" -DINSTALL_GMOCK:BOOL=OFF "
          "-DCMAKE_INSTALL_PREFIX:PATH=" + install_path + " ../")
os.system("cmake --build .")
os.system("make install/local")
print("Project installed to " + install_path)
