#define SNOW_ENABLED

#include "../inc/tree.h"
#include "../../include/vendors/snow/snow.h"

describe(test_tree_balanced) {
    Tree *tree;
    before_each() {
        tree = tree_create();
        change_tree_type(tree,BALANCED);
    }
    after_each() {
        tree_delete(tree);
    }
    subdesc("Tree initializations (should be run with valgrind)") {
        it("Empty Tree") {
            assert(is_empty(tree) == 1, "Empty tree was not correctly created");
        }

        it("One element insertion in Tree from stack") {
            char *stack_string = "test";
            tree_insert(tree, 12, stack_string);
            asserteq(tree->root->name, "test", "The name was not correctly set");
            assert(tree->root->age == 12, "The name was not correctly set");
            assert(tree->root->left == NULL, "The left tree was not correctly set");
            assert(tree->root->right == NULL, "The right tree was not correctly set");
        }
        it("One element insertion in Tree from heap") {
            char *stack_string = "test";
            char *heap_string = (char *) malloc((strlen(stack_string) + 1) * sizeof(char));
            strcpy(heap_string, stack_string);
            tree_insert(tree, 12, heap_string);
            free(heap_string);
            asserteq(tree->root->name, "test", "The name was not correctly set for root ");
            assert(tree->root->age == 12, "The age was not correctly set for root");
            assert(tree->root->left == NULL, "The left tree was not correctly set");
            assert(tree->root->right == NULL, "The right tree was not correctly set");
        }
        it("Two elements insertion in Tree, one from heap, one from stack") {
            char *stack_string = "test";
            char *heap_string = (char *) malloc((strlen(stack_string) + 1) * sizeof(char));
            strcpy(heap_string, stack_string);
            tree_insert(tree, 12, heap_string);
            tree_insert(tree, 13, stack_string);
            free(heap_string);
            asserteq(tree->root->name, "test", "The name was not correctly set for root ");
            assert(tree->root->age == 12, "The age was not correctly set for root");
            assert(tree->root->left == NULL, "The left tree was not correctly set");
            assert(tree->root->right != NULL, "The right tree was not correctly set");
            asserteq(tree->root->right->name, "test", "The name was not correctly set for root-right");
            assert(tree->root->right->age == 13, "The age was not correctly set for root-right");
        }
        it("Three elements insertion in Tree, checks they are correctly placed") {
            tree_insert(tree, 12, "test1");
            tree_insert(tree, 13, "test2");
            tree_insert(tree, 11, "test3");
            asserteq(tree->root->name, "test1", "The name was not correctly set");
            assert(tree->root->age == 12, "The name was not correctly set");
            assert(tree->root->left != NULL, "The left tree was not correctly set");
            assert(tree->root->right != NULL, "The right tree was not correctly set");
            asserteq(tree->root->right->name, "test2", "The name was not correctly set for root-right");
            assert(tree->root->right->age == 13, "The age was not correctly set for root-right");
            asserteq(tree->root->left->name, "test3", "The name was not correctly set for root-left");
            assert(tree->root->left->age == 11, "The age was not correctly set for root-left");
        }
    }
    subdesc("Tree node deletion") {
        it("Zero element deletion from Tree") {
            assert(tree->root == NULL, "The tree root was not correctly set");
            tree_erase(tree, 12, "test");
            assert(tree->root == NULL, "The tree root was not correctly deleted");
        }
        it("One element deletion from Tree") {
            tree_insert(tree, 12, "");
            assert(tree->root != NULL, "The tree root was not correctly set");
            assert(tree->root->age == 12, "The root was not correctly set");
            assert(tree->root->left == NULL, "The left tree was not correctly set");
            assert(tree->root->right == NULL, "The right tree was not correctly set");
            tree_erase(tree, 12, "");
            assert(tree->root == NULL, "The tree root was not correctly deleted");
        }
        it("Two elements deletion from left branch tree") {
            /**
             *          12
             *         /
             *       11
             */
            tree_insert(tree, 12, "");
            tree_insert(tree, 11, "");
            assert(tree->root != NULL, "The tree root was not correctly set");
            assert(tree->root->age == 12, "The tree root was not correctly set");
            assert(tree->root->left != NULL, "The left tree was not correctly set");
            assert(tree->root->left->age == 11, "The left tree was not correctly set");
            assert(tree->root->right == NULL, "The right tree was not correctly set");
            tree_erase(tree, 11, "");
            assert(tree->root != NULL, "The tree root was wrongly deleted");
            assert(tree->root->left == NULL, "The left tree was not correctly deleted");
            assert(tree->root->right == NULL, "The right tree was wrongly set");
            tree_erase(tree, 12, "");
            assert(tree->root == NULL, "The tree root was not correctly deleted");
        }

        it("Two elements deletion from left branch tree") {
            /**
             *          12
             *            \
             *            13
             */
            tree_insert(tree, 12, "");
            tree_insert(tree, 13, "");
            assert(tree->root != NULL, "The tree root was not correctly set");
            assert(tree->root->age == 12, "The tree root was not correctly set");
            assert(tree->root->right != NULL, "The right tree was not correctly set");
            assert(tree->root->right->age == 13, "The right tree was not correctly set");
            assert(tree->root->left == NULL, "The left tree was not correctly set");
            tree_erase(tree, 13, "");
            assert(tree->root != NULL, "The tree root was wrongly deleted");
            assert(tree->root->right == NULL, "The right tree was not correctly deleted");
            assert(tree->root->left == NULL, "The left tree was wrongly set");
            tree_erase(tree, 12, "");
            assert(tree->root == NULL, "The tree root was not correctly deleted");
        }
        it("Three elements deletion from right branch tree") {
            /**
             *          12
             *         / \
             *       11   13
             *              \
             *              14
             */
            tree_insert(tree, 12, "");
            tree_insert(tree, 13, "");
            tree_insert(tree, 11, "");
            tree_insert(tree, 14, "");
            assert(tree->root != NULL, "The tree root was not correctly set");
            assert(tree->root->age == 12, "The tree root was not correctly set");
            assert(tree->root->right != NULL, "The right tree was not correctly set");
            assert(tree->root->right->age == 13, "The right tree was not correctly set");
            assert(tree->root->right->right != NULL, "The right right tree was not correctly set");
            assert(tree->root->right->right->age == 14, "The right right tree was not correctly set");
            assert(tree->root->left != NULL, "The left tree was not correctly set");
            assert(tree->root->left->age == 11, "The left tree was not correctly set");
            tree_erase(tree, 13, "");
            assert(tree->root != NULL, "The tree root was wrongly deleted");
            assert(tree->root->right != NULL, "The right tree was wrongly deleted");
            assert(tree->root->right->right == NULL, "The right right tree was not correctly deleted");
            assert(tree->root->left != NULL, "The left tree was wrongly deleted");
            assert(tree->root->age == 12, "The tree root was wrongly changed");
            assert(tree->root->right->age == 14, "The right tree was not correctly changed");
            assert(tree->root->left->age == 11, "The left tree was wrongly changed");
            tree_erase(tree, 14, "");
            assert(tree->root != NULL, "The tree root was wrongly deleted");
            assert(tree->root->right == NULL, "The right tree was not correctly deleted");
            assert(tree->root->left != NULL, "The left tree was wrongly deleted");
            assert(tree->root->age == 12, "The tree root was wrongly changed");
            assert(tree->root->left->age == 11, "The left tree was wrongly changed");
            tree_erase(tree, 12, "");
            assert(tree->root != NULL, "The tree root was wrongly deleted");
            assert(tree->root->left == NULL, "The left tree was not correctly deleted");
            assert(tree->root->age == 11, "The tree root was not correctly changed");
        }
        it("Simple root deletion with full tree") {
            /**
             * This one is a bit tricky but if you recall the both right and left then it become easy
             *          12
             *         / \
             *       11   13
             *
             * since we fetch first right then it becomes
             *      13
             *     /
             *   11
             */
            tree_insert(tree, 12, "");
            tree_insert(tree, 13, "");
            tree_insert(tree, 11, "");
            assert(tree->root != NULL, "The tree root was not correctly set");
            assert(tree->root->age == 12, "The tree root was not correctly set");
            assert(tree->root->right != NULL, "The right tree was not correctly set");
            assert(tree->root->right->age == 13, "The right tree was not correctly set");
            assert(tree->root->left != NULL, "The left tree was not correctly set");
            assert(tree->root->left->age == 11, "The left tree was not correctly set");
            tree_erase(tree, 12, "");
            assert(tree->root->age == 13, "The tree root was not correctly changed");
            assert(tree->root->right == NULL, "The right tree was not deleted");
            assert(tree->root->left != NULL, "The left tree was wrongfully deleted");
            assert(tree->root->left->age == 11, "The right tree was not correctly changed");
        }
        it("Complex deletion with partial tree") {
            /**
             *       10
             *      / \
             *     3   12
             *    / \   \
             *   2  7   13
             *     / \
             *    6   8
             *
             *  We want to delete 3 here, for that the new tree will be
             *       10
             *      / \
             *     6   12
             *    / \   \
             *   2   7   13
             *        \
             *         8
             *
             *  We want to delete 12 here, for that the new tree will be
             *       7
             *      / \
             *     6  10
             *    /   / \
             *   2   8  13
             *
             *  We want to delete 10 here, for that the new tree will be
             *       7
             *      / \
             *     6  13
             *    /   /
             *   2   8
             *
             *  We want to delete 7 here, for that the new tree will be
             *       8
             *      / \
             *     6  13
             *    /
             *   2
             */
            tree_insert(tree, 12, "");
            tree_insert(tree, 13, "");
            tree_insert(tree, 3, "");
            tree_insert(tree, 2, "");
            tree_insert(tree, 10, "");
            tree_insert(tree, 8, "");
            tree_insert(tree, 6, "");
            tree_insert(tree, 7, "");
            char *dump = tree_dump(tree);
            asserteq_str("[{10:},[{3:},[{2:},null,null],[{7:},[{6:},null,null],[{8:},null,null]]],[{12:},null,[{13:},null,null]]]", dump, "wrong tree");
            free(dump);

            tree_erase(tree, 3, "");
            dump = tree_dump(tree);
            asserteq_str("[{10:},[{6:},[{2:},null,null],[{7:},null,[{8:},null,null]]],[{12:},null,[{13:},null,null]]]", dump, "wrong tree");
            free(dump);

            tree_erase(tree, 12, "");
            dump = tree_dump(tree);
            asserteq_str("[{7:},[{6:},[{2:},null,null],null],[{10:},[{8:},null,null],[{13:},null,null]]]", dump, "wrong tree");
            free(dump);

            tree_erase(tree, 10, "");
            dump = tree_dump(tree);
            asserteq_str("[{7:},[{6:},[{2:},null,null],null],[{13:},[{8:},null,null],null]]", dump, "wrong tree");
            free(dump);

            tree_erase(tree, 7, "");
            dump = tree_dump(tree);
            asserteq_str("[{8:},[{6:},[{2:},null,null],null],[{13:},null,null]]", dump, "wrong tree");
            free(dump);
        }
        it("More complex tree insertion") {
            /**
             *       10
             *      / \
             *     3   12
             *    / \   \
             *   2  7   13
             *     / \
             *    5   8
             *
             * We insert 6
             *
             *       10
             *      / \
             *     5   12
             *    / \   \
             *   3  7   13
             *  /  / \
             * 2  6   8
             *
             * We insert 4
             *
             *       10
             *      / \
             *     5   12
             *    / \    \
             *   3   7   13
             *  / \  /\
             * 2  4 6  8
             *
             * We insert 1
             *
             *         5
             *      /     \
             *     5      10
             *    / \     / \
             *   2   4   7  12
             *  /       /\  /
             * 1       6 8 13
             *
             *
             */
            tree_insert(tree, 12, "");
            tree_insert(tree, 13, "");
            tree_insert(tree, 3, "");
            tree_insert(tree, 2, "");
            tree_insert(tree, 10, "");
            tree_insert(tree, 8, "");
            tree_insert(tree, 5, "");
            tree_insert(tree, 7, "");
            char *dump = tree_dump(tree);
            asserteq_str("[{10:},[{3:},[{2:},null,null],[{7:},[{5:},null,null],[{8:},null,null]]],[{12:},null,[{13:},null,null]]]", dump, "wrong tree");
            free(dump);

            tree_insert(tree, 6, "");
            dump = tree_dump(tree);
            asserteq_str("[{10:},[{5:},[{3:},[{2:},null,null],null],[{7:},[{6:},null,null],[{8:},null,null]]],[{12:},null,[{13:},null,null]]]", dump, "wrong tree");
            free(dump);

            tree_insert(tree, 4, "");
            dump = tree_dump(tree);
            asserteq_str("[{10:},[{5:},[{3:},[{2:},null,null],[{4:},null,null]],[{7:},[{6:},null,null],[{8:},null,null]]],[{12:},null,[{13:},null,null]]]", dump, "wrong tree");
            free(dump);
            tree_insert(tree, 1, "");
            dump = tree_dump(tree);
            asserteq_str("[{5:},[{3:},[{2:},[{1:},null,null],null],[{4:},null,null]],[{10:},[{7:},[{6:},null,null],[{8:},null,null]],[{12:},null,[{13:},null,null]]]]", dump, "wrong tree");
            free(dump);
        }


  }
  subdesc("Test free") {
      it("Does it even work?") {
          int *t = calloc(12, sizeof(int));
          assert(t != NULL, "Malloc failed");
          custom_free((void **) &t);
          assert(t == NULL, "Free failed");
      }
  }
  subdesc("Test heap") {
      it("This will fail with valgrind, that's the whole point ;)") {
          char *heapvar = (void *) malloc(10);
          int i = 0;
          void *stackvar = (void *) &i;
          void *program_break = ((void *) sbrk(0));
          if (RUNNING_ON_VALGRIND) {
              assert((void*)heapvar > program_break, "heap end is misplaced");
              assert(stackvar > program_break, "stack is misplaced");
              free(heapvar);
              for (int j = 0; j < 100; ++j) {
                  heapvar = (char *) malloc(10);
                  *heapvar='\0';
                  tree_insert(tree, 1, heapvar);
                  program_break = ((void *) sbrk(0));
                  assert((void*)heapvar > program_break, "heap is misplaced");
                  free(heapvar);
              }

          } else {
              assert((void*)heapvar <= program_break, "heap end is misplaced");
              // this line can not work in this type of testing sadly
              //assert(tree->heap_start <= heapvar, "heap start is misplaced");
              assert(stackvar > program_break, "stack is misplaced");
              free(heapvar);
              for (int j = 0; j < 100; ++j) {
                  heapvar = (void *) malloc(10);
                  tree_insert(tree, 1, heapvar);
                  program_break = ((void *) sbrk(0));
                  assert((void*)heapvar <= program_break, "heap is misplaced");
                  free(heapvar);
              }
          }

      }
  }

}

snow_main();
