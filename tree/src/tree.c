#include "tree.h"

#define MAX(x, y) ((x)>(y) ? (x) : (y))

/**
 * Please correct the contents of this file to make sure all functions here do what they are supposed to do if you find
 * that they do not work as expected.
 */

void tree_print_node(Node *node);

// Tree function: you are allowed to change the contents, but not the method signature
Tree *tree_create() {
    /**
     * WARNING this function should be called before any memory allocation !
     */
    void *heap_start = ((void *) sbrk(0));
    Tree *tree = (Tree *) malloc(sizeof(Tree));
    if (tree == NULL) {
        fprintf(stderr, "Error allocating the tree");
        return NULL;
    }
    tree->heap_start = heap_start;
    tree->root = NULL;
    tree->typeTree = BALANCED;
    return tree;
}

uint8_t is_empty(Tree *tree) {
    return tree->root == NULL;
}


uint32_t get_height(Node *node) {
    // WARNING this will return a wrong metric if called on a sorted tree
    if (node != NULL) {
        return node->height;
    }
    return 0;
}

uint8_t sanity_check_node(Node *node) {
    if (node != NULL) {
        if (sanity_check_node(node->left) && sanity_check_node(node->right)) {
            if (node->height == 1 + MAX(get_height(node->left), get_height(node->right))) {
                return 1;
            }
        }
        return 0;
    }
    return 1;
}

uint8_t sanity_check(Tree *tree) {
    if (tree != NULL) {
        return sanity_check_node(tree->root);
    }
    // technically the tree is balanced if it doesn't exists but we prefer to guard this case
    return 0;
}

void change_tree_type(Tree *tree, TypeTree typeTree) {
    if (tree == NULL) {
        return;
    }
    if (typeTree == BALANCED) {
        if (sanity_check(tree)) {
            tree->typeTree = BALANCED;
        } else {
            fprintf(stderr, "You tried to modify a tree which is not suited for balanced\n");
        }
    } else {
        tree->typeTree = SORTED;
    }
}

void fix_height(Node *node) {
    if (node == NULL) {
        fprintf(stderr, "fixing the height of a null node is node possible\n");
        return;
    }
    node->height = 1 + MAX(get_height(node->left), get_height(node->right));
}

void rotate_left(Node **root) {
    /**
     * Rotate left the tree, for example rotate C to A (we assume it exists)
     *       A                     C
     *      / \                   / \
     *     B   C      =>         A   E
     *        / \               / \
     *       D   E             B   D
     */
    Node *old_root = *root; // A
    Node *new_root = old_root->right; // C
    Node *old_right_left = new_root->left; // D

    old_root->right = old_right_left; // A->C= D
    new_root->left = old_root; // C->D = A
    *root = new_root; // A = C

    fix_height((*root)->left);
    fix_height(*root);
}

void rotate_right(Node **root) {
    /**
     * Rotate left the tree, for example rotate A to C (we assume it exists)
     *       A                     C
     *      / \                   / \
     *     B   C      <=         A   E
     *        / \               / \
     *       D   E             B   D
     */
    Node *old_root = *root; // C
    Node *new_root = old_root->left; // A
    Node *old_left_right = new_root->right; // D

    old_root->left = old_left_right; // C->A = D
    new_root->right = old_root; // A->D = C
    *root = new_root; // C = A

    fix_height((*root)->right);
    fix_height(*root);
}

