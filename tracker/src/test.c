#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_WORD_LENGTH 50 // Longueur maximale d'un mot
#define MAX_WORDS 100 // Nombre maximal de mots

// Structure pour stocker une liste de mots
typedef struct {
    char words[MAX_WORDS][MAX_WORD_LENGTH];
    int count;
} WordList;

// Fonction pour extraire les mots entre des crochets
WordList extractBracketWords(const char *str) {
    WordList list;
    list.count = 0;

    const char *start = strchr(str, '['); // Recherche du premier délimiteur ouvrant
    const char *end;
    
    if (start != NULL) {
        end = strchr(start, ']'); // Recherche du premier délimiteur fermant à partir du début
        if (end != NULL) {
            // Copie de la sous-chaîne entre les délimiteurs dans un tampon temporaire
            char buffer[MAX_WORD_LENGTH];
            strncpy(buffer, start + 1, end - start - 1);
            buffer[end - start - 1] = '\0';

            // Découpage de la sous-chaîne en mots séparés par des espaces
            char *token = strtok(buffer, " ");
            while (token != NULL && list.count < MAX_WORDS) {
                strcpy(list.words[list.count], token);
                list.count++;
                token = strtok(NULL, " ");
            }
        }
    }

    return list;
}

int main() {
    const char *input = "dhfbds[jjfdnv jf djfgn fg efef efzef zefzef aaa aaa aaa zefzef zefzef  fg] dfbjdfjksb [ sjdfb jdjsbf jdskbf]";

    // Extraction des mots entre crochets pour le premier groupe
    WordList bracketWords1 = extractBracketWords(input);
    
    printf("Mots entre crochets pour le premier groupe: \n");
    for (int i = 0; i < bracketWords1.count; i++) {
        printf("%s\n", bracketWords1.words[i]);
    }

    // Extraction des mots entre crochets pour le deuxième groupe
    const char *input2 = strchr(input, ']') + 1; // Décalage pour sauter le premier groupe et chercher le deuxième
    WordList bracketWords2 = extractBracketWords(input2);
    
    printf("\nMots entre crochets pour le deuxième groupe: \n");
    for (int i = 0; i < bracketWords2.count; i++) {
        printf("%s\n", bracketWords2.words[i]);
    }

    return 0;
}
