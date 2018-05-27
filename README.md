cpp_cmake startup

step

make clean
mkdir build
cd build
cmake -DCMAKE_INSTALL_PREFIX=/tmp/t2 ..
make package
sudo dpkg -i example-3.3.0.deb

DCMAKE_INSTALL_PREFIX 默认 /usr/local
CPACK_PACKAGING_INSTALL_PREFIX=CMAKE_INSTALL_PREFIX


run with : /usr/bin/example
reference_links:
https://cgold.readthedocs.io/en/latest/index.html
0.https://docs.hunter.sh/en/latest/index.html
1.https://github.com/pabloariasal/modern-cmake-sample/