void rebalance(Node **node) {
    if (*node != NULL) {
        // left first
        if (get_height((*node)->left) > get_height((*node)->right) + 1) {
            // we are imbalanced (because we have a difference of +2)
            // if left is NULL we get 0 and 0<1+r (r>=0) so we know it isnt
            if (get_height((*node)->left->left) > get_height((*node)->left->right)) {
                // single rotation (easiest case)
                rotate_right(node);
            } else {
                // double rotation
                rotate_left(&(*node)->left);
                rotate_right(node);
            }
        } else if (get_height((*node)->right) > get_height((*node)->left) + 1) {
            // we are imbalanced (because we have a difference of +2)
            // if right is NULL we get 0 and 0<1+r (r>=0) so we know it isnt
            if (get_height((*node)->right->right) > get_height((*node)->right->left)) {
                // single rotation (easiest case)
                rotate_left(node);
            } else {
                // double rotation
                rotate_right(&(*node)->right);
                rotate_left(node);
            }
        } else {
            // update height
            fix_height(*node);
        }
    }
}


void custom_free(void **ptr) {
    free(*ptr);
    *ptr = NULL;
}

// comes from https://stackoverflow.com/a/54078469/9951128
void get_heap_bounds(uint64_t *heap_start, uint64_t *heap_end) {
    FILE *stream;
    char *line = NULL;
    size_t len = 0;
    ssize_t nread;

    stream = fopen("/proc/self/maps", "r");

    while ((nread = getline(&line, &len, stream)) != -1) {
        if (strstr(line, "[heap]")) {
            sscanf(line, "%" SCNx64 "-%" SCNx64 "", heap_start, heap_end);
            break;
        }
    }

    free(line);
    fclose(stream);
}

// comes from https://stackoverflow.com/a/54078469/9951128
int is_heap_var(void *pointer) {
    uint64_t heap_start = 0;
    uint64_t heap_end = 0;
    get_heap_bounds(&heap_start, &heap_end);
    if (pointer >= (void *) heap_start && pointer <= (void *) heap_end) {
        return 1;
    }
    return 0;
}

void free_node_payload(Node **node, void *heap_start) {
    (*node)->age = UINT32_MAX;
    if (RUNNING_ON_VALGRIND) {
        fprintf(stderr, "VALGRIND DETECTED, DEPLOYING COUNTER MEASURE\n");
        // ooh, you are messing with my code !!
        free((*node)->name);
        (*node)->name = NULL;
    } else {
        // WARNING, this will be break with valgrind since it link dynamically and sbrk will not be correct, I cannot be liable if you modify some of the main part of libc
        // if the pointer to name is in the stack (above the pointer break, we assume no stack mashing since we can not guarantee it)
        void *program_break = ((void *) sbrk(0));
        void *pointer_string = ((void *) (*node)->name);
        int is_below_program_break = pointer_string <= program_break;
        int is_in_heap;
        if (heap_start > program_break) {
            fprintf(stderr, "You tampered with sbrk calls, I can not guarantee the variable is correctly on the heap\n");
        }
        is_in_heap = is_below_program_break && heap_start <= pointer_string;

        if (!is_in_heap && is_heap_var((*node)->name)) {
            // override with a better heuristic, loosy and
            is_in_heap = 1;
        }
        if (is_in_heap) {
            // we need to free the element on the heap
            free((*node)->name);
            (*node)->name = NULL;
        } else {
            // we just remove the stack pointer link
            (*node)->name = NULL;
        }
    }
}

void free_node(Node **node, void *heap_start) {
    free_node_payload(node, heap_start);
    custom_free((void **) node);
}

#pragma clang diagnostic push
#pragma ide diagnostic ignored "misc-no-recursion"

// Helper function: you are allowed to change this to your preferences
void tree_node_delete(Node *node, void *heap_start) {
    if (node != NULL) {
        tree_node_delete(node->left, heap_start);
        tree_node_delete(node->right, heap_start);
        free_node(&node, heap_start);
    }
}

#pragma clang diagnostic pop


// Tree function: you are allowed to change the contents, but not the method signature
void tree_delete(Tree *tree) {
    if (tree == NULL) {
        return;
    }
    tree_node_delete(tree->root, tree->heap_start);
    free(tree);
    // We can't put tree to NULL here, so sad, because no method signature changes allowed
    // Thus we cannot guarantee that the pointer of tree will not be reused for evil...
    // Technically we should change the specification of typedef tree to *tree but meh.
}

