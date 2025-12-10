# minitalk

The purpose of this project is to code a small data exchange program using UNIX signals. It involves creating a client and a server.

## Overview
The client communicates a string of characters to the server. The server receives the string and prints it. The challenge is that communication must be done using only two signals: `SIGUSR1` and `SIGUSR2`.

## Features
- **Client**: Takes the server's PID and the string to send as arguments. Converts the string into bits and sends them as signals.
- **Server**: Prints its PID on startup and waits for incoming signals. Reconstructs the string from the received signals and prints it.
- **Signal Handling**: Uses `sigaction` to intercept signals.

## Usage
1. Compile the programs:
   ```bash
   make
   ```
2. Start the server:
   ```bash
   ./server
   ```
   (Copy the displayed PID)
3. Run the client with the PID and a message:
   ```bash
   ./client <PID> "Hello, Minitalk!"
   ```
