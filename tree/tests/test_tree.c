/**
 * This file exists to help you check if your tree is working correctly. You can run it with the following command line:
 *
 * You are allowed to add or remove parts of the test code if you wish. The contents of this file will not be used to
 * grade your assignment.
 */

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include "tree.h"

// You are allowed to change anything about this function to your preferences
int main(){
    Tree* tree = tree_create();

    printf("Checking basic functionality doesn't crash...\n");
    tree_insert(tree, 42,"Peter" );
    tree_insert(tree, 21, "Joanna");
    tree_insert(tree, 83, "Margareth");

    printf("Checking tree print method...\n");
    tree_print(tree, 1);

    printf("Testing find method...\n");
    if (!tree_find(tree, 83, "Margareth")){
        fprintf(stderr, "Could not find an item that was recently inserted\n");
        return 1;
    }

    printf("Testing erase method...\n");
    tree_erase(tree, 83, "Margareth");
    if(tree_find(tree, 83, "Margareth")){
        fprintf(stderr, "Found an item that was supposed to be removed (leaf node)\n");
        return 3;
    }
    tree_insert(tree, 83, "Margareth");
    tree_erase(tree, 42, "Peter");
    if(tree_find(tree, 42, "Peter")){
        fprintf(stderr, "Found an item that was supposed to be removed (node with children)\n");
        return 4;
    }

    printf("Stress testing your tree (to help you check for possible memory errors)");

    // This makes the results reproducable
    srand(0xC0FFEE);
    for (int i = 0; i < 10000; i++){
        int age = (int) (random() % 1000);
        char* name = malloc(sizeof(char) * 10);
        sprintf(name, "Name%d", age);

        if (tree_find(tree, age, name)){
            tree_erase(tree, age, name);
        } else {
            tree_insert(tree, age, name);
        }
        free(name);
    }
    tree_print(tree, 1);

    // Feel free to add your own tests here or somewhere else in this file
    tree_delete(tree);
    printf("The test succeeded\n");
    return 0;
}



