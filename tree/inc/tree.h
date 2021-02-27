#ifndef C_IMPLEMENTATION_TREE_H
#define C_IMPLEMENTATION_TREE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <unistd.h>
#include "../../include/vendors/valgrind/valgrind.h"

void custom_free(void **ptr);

typedef enum TypeTree {
    SORTED,
    BALANCED,
} TypeTree;

typedef struct Payload {
    /**
     * The age of the data in this node
     */
    u_int32_t age;
    /**
     * The name of the data in this node
     * WARNING : this pointer could be on the heap or the stack, this is handled,
     * we assume that once you give us a pointer, you don't own it anymore !
     * (well except on the stack but I mean did you really own it at the beginning ;)
     */
    char *name;

} Payload;

typedef struct Node {
    /**
     * Left child of this node
     */
    struct Node *left;
    /**
     * Right child of this node
     */
    struct Node *right;
    /**
    * The height of this node
    */
    uint32_t height;
    /**
    * The age of the data in this node
    */
    uint32_t age;
    /**
     * The name of the data in this node
     */
    char *name;
} Node;


typedef struct Tree {
    Node *root;
    void *heap_start;
    TypeTree typeTree;
} Tree;

/**
 * Dump a tree into a json like string
 *
 * @param tree
 * @return a string
 */
char *tree_dump(Tree *tree);

/**
 * Check if the tree is balanced
 *
 * @param tree
 * @return boolean (0 or 1)
 */
uint8_t sanity_check(Tree *tree);

/**
 * Get the height of a node (only works correctly for balanced tree
 *
 * @param node
 * @return height as uint32
 */
uint32_t get_height(Node *node);

/**
 * Allows to change tree type (only work if the tree has the correct form before hand
 *
 * @param tree
 * @param typeTree
 */
void change_tree_type(Tree *tree, TypeTree typeTree);

/**
 * Find a node according to two specifier
 *
 * @param node
 * @param age
 * @param name
 * @return the node or NULL
 */
Node **node_find(Node **node, int age, char *name);

/**
 * Create a new tree
 *
 * DO NOT MODIFY THE METHOD SIGNATURE
 * @param age The age value for the first data point
 * @param name The name value for the first data point
 * @return The root Node of the tree
 */
Tree *tree_create();

/**
 * Check if a tree is empty
 * @param tree
 * @return 1 if the tree is empty else 0
 */
uint8_t is_empty(Tree *tree);

/**
 * Delete an entire tree. This will delete the passed Node and all children below it
 *
 * DO NOT MODIFY THE METHOD SIGNATURE
 * @param node The root Node of the tree to delete.
 */
void tree_delete(Tree *tree);

/**
 * Insert a new data point into the tree
 *
 * DO NOT MODIFY THE METHOD SIGNATURE
 * @param tree The root node of the tree
 * @param age The age part of the data point
 * @param name The name part of the data point
 */
void tree_insert(Tree *tree, int age, char *name);

/**
 * Remove a data point from a tree
 *
 * DO NOT MODIFY THE METHOD SIGNATURE
 * @param tree The root node of the tree
 * @param age The age part of the data point to delete
 * @param name The name part of the data point to delete
 */
void tree_erase(Tree *tree, int age, char *name);

/**
 * Print a tree in the following format:
 * [<data>, <left>, <right>]
 * where the elements above have the following format:
 *  <data>             {<age:int>: "<name:string>"}
 *  <left>, <right>:   The same format as the root node. When a child node is NULL, the string NULL is to be printed.
 *
 * DO NOT MODIFY THE METHOD SIGNATURE
 * @param tree The root node for the tree to print
 * @param printNewline Whether or not to print a newline
 */
void tree_print(Tree *tree, int printNewline);

/**
 * Find an item in the tree. Should return the tree node containing the data specified if found, or NULL if not
 *
 * DO NOT MODIFY THE METHOD SIGNATURE
 * @param tree The tree to search in
 * @param age The age to look for
 * @param name The name to look for
 * @return
 */
Node *tree_find(Tree *node, int age, char *name);

#endif //C_IMPLEMENTATION_TREE_H
