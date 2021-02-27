#!/usr/bin/env python3

import json
import sys
from json import JSONDecodeError
import argparse
import sys


def main(arg):
    parser = argparse.ArgumentParser(description='Check the SortedTree output')
    parser.add_argument('-b', action='store_true', help="Check for balaced")
    parser.add_argument('-s', action='store_true', help="Check for sorted")
    parser.add_argument('infile', nargs='?', type=argparse.FileType('r'), default=sys.stdin)
    args = parser.parse_args()

    try:
        root = read_json(args.infile)
    except JSONDecodeError as e:
        print("Failed to parse JSON: " + str(e))
        return

    if not is_valid_tree(root):
        print("The JSON string does not contain a valid tree")
        return

    check_balanced = True
    check_sorted = True

    if args.b:
        check_balanced = is_balanced(root)
        print("Balanced: " + str(check_balanced))

    if args.s:
        check_sorted = is_sorted(root)
        print("Sorted: " + str(check_sorted))

    if not (check_balanced and check_sorted):
        sys.exit(1)
        

def read_json(input_string):
    """
    Read and parse the JSON input.
    JSON Null -> Python None
    """
    return json.load(input_string)


def is_valid_tree(root):
    """Check whether the loaded json is a valid tree."""

    # JSON null -> python None, return true as a leaf is a valid tree.
    if root is None:
        return True
    # Check the type and size of the node, check the type and size of the data element.
    # Recursively check the subtrees.
    elif type(root) == list and len(root) == 3 and is_valid_data_node(root[0]):
        return is_valid_tree(root[1]) and is_valid_tree(root[2])
    # Invalid structure in tree, return False
    else:
        return False


def is_valid_data_node(data_dict):
    """Checks whether the json data node is valid."""

    # Check if json element is a dict
    if type(data_dict) != dict:
        return False

    # Check if only data element is present in the dict
    if len(data_dict) != 1:
        return False

    # Try to parse the string key to int
    try:
        int(list(data_dict)[0])
        return True
    except ValueError:
        return False


def is_balanced(root):
    """Returns true when the tree is balanced."""
    return _is_balanced(root) > -1


def _is_balanced(root):
    """Calculates the height of the tree, or returns -1 when the tree is not balanced."""
    # Return 0 for leafs
    if root is None:
        return 0

    # Get height or unbalanced (-1) for left subtree
    left_height = _is_balanced(root[1])
    if left_height == -1:
        return -1

    # Get height or unbalanced (-1) for right subtree
    right_height = _is_balanced(root[2])
    if right_height == -1:
        return -1

    # The difference between subtree heights should not exceed 1:
    if abs(left_height - right_height) > 1:
        return -1

    # This (sub)tree is balanced so we return the height
    return max(left_height, right_height) + 1


def is_sorted(root):
    """
    Checks whether the tree is sorted
    Performs an inorder walk
    """
    # Define nonlocal var for function closure
    prev = None

    def _is_sorted(curr):
        nonlocal prev

        # Tree is empty return true
        if curr is None:
            return True

        # Inorder walk of tree
        if _is_sorted(curr[1]) is False:
            return False

        # Found the next inorder node, compare.
        if prev is not None and data_tuple(prev[0]) > data_tuple(curr[0]):
            return False

        # Store the current node, and walk
        prev = curr
        return _is_sorted(curr[2])

    return _is_sorted(root)


def data_tuple(json_dict):
    """
    Parses a json data dict to a tuple
    Assumes the dict is already in valid tree format.
    """
    key = list(json_dict)[0]
    return int(key), json_dict[key]


if __name__ == "__main__":
    main(sys.argv)
