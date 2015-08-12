del *.exe
del *.o
cls
C:\MinGW\bin\g++.exe -std=c++11 -O2 -Wall -c .\include\InitShader.cpp -o .\include\InitShader.o -I"C:\MinGW\include" -I".\include"
C:\MinGW\bin\g++.exe -std=c++11 -O2 -Wall -c Look_At.cpp -o Look_At.o -I"C:\MinGW\include" -I".\include"
C:\MinGW\bin\g++.exe -std=c++11 -O2 -Wall -o Look_At.exe Look_At.o .\include\InitShader.o -L"C:\MinGW\lib" -lglu32 -lfreeglut -lopengl32 -L"C:\MinGW\msys\1.0\glew-1.11.0\lib" -lglew32
