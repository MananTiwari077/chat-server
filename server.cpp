#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <cstring>
#include <thread>

void handleClient(SOCKET clientSocket){
    char buffer[1024];
    
    while(true){
        memset(buffer,0,sizeof(buffer));

        int bytesReceived= recv(
            clientSocket,
            buffer,
            sizeof(buffer),
            0
        );

        if(bytesReceived>0){
            std::cout<< "client says: "<< buffer<< "\n";
        }
        else if(bytesReceived==0){
            std::cout<< "client disconnected! \n";
            break;
        }
        else{
            std::cout<< "Receive failed! \n";
            break;
        }
    }
    closesocket(clientSocket);
}

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

    while(true){
        SOCKET clientSocket= accept(
            serverSocket,
            nullptr,
            nullptr
        );

        if(clientSocket == INVALID_SOCKET){
            std::cout<< "Accept failed! \n";
            continue;
        }
        std::cout<< "client connected successfully! \n";

        std::thread clientThread(handleClient,clientSocket);

        clientThread.detach();
    }

    closesocket(serverSocket);
    WSACleanup();

    return 0;
}