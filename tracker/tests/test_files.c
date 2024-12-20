#include "../src/files.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>


void test_extractBracketWords() {
    const char *input = " announce listen 35829 seed [file2.txt 2 1024 350867dc file.txt 5 1024 e5d5e83f file3.txt 14 1024 7efaaf5d] leech [jb sfbsd 53151 51651 516] ]";

    WordList bracketWords = extractBracketWords(input);
    
    assert(bracketWords.count == 12);
    assert(strcmp(bracketWords.words[0], "file2.txt") == 0);
    assert(strcmp(bracketWords.words[1], "2") == 0);
    assert(strcmp(bracketWords.words[2], "1024") == 0);
    assert(strcmp(bracketWords.words[3], "350867dc") == 0);
    assert(strcmp(bracketWords.words[4], "file.txt") == 0);
    assert(strcmp(bracketWords.words[5], "5") == 0);
    assert(strcmp(bracketWords.words[6], "1024") == 0);
    assert(strcmp(bracketWords.words[7], "e5d5e83f") == 0);
    assert(bracketWords.port == 35829);

    printf("Test extractBracketWords successful.\n");
}

void test_addAnnouncedFile() {
    AnnouncedFile announced_files[100];
    int num_announced_files = 0;
    WordList list1;
    list1.count = 4;
    strcpy(list1.words[0], "file1.txt");
    strcpy(list1.words[1], "100");
    strcpy(list1.words[2], "1024");
    strcpy(list1.words[3], "key1");
    list1.port = 12345;
    addAnnouncedFile(list1);

    // Assert the filename after adding
    printf("filename: %s  \n", announced_files[0].filename);
    assert(strcmp(announced_files[0].filename, "file1.txt") == 0);

    WordList list2;
    list2.count = 4;
    strcpy(list2.words[0], "file2.txt");
    strcpy(list2.words[1], "200");
    strcpy(list2.words[2], "2048");
    strcpy(list2.words[3], "key2");
    list2.port = 54321;
    addAnnouncedFile(list2);

    // Assert the filename after adding the second file
    assert(strcmp(announced_files[1].filename, "file2.txt") == 0);

    // Adding more files to fill the announced_files array
    for (int i = 2; i < 100; ++i) {
        WordList list;
        list.count = 4;
        sprintf(list.words[0], "file%d.txt", i + 1);
        sprintf(list.words[1], "%d", (i + 1) * 100);
        sprintf(list.words[2], "%d", (i + 1) * 1024);
        sprintf(list.words[3], "key%d", i + 1);
        list.port = 54321 + i;
        addAnnouncedFile(list);

        // Assert the filename after adding the file
        assert(strcmp(announced_files[i].filename, list.words[0]) == 0);
    }

    // Adding one more file should not exceed the array size
    WordList list_full;
    list_full.count = 4;
    strcpy(list_full.words[0], "file_full.txt");
    strcpy(list_full.words[1], "9999");
    strcpy(list_full.words[2], "1024");
    strcpy(list_full.words[3], "key_full");
    list_full.port = 65535;
    addAnnouncedFile(list_full);

    // Assert that the number of announced files does not exceed the array size
    assert(num_announced_files == 100);
}

void test_formatAnnouncedFiles() {
    // Case: No announced files
    num_announced_files = 0;
    char *formattedFiles = formatAnnouncedFiles();
    assert(strcmp(formattedFiles, "") == 0);
    free(formattedFiles);

    // Case: One announced file
    num_announced_files = 1;
    strcpy(announced_files[0].filename, "file1.txt");
    announced_files[0].length = 100;
    announced_files[0].pieceSize = 1024;
    strcpy(announced_files[0].key, "key1");
    announced_files[0].port = 12345;

    formattedFiles = formatAnnouncedFiles();
    assert(strcmp(formattedFiles, "Filename: file1.txt, Length: 100, Piece Size: 1024, Key: key1, Port: 12345\n") == 0);
    free(formattedFiles);

    // Case: Multiple announced files
    num_announced_files = 3;
    strcpy(announced_files[1].filename, "file2.txt");
    announced_files[1].length = 200;
    announced_files[1].pieceSize = 2048;
    strcpy(announced_files[1].key, "key2");
    announced_files[1].port = 54321;

    strcpy(announced_files[2].filename, "file3.txt");
    announced_files[2].length = 300;
    announced_files[2].pieceSize = 3072;
    strcpy(announced_files[2].key, "key3");
    announced_files[2].port = 9876;

    formattedFiles = formatAnnouncedFiles();
    assert(strcmp(formattedFiles,
                  "Filename: file1.txt, Length: 100, Piece Size: 1024, Key: key1, Port: 12345\n"
                  "Filename: file2.txt, Length: 200, Piece Size: 2048, Key: key2, Port: 54321\n"
                  "Filename: file3.txt, Length: 300, Piece Size: 3072, Key: key3, Port: 9876\n") == 0);
    free(formattedFiles);
}

int main(){
    test_extractBracketWords();
    //test_addAnnouncedFile();
    // test_formatAnnouncedFiles();
    return 0;
}
