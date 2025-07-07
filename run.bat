cd %~dp0
:: g++ --std=gnu++17 -Wall -m64 -I "C:/Program Files/mingw64/sdl2-w64/include" -I "C:\Program Files\mingw64\entt\single_include\entt" -I include src/*.cpp -L "C:/Program Files/mingw64/sdl2-w64/lib" -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_mixer -o debug/out
g++ --std=gnu++17 -Wall -m64 -I "C:/Program Files/mingw64/sdl2-w64/include" -I include src/*.cpp -L "C:/Program Files/mingw64/sdl2-w64/lib" -lmingw32 -lSDL2main -lSDL2 -o debug/out
.\debug\out