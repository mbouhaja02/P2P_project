
#include "tracker.h"
int socket_desc; 



void signal_handler(int sig) {
    printf("Fermeture propre du serveur\n");
    close(socket_desc);
    exit(0);
}

void startTracker() {
    write_assigned_port_to_config();
    int port;
    FILE *config_file = fopen("config.ini", "r");

    if (config_file == NULL) {
        perror("Erreur lors de l'ouverture de config.ini");
        return;
    }

    char line[100]; // Taille suffisante pour stocker une ligne du fichier config.ini

    while (fgets(line, sizeof(line), config_file) != NULL) {
        //printf("Ligne lue depuis config.ini : %s", line); // Débogage

        // Vérifier si la ligne contient "tracker-port"
        if (strstr(line, "tracker-port") != NULL) {
            // Si la ligne contient "tracker-port", essayer de lire le port
            if (sscanf(line, "tracker-port = %d", &port) == 1) {
                //printf("Port lu depuis config.ini : %d\n", port); // Débogage
                break; // Sortir de la boucle si le port est lu avec succès
            } else {
                printf("Erreur lors de la lecture du port depuis config.ini\n");
            }
        }
    }
    printf("le port est : %d\n", port);

    fclose(config_file);

    int  client_sock, c, *new_sock;
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
    printf("Waiting for incoming connections...\n");
    c = sizeof(struct sockaddr_in);
    
    while (1) {
        
        client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c);
        if (client_sock < 0) {
            perror("accept failed");
            continue;
        }
        peer_info *info = malloc(sizeof(peer_info));
        if (info == NULL) {
            perror("malloc failed");
            close(client_sock);
            continue;
        }
        info->sock = client_sock;
        pthread_t sniffer_thread;
        /*if (new_sock == NULL) {
            perror("malloc failed");
            close(client_sock);
            continue;
        }*/
        info->sock = client_sock;
        strncpy(info->ip, inet_ntoa(client.sin_addr), INET_ADDRSTRLEN);
        if (pthread_create(&sniffer_thread, NULL, connectionHandler, (void*) info) < 0) {
            perror("could not create thread");
            close(client_sock);
            free(info);
            continue;
        }

        pthread_detach(sniffer_thread);
    }

    close(socket_desc);
}


int main() {
    signal(SIGINT, signal_handler);
    startTracker(); 
    return 0;
}
