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

## Option 3 - Connecting over the internet with a third party tunnerling service.

The chat server is built using **TCP sockets with Winsock** and already supports network-based client-server communication. The server listens on **port 54000**, and clients can connect using the server's IP address and port.

For connections over the **public internet**, the local server needs to be exposed through a TCP tunnel. This project does not depend on a specific tunneling provider; services such as **Pinggy, Bore, ngrok, or similar TCP tunneling solutions** can be used for this purpose.

For example, with Pinggy, the server can be exposed using:

```bash
ssh -p 443 -R0:127.0.0.1:54000 tcp@free.pinggy.io
```

The tunnel provides a public TCP address that can then be entered by a remote client.

The connection flow is:

```text
Remote Client
     ↓
Public TCP Tunnel
     ↓
Local Machine :54000
     ↓
C++ TCP Chat Server
```

The tunneling service is **not part of the chat server itself**. It only provides a route from the public internet to the locally running TCP server.

> **Note:** Third-party tunneling services are external infrastructure and their availability, commands, limits, and free-tier policies may change over time. If the example above no longer works, use the current TCP tunneling instructions provided by the service you choose.
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


---

# Project Structure

server.cpp      -> Chat server
client.cpp      -> Chat client
build.bat       -> Windows build script
README.md       -> Documentation
