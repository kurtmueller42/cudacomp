mkdir build
cd build
cmake -DCMAKE_INSTALL_PREFIX="../out/" \
    -DLLVM_PATH="/usr/lib/llvm-3.5/bin/"  ../
make && make install