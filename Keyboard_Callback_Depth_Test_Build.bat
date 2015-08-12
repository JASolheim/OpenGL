del *.exe
del *.o
cls
C:\MinGW\bin\g++.exe -std=c++11 -O2 -Wall -c .\include\InitShader.cpp -o .\include\InitShader.o -I"C:\MinGW\include" -I".\include"
C:\MinGW\bin\g++.exe -std=c++11 -O2 -Wall -c Keyboard_Callback_Depth_Test.cpp -o Keyboard_Callback_Depth_Test.o -I"C:\MinGW\include" -I".\include"
C:\MinGW\bin\g++.exe -std=c++11 -O2 -Wall -o Keyboard_Callback_Depth_Test.exe Keyboard_Callback_Depth_Test.o .\include\InitShader.o -L"C:\MinGW\lib" -lglu32 -lfreeglut -lopengl32 -L"C:\MinGW\msys\1.0\glew-1.11.0\lib" -lglew32
