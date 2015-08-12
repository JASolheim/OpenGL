del *.exe
del *.o
cls
C:\MinGW\bin\g++.exe -std=c++11 -O2 -Wall -c Mouse_Click_Fan.cpp -o Mouse_Click_Fan.o -I"C:\MinGW\include" -I".\include"
C:\MinGW\bin\g++.exe -std=c++11 -O2 -Wall -o Mouse_Click_Fan.exe Mouse_Click_Fan.o -L"C:\MinGW\lib" -lglu32 -lfreeglut -lopengl32 -L"C:\MinGW\msys\1.0\glew-1.11.0\lib" -lglew32
