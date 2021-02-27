#!/bin/bash

# Feel free to add more tests. Write your inputs to # inputX.txt
# and the expected output to expected_output_for_inputX.txt

set -e

for i in input*.txt; do
    $1 < $i | diff -q expected_output_for_$i -
done