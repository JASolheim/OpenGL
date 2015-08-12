del *.exe
del *.o
cls
C:\MinGW\bin\g++.exe -std=c++11 -O2 -Wall -c Geo_Prim.cpp -o Geo_Prim.o -I"C:\MinGW\include" -I".\include"
C:\MinGW\bin\g++.exe -std=c++11 -O2 -Wall -o Geo_Prim.exe Geo_Prim.o -L"C:\MinGW\lib" -lglu32 -lfreeglut -lopengl32 -L"C:\MinGW\msys\1.0\glew-1.11.0\lib" -lglew32
