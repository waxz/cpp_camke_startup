cpp_cmake startup

step

make clean
mkdir build
cd build
cmake ..
make package
sudo dpkg -i example-3.3.0.deb

binary file will be installed in /usr/bin
library wiil be install in /usr/lib

run with : /usr/bin/example
reference_links:
https://cgold.readthedocs.io/en/latest/index.html
0.https://docs.hunter.sh/en/latest/index.html
1.https://github.com/pabloariasal/modern-cmake-sample/