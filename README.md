# Multi-Client TCP Chat Server (C++)

A terminal-based multi-client chat application built in C++ using the Winsock2 API. The server supports multiple concurrent users, chat rooms, room history, user management commands, and can be accessed over localhost, a local network (LAN), or the public internet.

---

#  Quick Start

## 1. Build the Project

Run:
./build


---

## 2. Start the Server

Run:
./server


(Leave the server running while clients connect.)

---

# Running the Client

Start the client:

Run:
./client


The client will ask for:

* Server hostname/IP
* Server port
* Username

Choose one of the connection methods below.

---

# Connection Methods

## Option 1 — Same Computer (Localhost)

If both the server and client are running on the same machine:

Host:127.0.0.1
Port:54000


No additional setup is required.

---

## Option 2 — Local Network (LAN)

If the server and client are connected to the same Wi-Fi or local network:

1. Find the server computer's IPv4 address (for example using 'ipconfig' on Windows).

Example:
192.168.1.42


2. On the client enter:

Host:192.168.1.42
Port:54000

No tunneling software is required.

---

## Option 3 — Internet Connection (Using Bore. you will have to setup bore to do)

To allow users outside your local network to connect:

### Start the server
./server


### Start Bore
bore local 54000 --to bore.pub


Bore will print something similar to:
Listening at bore.pub:42871


Share these details with anyone connecting:

Host: bore.pub
Port: 42871


*(The port changes each time Bore starts.)*

Clients simply enter the provided hostname and port when prompted.

---

# Features

* Multi-client TCP server
* Concurrent client handling using threads
* Multiple chat rooms
* Room-specific message history
* Username changes
* List online users
* List active rooms
* Thread-safe shared data using mutexes
* Hostname support using 'getaddrinfo()'
* Supports localhost, LAN, and internet connections

---

# Commands

| Command            | Description                    |
| ------------------ | ------------------------------ |
| '/help'            | Display all available commands |
| '/join <room>'     | Join or create a room          |
| '/leave'           | Leave the current room         |
| '/rooms'           | List all active rooms          |
| '/users'           | List all online users          |
| '/name <new_name>' | Change your username           |
| '/stats            | Display server stats           |
---

# Technologies Used

* C++
* Winsock2
* TCP Sockets
* Multithreading ('std::thread')
* Mutexes ('std::mutex')
* 'getaddrinfo()' for hostname resolution
* Bore (optional, for internet access)

---

# Project Structure

server.cpp      -> Chat server
client.cpp      -> Chat client
build.bat       -> Windows build script
README.md       -> Documentation
