# Breadth-First Search and Depth-First Search
Breadth-first search and depth-first search techniques implementation in C++

This program takes an input txt file which has three words separated by space as its name. Inside this file, a matrix layout exists where each row represents a unique letter from the file name, and each column represents numbers from 0 to 9. Each node of this matrix has "." at the moment. Each unique letter will have a unique number, which will be represented with a 1 in the matrix (if 1 is seen, row is the letter and column is the number of that letter). The goal of this program is to solve a cryptarithmetic puzzle by creating all possible numberings of letters, placing them in a tree and finding a possible solution inside the tree using BFS or DFS. Tree structure is like this: Root node doesn't have any numbers assigned to any letters. It's children will have their first letter assigned to a number (each children will have different numberings). Their children will have two of their letters assigned to numbers. Like this, each new node will copy it's parent and do one more numbering of a letter. If a node has all letters numbered, it won't create new children.

The program fills the output file with a solution. Also, number of nodes created, number of nodes visited, running time and solution is printed to the screen.

The program should take 5 command arguments, first one should be either "BFS" or "DFS", selecting the search technique to be used. Second, third and fourth arguments are the words of the input files name. Fifth argument is the name of the output file. Ex: "BFS TWO TWO FOUR output.txt"

Example is taken from ITU BLG336E Course, Spring 2021 Homeworks
