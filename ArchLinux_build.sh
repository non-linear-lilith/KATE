mkdir -p build
cd build
cmake -S ../ -B .
make && make Shaders &&./First_App
cd ../ 