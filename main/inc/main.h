#ifndef C_IMPLEMENTATION_MAIN_H

#include "tree.h"

/**
 * Takes a command and a tree and ensures the right action is taken
 *
 * You are allowed to change anything about this function to your preferences
 * @param command The command entered by the user
 * @param tree The tree currently being operated on
 */
Tree* handleString(char command[], Tree *tree);

/**
 * Handles insertion of data into a tree
 *
 * You are allowed to change anything about this function to your preferences
 * @param command The command given by the user
 * @param tree The tree currently being operated on
 * @return The tree passed as the input tree
 */
Tree* insert(char* command, Tree* tree);
/**
 * Handles erasing a single node from a tree
 *
 * You are allowed to change anything about this function to your preferences
 * @param command The command entered by the user
 * @param tree The tree currently being operated on
 * @return The tree passed as the input tree
 */
void erase(char* command, Tree* tree);
/**
 * Checks if an entry exists in the tree. If it does, it prints "y\n". If it doesn't, it prints "n\n".
 *
 * You are allowed to change anything about this function to your preferences
 * @param command The command entered by the user
 * @param tree The tree currently being operated on
 * @return The tree passed as the input tree
 */
void check(char* command, Tree* tree);

#define C_IMPLEMENTATION_MAIN_H

#endif //C_IMPLEMENTATION_MAIN_H
