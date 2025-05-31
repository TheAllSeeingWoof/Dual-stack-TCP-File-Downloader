# Dual-stack-TCP-File-Downloader


This is a file transfer system that implements a client-server architecture with support for both IPv4 and IPv6 protocols. Here are the key components and features:
Server Component:
Uses dual-stack sockets (supports both IPv4 and IPv6)
Implements multi-threading to handle multiple client connections
Includes file encryption functionality before sending files

Client Components:
Two separate client implementations:
Client_IPv4: For IPv4 connections
Client_IPv6: For IPv6 connections

Both clients can:
Connect to the server
Request file downloads
Decrypt received files
Handle file transfer operations

Security Features:
Implements a simple monoalphabetic cipher for file encryption/decryption
Files are encrypted before transmission and decrypted after reception

Communication Flow:
Client connects to server on port 27015
Client requests a port number for file transfer
Server sends a new port number (27016)
Client connects to the new port
Client requests file download
Server encrypts and sends the file
Client receives, decrypts, and saves the file

Technical Details:
Built using Windows Socket API (Winsock2)
Uses TCP for reliable file transfer
Implements pthread for multi-threading
Includes error handling and connection management

