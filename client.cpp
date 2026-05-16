#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>

int main(){
    WSADATA wsaData;
    int result= WSAStartup(MAKEWORD(2,2),&wsaData);

    if(result!=0){
        std::cout<< "WSAStartup failed! \n";
        return 1;
    }

    SOCKET clientSocket= socket(
        AF_INET,
        SOCK_STREAM,
        IPPROTO_TCP
    );

    if(clientSocket==INVALID_SOCKET){
        std::cout<< "socket creation failed! \n";
        WSACleanup();
        return 1;
    }
    sockaddr_in serverAddress;

    serverAddress.sin_family= AF_INET;
    serverAddress.sin_port= htons(54000);

    inet_pton(
        AF_INET,
        "127.0.0.1",
        &serverAddress.sin_addr
    );

    result = connect(
        clientSocket,
        (sockaddr*)&serverAddress,
        sizeof(serverAddress)
    );
    
    if(result==SOCKET_ERROR){
        std::cout<< "connection failed!\n";
        closesocket(clientSocket);
        WSACleanup();
        return 1;
    }

    std::cout<< "connected to server successfully! \n";

    const char* message = "Hello !";

    send(
        clientSocket,
        message,
        strlen(message) + 1,
        0
    );

    closesocket(clientSocket);
    WSACleanup();
    
    return 0;
}