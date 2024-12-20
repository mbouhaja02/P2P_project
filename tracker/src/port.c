#include "port.h"
#include <unistd.h>

int find_free_port() {
    int temp_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (temp_socket == -1) {
        perror("Erreur lors de la création du socket");
        exit(EXIT_FAILURE);
    }

    // Création de l'adresse de test
    struct sockaddr_in temp_address;
    memset(&temp_address, 0, sizeof(temp_address));
    temp_address.sin_family = AF_INET;
    temp_address.sin_addr.s_addr = htonl(INADDR_LOOPBACK); // Adresse locale
    temp_address.sin_port = 0; // Utilise un port disponible

    // Association du socket temporaire avec l'adresse de test
    if (bind(temp_socket, (struct sockaddr *)&temp_address, sizeof(temp_address)) == -1) {
        perror("Erreur lors de la liaison du socket avec l'adresse de test");
        exit(EXIT_FAILURE);
    }

    // Récupération du numéro de port attribué par le système
    struct sockaddr_in temp_address_info;
    socklen_t temp_address_info_length = sizeof(temp_address_info);
    if (getsockname(temp_socket, (struct sockaddr *)&temp_address_info, &temp_address_info_length) == -1) {
        perror("Erreur lors de la récupération du numéro de port attribué par le système");
        exit(EXIT_FAILURE);
    }

    int port = ntohs(temp_address_info.sin_port);

    // Fermeture du socket temporaire
    close(temp_socket);

    return port;
}

void write_assigned_port_to_config() {
    FILE *config_file = fopen("config.ini", "r+");
    if (config_file == NULL) {
        printf("Erreur lors de l'ouverture de config.ini");
        return;
    }

    char line[100]; // Taille suffisante pour stocker une ligne du fichier config.ini

    // Créer une chaîne de caractères contenant "tracker-port ="
    char* search_str = "tracker-port =";

    // Rechercher la ligne contenant "tracker-port =" et mettre à jour le port
    while (fgets(line, sizeof(line), config_file) != NULL) {
        if (strstr(line, search_str) != NULL) {
            // Mettre à jour la valeur de "tracker-port" avec la nouvelle valeur retournée par find_free_port()
            fseek(config_file, -strlen(line), SEEK_CUR); // Reculer jusqu'au début de la ligne
            fprintf(config_file, "tracker-port = %d\n", find_free_port()); // Écrire la nouvelle valeur
            break; // Sortir de la boucle une fois que le port est mis à jour
        }
    }

    fclose(config_file);
}

// int main() {
//     int port = find_free_port();
//     printf("Port disponible : %d\n", port);

//     return 0;
// }
