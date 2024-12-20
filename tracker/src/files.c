#include "files.h"

AnnouncedFile announced_files[100];
int num_announced_files = 0;

// Fonction pour parcourir la chaîne de caractères et récupérer les mots entre des délimiteurs donnés
WordList extractBracketWords(const char *str)
{
    WordList list;
    list.count = 0;
    list.port = 0;

    const char *start = strchr(str, '[');
    const char *end;

    if (start != NULL)
    {
        char buffer_0[MAX_WORD_LENGTH];
        strncpy(buffer_0, str, start - str);
        buffer_0[start - str] = '\0';

        char *port_list = strtok(buffer_0, " ");
        while (port_list != NULL)
        {
            if (strcmp(port_list, "listen") == 0)
            {
                port_list = strtok(NULL, " ");
                if (port_list != NULL)
                {
                    list.port = atoi(port_list);
                    break;
                }
            }
            port_list = strtok(NULL, " ");
        }

        end = strchr(start, ']');
        if (end != NULL)
        {
            char buffer[MAX_WORD_LENGTH];
            strncpy(buffer, start + 1, end - start - 1);
            buffer[end - start - 1] = '\0';

            char *token = strtok(buffer, " ");
            while (token != NULL && list.count < MAX_WORDS)
            {
                strcpy(list.words[list.count], token);
                list.count++;
                token = strtok(NULL, " ");
            }
        }
    }

    return list;
}

char *ipAdresse(int num)
{
    char *result = malloc(17 * sizeof(char));
    if (result == NULL)
    {
        fprintf(stderr, "Erreur d'allocation mémoire\n");
        exit(EXIT_FAILURE);
    }

    snprintf(result, 17, "1.1.1.%d", num + 1);
    return result;
}

void addAnnouncedFile(WordList list, char* ip)
{
    if (num_announced_files < 100)
    {
        AnnouncedFile new_file;
        for (int i = 0; i < list.count; i += 4)
        {
            strcpy(new_file.filename, list.words[i]);
            new_file.length = atoi(list.words[i + 1]);
            new_file.pieceSize = atoi(list.words[i + 2]);
            strcpy(new_file.key, list.words[i + 3]);
            new_file.port = list.port;
            strcpy(new_file.ip, ip);
            strcpy(new_file.ip, ipAdresse(num_announced_files));
            announced_files[num_announced_files] = new_file;
            num_announced_files++;
        }
    }
}

char *formatAnnouncedFiles()
{
    char *result = (char *)malloc(MAX_BUFFER_SIZE * sizeof(char));

    if (result == NULL)
    {
        fprintf(stderr, "Erreur lors de l'allocation de mémoire.\n");
        exit(EXIT_FAILURE);
    }

    result[0] = '\0';

    strcat(result, "list [");

    for (int i = 0; i < num_announced_files; i++)
    {
        char fileEntry[MAX_BUFFER_SIZE];
        snprintf(fileEntry, sizeof(fileEntry), "%s %u %u %s",
                 announced_files[i].filename,
                 announced_files[i].length,
                 announced_files[i].pieceSize,
                 announced_files[i].key);

        strcat(result, fileEntry);
        strcat(result, " ");
    }

    result[strlen(result) - 1] = ']';
    strcat(result, "\n");

    return result;
}

char *find_files(char *key)
{
    size_t result_size = MAX_BUFFER_SIZE;
    char *result = (char *)malloc(result_size);
    if (result == NULL)
    {
        fprintf(stderr, "Erreur lors de l'allocation de mémoire.\n");
        exit(EXIT_FAILURE);
    }
    result[0] = '\0';

    for (int i = 0; i < num_announced_files; i++)
    {
        if (strncmp(announced_files[i].key, key, strlen(announced_files[i].key)) == 0)
        {
            char fileEntry[MAX_BUFFER_SIZE];
            snprintf(fileEntry, MAX_BUFFER_SIZE, "%s: %d", announced_files[i].ip, announced_files[i].port);
            size_t entry_size = strlen(fileEntry) + 1;

            if (strlen(result) + entry_size > result_size)
            {
                result_size *= 2;
                char *tmp = (char *)realloc(result, result_size);
                if (tmp == NULL)
                {
                    fprintf(stderr, "Erreur lors de la réallocation de mémoire.\n");
                    free(result);
                    exit(EXIT_FAILURE);
                }
                result = tmp;
            }

            strcat(result, fileEntry);
            strcat(result, " ");
        }
    }
    strcat(result, "\n");

    return result;
}