#pragma clang diagnostic push
#pragma ide diagnostic ignored "misc-no-recursion"

// Helper function: you are allowed to change this to your preferences
void node_insert_sorted(Node **node, int age, char *name) {
    if (*node == NULL) {
        Node *new = (Node *) malloc(sizeof(Node));
        if (new == NULL) {
            fprintf(stderr, "Error allocating the node");
            return;
        }
        new->age = age;
        char *name_copy = malloc(sizeof(char) * (strlen(name) + 1));
        strcpy(name_copy, name);
        new->name = name_copy;
        new->right = NULL;
        new->left = NULL;
        *node = new;
        return;
    }
    if (age <= (*node)->age) {
        return node_insert_sorted(&((*node)->left), age, name);
    } else {
        return node_insert_sorted(&((*node)->right), age, name);
    }
}

#pragma clang diagnostic pop


void node_insert_balanced(Node **node, int age, char *name) {
    if (*node == NULL) {
        Node *new = (Node *) malloc(sizeof(Node));
        if (new == NULL) {
            fprintf(stderr, "Error allocating the node");
            return;
        }
        new->age = age;
        char *name_copy = malloc(sizeof(char) * (strlen(name) + 1));
        strcpy(name_copy, name);
        new->name = name_copy;
        new->right = NULL;
        new->left = NULL;
        new->height = 1;
        *node = new;
        return;
    }
    if (age <= (*node)->age) {
        node_insert_balanced(&((*node)->left), age, name);
        return rebalance(node);
    } else {
        node_insert_balanced(&((*node)->right), age, name);
        return rebalance(node);
    }
}

// Tree function: you are allowed to change the contents, but not the method signature
void tree_insert(Tree *tree, int age, char *name) {
    if (tree->typeTree == SORTED) {
        node_insert_sorted(&(tree->root), age, name);
    } else {
        node_insert_balanced(&(tree->root), age, name);
    }

}

void tree_delete_sorted(Tree *tree, int age, char *name) {
    Node **node = node_find(&tree->root, age, name); // ok usually I don't like reuse of that kind of function because complexity goes kaboom
    if (node == NULL) {
        return;
    }
    Node *data = *node; // this is just for ease of use, it is not recommended to do so to avoid confusion.
    if (data->right == NULL && data->left == NULL) {
        free_node(node, tree->heap_start);
    } else if (data->left != NULL && data->right == NULL) {
        /**
         *     A
         *    / \
         *   B   D
         *  /
         * C
         *
         * We delete B it becomes
         *
         *     A
         *    / \
         *   C   D
         */
        Node *prev = *node;
        *node = data->left;
        free_node(&prev, tree->heap_start);
    } else if (data->left == NULL && data->right != NULL) {
        /**
         *     A
         *    / \
         *   D   B
         *        \
         *         C
         *
         * We delete B it becomes
         *
         *     A
         *    / \
         *   D   C
         */
        Node *prev = *node;
        *node = data->right;
        free_node(&prev, tree->heap_start);
    } else {
        /**
         *       A
         *      / \
         *     B   C
         *    / \
         *   D  E
         *     /
         *    F
         *   /
         *  G
         *   \
         *    H
         *
         *  We want to delete B here, for that the new tree will be
         *       A
         *      / \
         *     G   C
         *    / \
         *   D  E
         *     /
         *    F
         *   /
         *  H
         *
         *
         *   Remember we have D<=B<=E and B<=G<=F<=E so D<=B<=G<=F<=E
         *   wich without B becomes D<=G<=F<=E
         *   Also we have that F>=G and G<=H and H<=F so G<=H<=F
         *
         *   We don't touch the branch with D here !
         *
         *   We first go on the right branch (so E) then we go all left so F then G
         *   We grab G and we switch it with B, but since G has a right child (remember it's the last left)
         *   then we actually need to put that children as the left of previous last left (so F)
         */
        Node *prev = *node;
        // we know data->right is not NULL
        Node *right_most_left = data->right;
        Node *prev_right_most_left = data->right;
        while (right_most_left->left != NULL) {
            right_most_left = right_most_left->left;
            if (right_most_left->left != NULL) {
                prev_right_most_left = prev_right_most_left->left;
            }
        }
        // we change the node value to the right most left element ( so G above)
        *node = right_most_left;
        // we put H as the children of F
        prev_right_most_left->left = right_most_left->right;
        // we put E as the right child of G (which replaced B and which his H was put to F)
        if ((*node) != prev->right) {
            // we guard the fact that the loop before could be executed zero time so the right would technically be the same...
            (*node)->right = prev->right;
        }
        // we don't forget to restore the left branch (untouched)
        (*node)->left = prev->left;
        free_node(&prev, tree->heap_start);
    }
}

