# CMake generated Testfile for 
# Source directory: /mnt/C80C66C70C66B05C/BaiTap/CrossyRoady/ConsoleEngine/raylib/src
# Build directory: /mnt/C80C66C70C66B05C/BaiTap/CrossyRoady/ConsoleEngine/build/raylib/src
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(pkg-config--static "/mnt/C80C66C70C66B05C/BaiTap/CrossyRoady/ConsoleEngine/raylib/src/../cmake/test-pkgconfig.sh" "--static")
set_tests_properties(pkg-config--static PROPERTIES  _BACKTRACE_TRIPLES "/mnt/C80C66C70C66B05C/BaiTap/CrossyRoady/ConsoleEngine/raylib/src/CMakeLists.txt;180;add_test;/mnt/C80C66C70C66B05C/BaiTap/CrossyRoady/ConsoleEngine/raylib/src/CMakeLists.txt;0;")
subdirs("external/glfw")
