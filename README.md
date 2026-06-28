# Multi-Client Chat Server (C++)

> [!IMPORTANT]
>
> ##  Quick Start
>
> This project is built for **Windows** using **WinSock2**.
>
> ### 1. Build the project
>
> ```powershell
> ./build
> ```
>
> ### 2. Start the server
>
> ```powershell
> ./server
> ```
>
> ### 3. Start one or more clients
>
> Open a new terminal for each client and run:
>
> ```powershell
> ./client
> ```
>
> You're now ready to chat!

---

## Overview

A multithreaded terminal-based chat server built in **C++** using **TCP sockets** and **WinSock2**.

The server supports multiple clients simultaneously, private messaging, multiple chat rooms, room history, username management, and various utility commands while ensuring thread safety through mutex synchronization.

This project was built to strengthen my understanding of:

* Socket Programming
* Multithreading
* Synchronization using Mutexes
* Client-Server Architecture
* TCP Communication
* Command Parsing
* Shared Resource Management

---

# Features

*  Real-time multi-client communication
*  Multiple chat rooms
*  Room-specific message history
*  Online user listing
*  Thread-safe shared resources using mutexes
*  Private messaging
*  Username renaming
*  Server statistics
*  Active room statistics
*  Join, leave and rename notifications
*  Duplicate username protection

---

# Supported Commands

| Command                     | Description                    |
| --------------------------- | ------------------------------ |
| `/help`                     | Display all available commands |
| `/list`                     | Show all connected users       |
| `/join <room>`              | Join or create a room          |
| `/rooms`                    | Display all active rooms       |
| `/whisper <user> <message>` | Send a private message         |
| `/rename <new_name>`        | Change your username           |
| `/stats`                    | Display server statistics      |

---

# Technologies Used

* C++17
* WinSock2
* TCP Sockets
* Multithreading (`std::thread`)
* Mutex (`std::mutex`)
* STL Containers

---

# Project Structure

```text
.
├── client.cpp
├── server.cpp
├── build.bat
├── README.md
└── .gitignore
```

---

# Concepts Demonstrated

* TCP Socket Programming
* Concurrent Client Handling
* Thread Synchronization
* Shared Memory Protection
* Command Processing
* Object-Oriented Programming
* Network Programming Fundamentals