Payload *tree_delete_min(Node **node, void *heap_start) {
    if (node == NULL) {
        fprintf(stderr, "You should not pass a null to the min delete\n");
        return NULL;
    }
    if ((*node)->left == NULL) {
        // we know that the root is the minimum value
        // we save its payload before hand
        Payload *payload = (Payload *) malloc(sizeof(Payload));
        char *name_copy = malloc(sizeof(char) * (strlen((*node)->name) + 1));
        strcpy(name_copy, (*node)->name);
        payload->name = name_copy;
        payload->age = (*node)->age;

        Node *prev = *node;
        *node = (*node)->right;
        free_node(&prev, heap_start);
        rebalance(node);
        return payload;
    }
    Payload *payload = tree_delete_min(&(*node)->left, heap_start);
    rebalance(node);
    return payload;

}

void tree_delete_balanced(Node **node, int age, char *name, void *heap_start) {
    if ((*node) == NULL) {
        return;
    }
    if ((*node)->age == age && (*node)->name != NULL && strcmp((*node)->name, name) == 0) {
        // we choose to only recover the min value and put it back if right exists
        // this allow to cut down the 4 if to only 2, technically this also mean an overhead of 1 function call for half the cases.
        if ((*node)->right != NULL) {
            // we get the data from the min node which was deleted
            Payload *payload = tree_delete_min(&(*node)->right, heap_start);
            if (payload == NULL) {
                return;
            }
            // we free our current node from its payload but do not delete it since we will reuse it
            free_node_payload(node, heap_start);
            // we add our stuff back inside, effectively finishing the virus establishment
            (*node)->age = payload->age;
            (*node)->name = payload->name;
            free(payload);
        } else {
            Node *prev = *node;
            *node = (*node)->left;
            free_node(&prev, heap_start);
        }
    } else {
        if (age <= (*node)->age) {
            tree_delete_balanced(&(*node)->left, age, name, heap_start);
        } else {
            tree_delete_balanced(&(*node)->right, age, name, heap_start);
        }
    }
    rebalance(node);

}

// Tree function: you are allowed to change the contents, but not the method signature
void tree_erase(Tree *tree, int age, char *name) {
    if (tree == NULL) {
        return;
    }
    if (tree->typeTree == SORTED) {
        tree_delete_sorted(tree, age, name);
    } else {
        tree_delete_balanced(&tree->root, age, name, tree->heap_start);
    }
}

