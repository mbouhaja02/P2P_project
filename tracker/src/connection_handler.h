#ifndef CONNECTION_HANDLER_H
#define CONNECTION_HANDLER_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include <signal.h>

typedef struct {
    int sock;
    char ip[INET_ADDRSTRLEN];
} peer_info;

void *connectionHandler(void *socket_desc);

#endif
