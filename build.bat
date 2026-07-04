@echo off

echo Building server...
g++ server.cpp -o server -std=c++17 -Wall -static -static-libgcc -static-libstdc++ -lws2_32

echo Building client...
g++ client.cpp -o client -std=c++17 -Wall -static -static-libgcc -static-libstdc++ -lws2_32

echo.
echo Build completed successfully!
pause