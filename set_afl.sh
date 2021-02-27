mkdir build
export CC="afl-gcc"
export CFLAGS="-fsanitize=address"
export AFL_USE_ASAN=1
export CFLAGS="$CFLAGS -m32"
cmake -S . -B build
cd ./build
make
