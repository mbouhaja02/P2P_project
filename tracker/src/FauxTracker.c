#include "connection_handler.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <stdbool.h>
#include <string.h> // Inclure l'en-tête pour strlen
#include <sys/socket.h>
#include <netinet/in.h> // Inclure l'en-tête pour struct sockaddr_in
#include <arpa/inet.h> // Inclure l'en-tête pour htons

// Fonction de démarrage du faux tracker
void startFakeTracker(int port) {
    int socket_desc, client_sock, c, *new_sock;
    struct sockaddr_in server, client;

    socket_desc = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_desc == -1) {
        printf("Could not create socket\n");
        return;
    }
    
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(port);

    if (bind(socket_desc, (struct sockaddr *)&server, sizeof(server)) < 0) {
        perror("bind failed. Error");
        return;
    }

    listen(socket_desc, 3);
    printf("Fake tracker is waiting for incoming connections...\n");
    c = sizeof(struct sockaddr_in);
    
    while ((client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c))) {
        pthread_t sniffer_thread;
        new_sock = malloc(sizeof(int));
        if (!new_sock) {
            perror("malloc failed");
            return;
        }
        *new_sock = client_sock;
        
        if (pthread_create(&sniffer_thread, NULL, connectionHandler, (void*) new_sock) < 0) {
            perror("could not create thread");
            return;
        }
        
        pthread_detach(sniffer_thread);
    }
    
    if (client_sock < 0) {
        perror("accept failed");
        return;
    }
}

int main() {
    startFakeTracker(12342); 
    return 0;
}
