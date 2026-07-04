#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <string>
#include <thread>

void receiveMessages(SOCKET clientSocket){
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
            std::cout<< "\nMessage: "<< buffer << "\n";
        }
        else{
            break;
        }
    }
}

int main(){
    WSADATA wsaData;
    int wsaStartupResult = WSAStartup(MAKEWORD(2,2),&wsaData);

    if(wsaStartupResult!=0){
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
    
    std::string serverAddressInput;

    std::cout<< "Enter server IP or hostname: ";
    std::getline(std::cin, serverAddressInput);

    addrinfo hints{};
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    addrinfo* result = nullptr;

    std::string port;

    std::cout << "Enter server port: ";
    std::getline(std::cin, port);

    int addressResult = getaddrinfo(
        serverAddressInput.c_str(),
        port.c_str(),
        &hints,
        &result
    );

    if (addressResult != 0) {
    std::cout << "Invalid server address or hostname!\n";
    closesocket(clientSocket);
    WSACleanup();
    return 1;
    }

    int connectResult = connect(
        clientSocket,
        result->ai_addr,
        static_cast<int>(result->ai_addrlen)
    );

    freeaddrinfo(result);
    
    if(connectResult==SOCKET_ERROR){
        std::cout<< "connection failed! "<< WSAGetLastError() << "\n";
        closesocket(clientSocket);
        WSACleanup();
        return 1;
    }

    std::cout<< "connected to server successfully! \n";

    std::string username;
    std::cout<< "Enter your username:";
    std::getline(std::cin,username);

    send(
        clientSocket,
        username.c_str(),
        username.length()+1,
        0
    );

    std::thread receiveThread(receiveMessages,clientSocket);
    receiveThread.detach();

    std::string message;

    while(true){
        std::getline(std::cin,message);
        if(message=="exit"){
            break;
        }
        

        
        send(
            clientSocket,
            message.c_str(),
            message.length()+1,
            0
        );
    }

    closesocket(clientSocket);
    WSACleanup();
    
    return 0;
}