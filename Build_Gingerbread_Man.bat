del *.o
del *.exe
cls
C:\MinGW\bin\g++.exe -std=c++11 -O2 -Wall -c Gingerbread_Man.cpp -o Gingerbread_Man.o
C:\MinGW\bin\g++.exe -std=c++11 -O2 -Wall -o Gingerbread_Man.exe Gingerbread_Man.o ^
  -L"C:\MinGW\msys\1.0\glew-1.11.0\lib" ^
  -lopengl32 -lglu32 -lfreeglut -lglew32
