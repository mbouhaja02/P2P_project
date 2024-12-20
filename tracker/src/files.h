#ifndef FILES_H
#define FILES_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include <signal.h>

#define MAX_WORD_LENGTH 500 // Longueur maximale d'un mot
#define MAX_WORDS 1000 // Nombre maximal de mots
#define MAX_BUFFER_SIZE 10000

#define MAX_PEERS 100
#define MAX_IP_LENGTH 16
#define MAX_KEY_LENGTH 32


typedef struct {
    char port[256];
    char ip_address[256];
} Peer;

// Structure pour stocker une liste de mots
typedef struct {
    char words[MAX_WORDS][MAX_WORD_LENGTH];
    int count;
    unsigned int port;
} WordList;

typedef struct {
    char filename[256];
    unsigned int length;
    unsigned int pieceSize;
    char key[256];
    unsigned int port;
    char ip[256];
} AnnouncedFile;



WordList extractBracketWords(const char *str);

void addAnnouncedFile(WordList list, char* ip);

char *formatAnnouncedFiles();

int getPeers(const char* message, Peer ** peers);

char* buildPeersResponse(const char* client_message);

char* find_files(char* key);

void printWordList(const WordList* wl);

char* extractFilename(const char* input);

char *fileCriteria(char* criteria);

#endif