// Fonction pour extraire les adresses IP, les numéros de port et les clés des pairs à partir d'une chaîne de caractères
int getPeers(const char* command, Peer** peers) {
    char keyToFind[256];
    int foundCount = 0;

    if (sscanf(command, "getfile %s", keyToFind) != 1) {
        *peers = NULL;
        return 0;
    }
    
    for (int i = 0; i < num_announced_files; i++) {
        if (strcmp(announced_files[i].key, keyToFind) == 0) {
            foundCount++;
        }
    }

    if (foundCount == 0) {
        *peers = NULL;
        return 0;
    }

    *peers = (Peer*)malloc(sizeof(Peer) * foundCount);
    if (*peers == NULL) {
        return 0; 
    }

    Peer* peer = *peers;
    int j = 0;
    for (int i = 0; i < num_announced_files && j < foundCount; i++) {
        if (strcmp(announced_files[i].key, keyToFind) == 0) {
            snprintf(peer[j].port, sizeof(peer[j].port), "%u", announced_files[i].port);
            strncpy(peer[j].ip_address, announced_files[i].ip, sizeof(peer[j].ip_address));
            j++;
        }
    }

    return foundCount; 
}

char* buildPeersResponse(const char* client_message) {
    char keyToFind[256];
    sscanf(client_message, "getfile %s", keyToFind);
    Peer *peers = NULL;
    int peersCount = getPeers(client_message, &peers);
    char* response = NULL;

    response = (char*)malloc(MAX_BUFFER_SIZE);
    if (!response) {
        return NULL;
    }
    
    response[0] = '\0';

    snprintf(response, MAX_BUFFER_SIZE, "peers %s [", keyToFind);

    char entry[100]; 
    if (peersCount > 0 && peers != NULL) {
        for (int i = 0; i < peersCount; i++) {
            snprintf(entry, sizeof(entry), "%s:%s ", peers[i].ip_address, peers[i].port);
            strcat(response, entry);
        }
    } else {
        strcat(response, "Aucun fichier trouvé pour la clé donnée ou erreur de mémoire.");
    }
    strcat(response, "]\n");

    return response;
}


void printWordList(const WordList* wl) {
    if (wl == NULL) {
        printf("WordList pointer is null.\n");
        return;
    }

    printf("Words in the list:\n");
    for (int i = 0; i < wl->count; i++) {
        printf("%d: %s\n", i + 1, wl->words[i]);
    }
    printf("Total words: %d\n", wl->count);
    printf("Port: %u\n", wl->port);
}

char* extractFilename(const char* input) {
    const char *start = strchr(input, '"');
    const char *end;

    if (start != NULL) {
        start++; 
        end = strchr(start, '"');
        if (end != NULL) {
            size_t length = end - start;
            char* filename = (char*)malloc(length + 1);
            if (filename != NULL) {
                strncpy(filename, start, length);
                filename[length] = '\0'; 
                return filename;
            }
        }
    }
    return NULL; 
}


char *fileCriteria(char* criteria)
{
    char *result = (char *)malloc(MAX_BUFFER_SIZE * sizeof(char));

    if (result == NULL)
    {
        fprintf(stderr, "Erreur lors de l'allocation de mémoire.\n");
        exit(EXIT_FAILURE);
    }

    result[0] = '\0';

    strcat(result, "list [");

    for (int i = 0; i < num_announced_files; i++)
    {
        if(strcmp(announced_files[i].filename, criteria)==0){
            char fileEntry[MAX_BUFFER_SIZE];
            snprintf(fileEntry, sizeof(fileEntry), "%s %u %u %s",
                 announced_files[i].filename,
                 announced_files[i].length,
                 announced_files[i].pieceSize,
                 announced_files[i].key);

            strcat(result, fileEntry);
            strcat(result, " ");
        }
    }

    result[strlen(result) - 1] = ']';
    strcat(result, "\n");

    return result;
}

