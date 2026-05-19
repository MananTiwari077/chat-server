#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <cstring>
#include <thread>
#include <vector>
#include <mutex>
#include <algorithm>

struct Client{
    SOCKET socket;
    int id;
};

std::mutex clientsMutex;
int nextClientId=1;
std::vector<Client> clients;

void handleClient(Client client){
    char buffer[1024];

    while(true){
        memset(buffer,0,sizeof(buffer));

        int bytesReceived= recv(
            client.socket,
            buffer,
            sizeof(buffer),
            0
        );

        if(bytesReceived>0){
            std::cout<< "client"<< client.id << ": " << buffer<< "\n";

            {
            std::lock_guard<std::mutex> lock(clientsMutex);
            for(Client otherClient : clients){
                if(otherClient.socket!=client.socket){
                    send(
                        otherClient.socket,
                        buffer,
                        strlen(buffer)+1,
                        0
                    );
                }
            }
            }
        }
        else if(bytesReceived==0){
            break;
        }
        else{
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
        std::cout<< "client " << newClient.id << " connected. \n";

        std::lock_guard<std::mutex> lock(clientsMutex);

        Client newClient;
        newClient.socket=clientSocket;
        newClient.id=nextClientId++;

        {
            std::lock_guard<std::mutex>lock(clientsMutex);
            clients.push_back(newClient);
        }

        std::cout<< "client" << newClient.id << "connected. \n";

        std::thread clientThread(handleClient,newClient);

        clientThread.detach();
    }

    closesocket(serverSocket);
    WSACleanup();

    return 0;
}