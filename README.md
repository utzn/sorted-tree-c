# Assignment - Implement a binary tree in C

## Introduction
Your assignment is to implement a binary tree in C. To get you started we are supplying you with some boilerplate code. This source code specifies the general design of the program, ie. the input-loop and the general tree structure.

Lookout though, just because we supplied you some boilerplate code, that doesn't mean it is bug-free!

Within the source code you will find comments like these:
```
// DO NOT MODIFY
```
The reason for this is that all of you work with the same interface and the possible solutions don't stray off too far from each other.

In general, you are allowed to:
* Change the structures
* Change the implementations
* Add more tests (you will find comments in the "tests" directories how you can do that)

But you are not allowed to:
* Change the method signatures of the "tree" library (but you are allowed to change the structures that are passed to this library since they are just modified through the "wrapper" functions of the "tree" library)
* Change the build system
* Rename the files
* Change the overall project layout

## Implementation details

The tree should hold the following attributes:

* Age
* Name

As can be seen in the boilerplate code:

```
typedef struct Node {
    /**
     * Left child of this node
     */
    struct Node* left;
    /**
     * Right child of this node
     */
    struct Node* right;
    /**
     * The age of the data in this node
     */
    int age;
    /**
     * The name of the data in this node
     */
    char* name;
} Node;
```

**You may change the `struct Node` or `struct Tree` if you wish to do so**

## Input and output format

The program consists of two main parts. The "tree" folder contains the implementation of the tree and the "main" folder contains the implementation of a program that reads instructions from stdin and then updates or dumps the tree. In detail, the main program is supposed to process the following commands:

### Insert

```
i <age:int> <name:string>
    For valid data, inserts an element into the tree with the specified data.
```

### Erase

```
e <age:int> <name:string>
    For valid data, erases the element from the tree that has data equal to the specified data.
```

### Check for the existence

```
c <age:int> <name:string>
    For valid data, prints "y\n" iff an equal element to the specified data is in the container already, "n\n" otherwise.
```

### Print

```
p
    Prints the elements in the tree. An element should be printed as: [<data>,<left>,<right>].
    After the entire dump, a newline character should be printed.

    <data> should be printed as: {"<age:int>":"<name:string>"}
           for example: {"5":"foo"}

    When left or right are null pointers, just "null" should be printed instead without the quotes.
        example: [{"5":"foo"},[{"3":"bar"},null,null],null]
        
    When the current tree is empty, just "null" (without the quotes) should be printed.
        example: null

```

### Reset

```
x
    Reset the internal state and start with a fresh and empty tree.
```


### Exit

```
q
    Exits the program
```

## Test

In the `tree` folder, you find a tests directory that is used to perform some kind of unit-testing with the test library. You may extend the test program here.

In the `main` folder, you find a tests directory that is used to perform some kind of implementation testing with the `main` program. You may add more tests here.

There is a Python tool in `tool.py` that check the dump of the tree whether it is sorted ("-s") or balanced ("-b").

Those tests designed to guide you to writing correct code but passing the test does **not** mean you have finished the assignment. We really recommend that you perform additional testing.

## Additional implementation notes

When you encounter the *q* command or the end of the input, please free all the memory you allocated using `malloc` or similar functions first and then exit the program.

Your program may also encounter input that doesn't match the description of the input format. Your program may then exit gracefully, but under no circumstances crash. Again, free all the memory first before you terminate the program.

## Running the code
If you use CLion, you can open the project folder and you should be good to go. Use the target selection in the top right to run main or the tree test.

If you use Visual Studio Code, you can use the [C/C++](https://marketplace.visualstudio.com/items?itemName=ms-vscode.cpptools) and GDB extensions to enable debugging. You can use the [CMake extension](https://vector-of-bool.github.io/docs/vscode-cmake-tools/index.html) to build and run the code.

The `main` and `test_tree` targets should appear in the bottom toolbar if the extension is installed correctly. Use the `Debug` button in the bottom row to run or debug the selected target.

When you would like to build the project on the command line, we recommend to create a new directory (for example *build*) and then run the build there: `mkdir build && cd build && cmake -S .. && make `. When you would like to build your project with a different compiler, you can use a second build directory for that: `mkdir build-clang && cd build-clang && CC=clang cmake -S .. && make`

To run the tests, you may execute the `make test` command in the build directory.

Also, we will test your code on the server when you commit and push the code to the server. You can see the test results in the Gitlab web view. Again, those are just the tests that are present in your project, when those tests succeed, it doesn't mean that your code is correct.

## Windows

We recommend running the code on a Linux (virtual) machine. MacOS should also work fine if you have installed the right packages. If you use Windows and do not want to use a virtual machine, take a look at using [the Windows Subsystem for Linux](https://code.visualstudio.com/docs/remote/wsl). This is fine for programming and running the code itself, but memory leak detection and other such tools might not work correctly. 

## To Do

1. Find and fix all existing bugs in the source code
2. Implement a sorted binary tree by making use of the supplied skeleton
3. Make that tree a balanced search tree (for example an AVL tree)
4. Verify that your implementation is correct not only logic-wise but also memory-safe, as well as memory-leak free.