# Huffman Encoding File Compression
Author: Rimsha Rizvi

## Description
This project is a simple implementation of Huffman encoding algorithm for text compression and decompression. The project contains two main files, hashmap.cpp and util.h, which work together to perform the Huffman encoding and decoding processes.

1. hashmap.cpp: This file contains a simple, untemplated implementation of a hashmap used to build the frequency map used in the Huffman encoding algorithm.
2. util.h: This file contains functions to perform Huffman encoding of a given text source file into a compressed destination file.

## How to Use
To use this project, you should:
1. Create a new C++ project in your preferred IDE or compiler.
2. Add hashmap.cpp and util.h to your project.
3. Write a main function to call the appropriate functions from util.h to compress or decompress a file.
4. To compress a file, call the compress function from util.h and pass the name of the file you want to compress as the parameter.
  string compressed = compress("filename.txt");
  This will create a new compressed file with the extension .huf and return a string version of the compressed file's bit pattern.
5. To decompress a file, call the decompress function from util.h and pass the compressed file's name as the parameter.
  string decompressed = decompress("filename.txt.huf");
  This will create a new decompressed file with the _unc extension added before the original file extension, and return a string version of the decompressed file.

## Copyright and Plagiarism Notice
This work is submitted for CS 251 - Data Structures at the University of Illinois at Chicago.
Plagiarism is a serious offense and is strictly prohibited. You may not use, copy, or submit this work as your own. Any plagiarism or unauthorized use will be reported to the relevant academic or professional authorities.
If you would like to use this work for educational or other non-commercial purposes, don't hesitate to get in touch with the author for permission.
