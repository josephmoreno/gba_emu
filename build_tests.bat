cd %~dp0
cmake -G"MinGW Makefiles" -DCMAKE_C_COMPILER="C:/Program Files/mingw64/bin/gcc.exe" -DCMAKE_CXX_COMPILER="C:/Program Files/mingw64/bin/g++.exe" -S . -B ./build

:: cd build
:: make
:: cd tests
:: .\unit_tests
:: ctest