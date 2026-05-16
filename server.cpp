#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>

int main() {
    WSADATA wsaData;

    int result = WSAStartup(MAKEWORD(2, 2), &wsaData);

    if (result != 0) {
        std::cout << "WSAStartup failed!\n";
        return 1;
    }

    SOCKET serverSocket = socket(
        AF_INET,
        SOCK_STREAM,
        IPPROTO_TCP
    );

    if (serverSocket == INVALID_SOCKET) {
        std::cout << "Socket creation failed!\n";
        WSACleanup();
        return 1;
    }

    sockaddr_in serverAddress;

    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(54000);
    serverAddress.sin_addr.s_addr = INADDR_ANY;

    result = bind(
        serverSocket,
        (sockaddr*)&serverAddress,
        sizeof(serverAddress)
    );

    if (result == SOCKET_ERROR) {
        std::cout << "Bind failed!\n";

        closesocket(serverSocket);
        WSACleanup();

        return 1;
    }

    std::cout << "Bind successful!\n";

    result = listen(serverSocket, SOMAXCONN);
    if(result== SOCKET_ERROR){
        std::cout<< "Listen failed! \n";
        closesocket(serverSocket);
        WSACleanup();

        return 1;
    }
    std::cout<< "server is listening on port 54000! \n";

    SOCKET clientSocket =accept(
        serverSocket,
        nullptr,
        nullptr
    );

    if(clientSocket == INVALID_SOCKET){
        std::cout<< "accept failed ! \n";
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }
    std::cout<< "client connected successfully! \n";

    char buffer[1024] = {0};

    int bytesReceived = recv(
        clientSocket,
        buffer,
        sizeof(buffer),
        0
    );

    if (bytesReceived > 0) {
        std::cout << "Client says: " << buffer << "\n";
    }


    closesocket(clientSocket);
    closesocket(serverSocket);
    WSACleanup();

    return 0;
}