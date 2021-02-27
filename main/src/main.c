#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <limits.h>
#include "main.h"
#include "tree.h"

/**
 * Return a malloc strings corresponding to an input till eof or newline
 * @param fp a file pointer (usually stdin)
 * @param size the buffer size at the beginning (use to get in batches)
 * @return
 */
char *inputString(FILE *fp, size_t size) {
    //The size is extended by the input with the value of the provisional
    char *str;
    int ch;
    size_t len = 0;
    str = realloc(NULL, sizeof(char) * size);//size is start size
    if (!str)return str;
    while (EOF != (ch = fgetc(fp)) && ch != '\n') {
        str[len++] = (char) ch;
        if (len == size) {
            str = realloc(str, sizeof(char) * (size += 16));
            if (!str)return str;
        }
    }
    str[len++] = '\0';
    return realloc(str, sizeof(char) * len);
}

/**
 * An exit handler which free all resources used in a single place
 * @param tree A tree used in the program
 * @param command A string used as the command
 */
void exitHandler(Tree *tree, char *command) {
    free(command);
    tree_delete(tree);
    exit(EXIT_FAILURE);
}

// You are allowed to change anything about this function to fix it
int main() {
    Tree *tree = tree_create();
    for (;;) {
        char *commandBuffer = inputString(stdin, 20);
        // Quit on EOF or 'q'
        if (commandBuffer==NULL){
            exitHandler(tree,commandBuffer);
        }
        if (feof(stdin) || strcmp(commandBuffer, "q") == 0) {
            free(commandBuffer);
            break;
        }
        tree = handleString(commandBuffer, tree);
        free(commandBuffer);
    };
    tree_delete(tree);
    return 0;
}

/**
 * Handle a command entered into the program
 *
 * You are allowed to change anything about this function to fix it
 * @param command The command string to handle
 */
Tree *handleString(char command[], Tree *tree) {
    if (command == NULL) {
        fprintf(stderr, "Invalid command; null pointer\n");
        return tree;
    }

    switch (command[0]) {
        case 'i':
            tree=insert(command, tree);
            break;
        case 'e':
            erase(command, tree);
            break;
        case 'c':
            check(command, tree);
            break;
        case 'p':
            tree_print(tree, 1);
            break;
        case 'x':
            tree_delete(tree);
            return NULL;
        default:
            fprintf(stderr, "Invalid command string: %s\n", command);
            break;
    }

    return tree;
}

/**
 * Parsing of the command
 * @param command
 * @param type
 * @return
 */
Payload *handleCommandBuffer(char *command, char *type) {
    uint32_t age;
    char *pch;
    pch = strtok(command, " "); // i
    if (pch == NULL) {
        fprintf(stderr, "No command given\n");
        return NULL;
    }
    if (strcmp(pch, type) != 0) {
        fprintf(stderr, "Command type is not correct\n");
        return NULL;
    }
    pch = strtok(NULL, " "); // age
    if (pch == NULL) {
        fprintf(stderr, "No age was given\n");
        return NULL;
    }
    char *endptr;
    errno = 0;
    long val = strtol(pch, &endptr, 10);
    if ((errno == ERANGE && (val == LONG_MAX || val == LONG_MIN)) || (errno != 0 && val == 0)) {
        perror("strtol");
        return NULL;
    }

    if (endptr == pch) {
        fprintf(stderr, "No digits were found\n");
        return NULL;
    }
    if (val >= UINT32_MAX || val < 0) {
        fprintf(stderr, "Outside of range\n");
        return NULL;
    }
    age = (uint32_t) val;
    pch = strtok(NULL, " "); // name
    if (pch == NULL) {
        fprintf(stderr, "No name was given\n");
        return NULL;
    }
    char *name = malloc(sizeof(char) * (strlen(pch) + 1));
    if (name==NULL){
        fprintf(stderr, "Malloc failed name\n");
        return NULL;
    }
    strcpy(name, pch);
    Payload *payload = (Payload *) malloc(sizeof(Payload));
    if (payload==NULL){
        free(name);
        fprintf(stderr, "Malloc failed payload\n");
        return NULL;
    }
    payload->age = age;
    payload->name = name;
    return payload;
}

void free_payload(Payload **payload) {
    (*payload)->age = UINT32_MAX;
    free((*payload)->name);
    (*payload)->name = NULL;
    free(*payload);
    *payload = NULL;
}

// You are allowed to change anything about this function to fix it
Tree *insert(char *command, Tree *tree) {

    Payload *payload = handleCommandBuffer(command, "i");
    if (payload == NULL) {
        exitHandler(tree, command);
    }
    if (tree == NULL) {
        tree = tree_create();
    }
    tree_insert(tree, payload->age, payload->name);
    free_payload(&payload);
    return tree;
}


// You are allowed to change anything about this function to fix it
void erase(char *command, Tree *tree) {
    Payload *payload = handleCommandBuffer(command, "e");
    if (payload == NULL) {
        exitHandler(tree, command);
    }
    tree_erase(tree, payload->age, payload->name);
    free_payload(&payload);
}

// You are allowed to change anything about this function to fix it
void check(char *command, Tree *tree) {
    Payload *payload = handleCommandBuffer(command, "c");
    if (payload == NULL) {
        exitHandler(tree, command);
    }
    Node *result = tree_find(tree, payload->age, payload->name);
    free_payload(&payload);
    if (result) {
        printf("y\n");
    } else {
        printf("n\n");
    }
}
