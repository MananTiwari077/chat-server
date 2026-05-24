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
#include <sstream>

std::vector<std::string> chatHistory;

struct Client{
    SOCKET socket;
    int id;
    std::string username;
};

std::atomic<bool> serverRunning(true);
std::mutex clientsMutex;
int nextClientId=1;
std::vector<Client> clients;

void broadcastMessage(const std::string &message, SOCKET senderSocket = INVALID_SOCKET){
    std::lock_guard<std::mutex> lock(clientsMutex);
    for(Client client : clients){
        if(client.socket!=senderSocket){
            std::string formatedMessage= message + "\n";

            int bytesSent= send(
                client.socket,
                formatedMessage.c_str(),
                formatedMessage.length()+1,
                0
            );

            if(bytesSent== SOCKET_ERROR){
                std::cout<< "Failed to send message to "<< client.username<< "\n";
            }
        }
    }
}

void handleClient(Client client){
    char buffer[1024];
    char usernameBuffer[1024];

    recv(client.socket,
        usernameBuffer,
        sizeof(usernameBuffer),
        0
    );

    client.username= usernameBuffer;

    std::cout<< client.username<< " connected. \n";

    {
        std::lock_guard<std::mutex> lock(clientsMutex);
        for(Client &c:clients){
            if(c.socket==client.socket){
                c.username= client.username;
                break;
            }
        }
    }

    broadcastMessage("[Server]: "+client.username+" joined the chat.", client.socket);

    std::string historyBlock;
    {
        std::lock_guard<std::mutex> lock(clientsMutex);

        for (const std::string& oldMessage : chatHistory) {
            historyBlock += oldMessage + "\n";
        }
    }

    if (!historyBlock.empty()) {

        send(
            client.socket,
            historyBlock.c_str(),
            historyBlock.length() + 1,
            0
        );
    }

    while(true){
        memset(buffer,0,sizeof(buffer));

        int bytesReceived= recv(
            client.socket,
            buffer,
            sizeof(buffer),
            0
        );

        if(bytesReceived > 0){
            std::string message= buffer;

            if(!message.empty() && message[0]== '/'){
                if(message=="/list"){
                    std::string userList= "[Server] Online users: ";

                    std::lock_guard<std::mutex> lock(clientsMutex);
                    for(Client c: clients){
                        userList+=c.username + " ";
                    }

                    userList+= "\n";

                    send(client.socket,
                        userList.c_str(),
                        userList.length()+1,
                        0);
                }

                else if(message.rfind("/whisper ", 0)==0){
                    std::stringstream ss(message);
                    
                    std::string command, targetUsername, whisperMessage;

                    ss>> command;
                    ss>> targetUsername;

                    std::getline(ss, whisperMessage);

                    if(!whisperMessage.empty() && whisperMessage[0]==' '){
                        whisperMessage.erase(0,1);
                    }
                    bool userFound = false;

                    std::lock_guard<std::mutex> lock(clientsMutex);
                    for(Client c: clients){
                        if(c.username== targetUsername){
                            std::string privateMessage= "[Whisper] "+client.username+": " + whisperMessage;
                            privateMessage+= "\n";

                            send(
                                c.socket,
                                privateMessage.c_str(),
                                privateMessage.length()+1,
                                0
                            );

                            userFound=true;
                            break;                  
                        }
                    }
                    if(!userFound){
                        std::string errorMessage="[Server] user not found.\n";
                        errorMessage+="\n";

                        send(
                            client.socket,
                            errorMessage.c_str(),
                            errorMessage.length()+1,
                            0
                        );
                    }

                }

            }
             
            else{
                std::string fullMessage= client.username + ": "+ message;
                std::cout<<fullMessage<< "\n";

                {
                    std::lock_guard<std::mutex> lock(clientsMutex);
                    chatHistory.push_back(fullMessage);
                }

                broadcastMessage(fullMessage,client.socket);
            }

        }
        else if(bytesReceived==0){
            break;
        }
        else{

            int error = WSAGetLastError();
            if(error != WSAECONNRESET){
                std::cout<< "receive failed with error: "<< error<< "\n";
            }
            break;
        }
    }
    std::cout<< client.username<< " disconnected. \n";

    broadcastMessage("[Server]: "+client.username+" left the chat.", client.socket);

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