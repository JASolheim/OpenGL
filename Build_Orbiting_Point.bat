del *.o
del *.exe
cls
C:\MinGW\bin\g++.exe -std=c++11 -O2 -Wall -c Orbiting_Point.cpp -o Orbiting_Point.o
C:\MinGW\bin\g++.exe -std=c++11 -O2 -Wall -o Orbiting_Point.exe Orbiting_Point.o ^
  -L"C:\MinGW\msys\1.0\glew-1.11.0\lib" ^
  -lopengl32 -lglu32 -lfreeglut -lglew32