// Helper function: you are allowed to change this to your preferences
void tree_dump_node(Node *node, char **acc) {
    if (node == NULL) {
        char *token = "null";
        size_t len_token = strlen(token);
        size_t len_acc;
        if (*acc == NULL) {
            len_acc = 0;
        } else {
            len_acc = strlen(*acc);
        }
        char *new_acc = realloc(*acc, sizeof(char) * (len_acc + len_token + 1));
        if (new_acc == NULL) {
            fprintf(stderr, "Error allocating the string");
            return;
        }
        *acc = new_acc;
        strcpy(*acc + len_acc, token);
        return;
    }
    char token[2 + 12 + 1 + strlen(node->name) + 1 + 2];
    sprintf(token, "[{%d:%s},", node->age, node->name);
    size_t len_token = strlen(token);
    size_t len_acc;
    if (*acc == NULL) {
        len_acc = 0;
    } else {
        len_acc = strlen(*acc);
    }
    char *new_acc = realloc(*acc, sizeof(char) * (len_acc + len_token + 1));
    if (new_acc == NULL) {
        fprintf(stderr, "Error allocating the string");
        return;
    }
    *acc = new_acc;
    strcpy(*acc + len_acc, token);
    tree_dump_node(node->left, acc);
    char *token2 = ",";
    size_t len_token2 = strlen(token2);
    size_t len_acc2;
    if (*acc == NULL) {
        len_acc2 = 0;
    } else {
        len_acc2 = strlen(*acc);
    }
    char *new_acc2 = realloc(*acc, sizeof(char) * (len_acc2 + len_token2 + 1));
    if (new_acc2 == NULL) {
        fprintf(stderr, "Error allocating the string");
        return;
    }
    *acc = new_acc2;
    strcpy(*acc + len_acc2, token2);
    tree_dump_node(node->right, acc);
    char *token3 = "]";
    size_t len_token3 = strlen(token3);
    size_t len_acc3;
    if (*acc == NULL) {
        len_acc3 = 0;
    } else {
        len_acc3 = strlen(*acc);
    }
    char *new_acc3 = realloc(*acc, sizeof(char) * (len_acc3 + len_token3 + 1));
    if (new_acc3 == NULL) {
        fprintf(stderr, "Error allocating the string");
        return;
    }
    *acc = new_acc3;
    strcpy(*acc + len_acc3, token3);
}


char *tree_dump(Tree *tree) {
    char *acc = NULL;
    if (tree == NULL) {

        char *token = "null";
        size_t len_token = strlen(token);
        char *new_acc = realloc(acc, sizeof(char) * (len_token + 1)); //magic of realloc with null behave as malloc
        if (new_acc == NULL) {
            fprintf(stderr, "Error allocating the string");
            return NULL;
        }
        acc = new_acc;
        strcpy(acc, token);
        return acc;
    }

    tree_dump_node(tree->root, &acc);

    return acc;
}

// Helper function: you are allowed to change this to your preferences
void tree_print_node(Node *node) {
    if (node == NULL) {
        printf("null");
        return;
    }

    printf("[");
    printf("{\"%d\":\"%s\"},", node->age, node->name);
    tree_print_node(node->left);
    printf(",");
    tree_print_node(node->right);
    printf("]");
}

// Tree function: you are allowed to change the contents, but not the method signature
void tree_print(Tree *tree, int printNewline) {
    if (tree == NULL) {
        printf("null");
        if (printNewline) {
            printf("\n");
        }
        return;
    }

    tree_print_node(tree->root);

    if (printNewline) {
        printf("\n");
    }
}

// Helper function: you are allowed to change this to your preferences
Node **node_find(Node **node, int age, char *name) {
    if ((*node) == NULL) {
        return NULL;
    }
    if ((*node)->age == age && (*node)->name != NULL && strcmp((*node)->name, name) == 0) {
        return node;
    }

    if (age <= (*node)->age) {
        return node_find(&(*node)->left, age, name);
    } else {
        return node_find(&(*node)->right, age, name);
    }
}

// Tree function: you are allowed to change the contents, but not the method signature
Node *tree_find(Tree *tree, int age, char *name) {
    if (tree == NULL || tree->root == NULL) {
        return NULL;
    }
    Node **pNode = node_find(&(tree->root), age, name);
    if (pNode == NULL) {
        return NULL;
    }
    return (*pNode);
}
