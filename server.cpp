#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <cstring>
#include <thread>
#include <vector>
#include <mutex>
#include <algorithm>
#include <atomic>
#include <string>


struct Client{
    SOCKET socket;
    int id;
    std::string username;
};

std::atomic<bool> serverRunning(true);
std::mutex clientsMutex;
int nextClientId=1;
std::vector<Client> clients;

void handleClient(Client client){
    char buffer[1024];
    char usernameBuffer[1024];

    recv(client.socket,
        usernameBuffer,
        sizeof(usernameBuffer),
        0
    );
    client.username= usernameBuffer;

    std::cout<< client.username<< "connected. \n";

    while(true){
        memset(buffer,0,sizeof(buffer));

        int bytesReceived= recv(
            client.socket,
            buffer,
            sizeof(buffer),
            0
        );

        if(bytesReceived>0){
            std::string fullMessage= client.username + ": "+ buffer;

            std::cout<< fullMessage<< " \n";

            {
            std::lock_guard<std::mutex> lock(clientsMutex);
            for(Client otherClient : clients){
                if(otherClient.socket!=client.socket){
                    int bytesSent= send(
                        otherClient.socket,
                        fullMessage.c_str(),
                        fullMessage.length()+1,
                        0
                    );

                    if(bytesSent==SOCKET_ERROR){
                        std::cout<<"Failed to send message to client "<< otherClient.id << "\n";
                    }
                }
            }
            }
        }
        else if(bytesReceived==0){
            break;
        }
        else{
            std::cout<< "Receive failed! \n";
            break;
        }
    }

    std::cout<< "client "<< client.id<< "disconnected. \n";

    {
        std::lock_guard<std::mutex> lock(clientsMutex);
        clients.erase(
            std::remove_if(
                clients.begin(),
                clients.end(),

                [&](const Client& c){
                return c.socket==client.socket;
            }
        ),
        clients.end()
    );
    }

    closesocket(client.socket);
}

void shutdownServer(){
    std::string command;
    while(true){
        std::getline(std::cin,command);
        if(command=="shutdown"){
            serverRunning=false;
            break;
        }
    }
}

int main() {
    WSADATA wsaData;

    int wsaStartupResult = WSAStartup(MAKEWORD(2, 2), &wsaData);

    if (wsaStartupResult != 0) {
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

    int bindResult = bind(
        serverSocket,
        (sockaddr*)&serverAddress,
        sizeof(serverAddress)
    );

    if (bindResult == SOCKET_ERROR) {
        std::cout << "Bind failed!\n";

        closesocket(serverSocket);
        WSACleanup();

        return 1;
    }

    std::cout << "Bind successful!\n";

    int listenResult = listen(serverSocket, SOMAXCONN);
    if(listenResult== SOCKET_ERROR){
        std::cout<< "Listen failed! \n";
        closesocket(serverSocket);
        WSACleanup();

        return 1;
    }
    std::cout<< "server is listening on port 54000! \n";

    std::thread shutdownThread(shutdownServer);
    shutdownThread.detach();

    while(serverRunning){
        
        SOCKET clientSocket= accept(
            serverSocket,
            nullptr,
            nullptr
        );

        if(clientSocket == INVALID_SOCKET){
            std::cout<< "Accept failed! \n";
            continue;
        }

        Client newClient;
        newClient.socket=clientSocket;
        newClient.id=nextClientId++;

        {
            std::lock_guard<std::mutex>lock(clientsMutex);
            clients.push_back(newClient);
        }


        std::thread clientThread(handleClient,newClient);

        clientThread.detach();
    }

    closesocket(serverSocket);
    WSACleanup();

    return 0;
}