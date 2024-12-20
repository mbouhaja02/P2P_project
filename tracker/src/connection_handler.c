#include "connection_handler.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include "files.h"

void *connectionHandler(void *socket_desc)
{
    peer_info *info = (peer_info *)socket_desc;
    int sock = info->sock;
    char *ip = info->ip;
    char client_message[2000];

    char *response = "ok\n";
    int bytes_received;
    while ((bytes_received = recv(sock, client_message, sizeof(client_message), 0)) > 0)
    {

        if (strncmp(client_message, "announce", 8) == 0)
        {
            printf("< %s\n", client_message);
            WordList bracketWords1 = extractBracketWords(client_message);
            addAnnouncedFile(bracketWords1, ip);
        }
        else if (strncmp(client_message, "look", 4) == 0)
        {
            printf("< %s", client_message);
            WordList bracketWords1 = extractBracketWords(client_message);
            if(bracketWords1.count == 0){
                response = formatAnnouncedFiles();
            }
            else{
                int count = 0;
                char* criteria;
                while(count < 1){
                    criteria = extractFilename(bracketWords1.words[count]);
                    count++;
                }
                response = fileCriteria(criteria);
            }
            puts(response);
            
        }
        else if (strncmp(client_message, "getfile", 7) == 0)
        {   
            // char keyToFind[256];
            // sscanf(client_message, "getfile %s", keyToFind);
            // printf("> peers %s [", keyToFind);
            // Peer *peers = NULL;
            // int peersCount = getPeers(client_message, &peers);
            // if (peersCount > 0 && peers != NULL) {
            //     for (int i = 0; i < peersCount; i++) {
            //         printf("%s:%s ", peers[i].ip_address, peers[i].port);
            //     }
            //     free(peers); 
            // } else {
            //     printf("Aucun fichier trouvé pour la clé donnée ou erreur de mémoire.\n");
            // }
            // printf("]");
            // response = "getfile\n";
            printf("< %s", client_message);
            response = buildPeersResponse(client_message);
            puts(response);
        }
        else
        {
            printf("< %s\n", client_message);
            response = "ok\n";
        }

        memset(client_message, 0, sizeof(client_message));

        send(sock, response, strlen(response), 0);
    }

    if (bytes_received == 0)
    {
        printf("Client disconnected\n");
    }
    else if (bytes_received == -1)
    {
        perror("recv failed");
    }

    close(sock);
    free(socket_desc);

    return NULL;
}
char *trim_spaces(char *str)
{
    size_t len = strlen(str);
    char *start = str;
    char *end = str + len - 1;

    while (start <= end && *start == ' ')
    {
        start++;
    }

    while (end >= start && *end == ' ')
    {
        end--;
    }

    if (start > end)
    {
        *str = '\0';
    }
    else
    {
        size_t new_len = end - start + 1;
        memmove(str, start, new_len);
        str[new_len] = '\0';
    }

    return str;
}

int getfile_cmd(char message[])
{
    const char *espace = strchr(message, ' ');
    if (espace == NULL)
    {
        return -1;
    }
    char key[MAX_WORD_LENGTH + 1];

    strcpy(key, espace + 1);
    key[MAX_WORD_LENGTH] = '\0';
    char *clean_key = trim_spaces(key);
    printf("peers %s [%s]\n", clean_key, find_files(clean_key));

    return 0;
}

//

// void *connectionHandler(void *socket_desc) {
//     int sock = *(int*)socket_desc;
//     char client_message[2000];
//     int bytes_received;
//     while ((bytes_received = recv(sock, client_message, sizeof(client_message), 0)) > 0) {
//         char *response = handleCommand(client_message);
//         if (response != NULL) {
//             send(sock, response, strlen(response), 0);
//             free(response);
//         }
//         memset(client_message, 0, sizeof(client_message));
//     }
//     if (bytes_received == 0) {
//         printf("Client disconnected\n");
//     } else if (bytes_received == -1) {
//         perror("recv failed");
//     }
//     close(sock);
//     free(socket_desc);
//     return NULL;
// }
