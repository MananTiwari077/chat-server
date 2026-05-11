#include <iostream>
#include <winsock2.h>


int main() {
    WSADATA wsaData;

    int result = WSAStartup(MAKEWORD(2, 2), &wsaData);

    if (result != 0) {
        std::cout << "WSAStartup failed!\n";
        return 1;
    }

    std::cout << "Winsock initialized successfully!\n";

    WSACleanup();

    return 0;
}