#pragma once

#include <cstring>
#include <iostream>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <unistd.h>
#include <errno.h>

#include "game.h"

int Q(int n, char * err) {
    if (n == -1) {
        perror(err); exit(1);
    }
    return n;
}

void client() {

    // creating socket
    int sender = socket(AF_INET, SOCK_STREAM | O_NONBLOCK, 0);
    int listener = socket(AF_INET, SOCK_STREAM, 0);
    // make listener nonblocking
    int flags = fcntl(listener, F_GETFL);
    fcntl(listener, F_SETFL, flags | O_NONBLOCK);
    if (sender == -1 || listener == -1) {
        perror("Failure creating TCP connection");
        return;
    }

    // specifying address
    sockaddr_in clientAddress;
    clientAddress.sin_family = AF_INET;
    clientAddress.sin_port = htons(8000);
    in_addr c_addr;
    clientAddress.sin_addr.s_addr = inet_aton("127.0.0.1", &c_addr);

    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(8080);
    in_addr s_addr;
    serverAddress.sin_addr.s_addr = inet_aton("127.0.0.1", &s_addr);

    bind(listener, (struct sockaddr*)&serverAddress,
         sizeof(serverAddress));
    listen(listener, 5);

    while (true) {
        // we either want to send a message that the door was open
        // or check if we've received the disc
        if (door_open) {
            // sending connection request
            connect(sender, (struct sockaddr*)&serverAddress,
                    sizeof(serverAddress));

            // sending data
            const char* message = "open";
            send(sender, message, strlen(message), 0);

            // closing socket
            close(sender);
            break;
        } else {
            // checking for data
            int accepted = accept(listener, nullptr, nullptr);
            // If we actually have a connection
            if (!(errno == EAGAIN || errno == EWOULDBLOCK)) {
                // recieving data
                char buffer[1024] = { 0 };
                recv(accepted, buffer, sizeof(buffer), 0);
                // disc has been inserted
                if (buffer == "disc") {
                    // safe since atomic
                    decrypt_unlocked = true;
                }
            }
            close(accepted);
        };
    }

    close(listener);
}

