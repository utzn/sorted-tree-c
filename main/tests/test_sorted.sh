#!/bin/bash

set -e

# Feel free to add more tests. Add files named dumpX.txt that do something
# and then finally dump the content of the tree.

for i in dump*.txt; do
    $1 < $i | ./tool.py -s
done
