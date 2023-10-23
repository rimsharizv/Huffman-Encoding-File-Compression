//
// STARTER CODE: util.h
//
// Rimsha Rizvi
//
// This file calls functions that will  perform a Huffman encoding
// of a given text source file into a destination compressed file
//

#pragma once
#include <iostream>
#include <fstream>
#include <map>
#include <unordered_map>
#include <queue>          // std::priority_queue
#include <vector>         // std::vector
#include <functional>     // std::greater
#include <string>
#include "bitstream.h"
#include "util.h"
#include "hashmap.h"
#include "mymap.h"

struct HuffmanNode {
    int character;
    int count;
    HuffmanNode* zero;
    HuffmanNode* one;
};

class prioritize {  // you could also use a struct
  public:
  bool operator()(const HuffmanNode* p1, HuffmanNode* p2) {
    return p1->count > p2->count;  // HuffmanNode* will be different
  }
};

//
// *This method frees the memory allocated for the Huffman tree.
//
void freeTree(HuffmanNode* node) {
  // TO DO:  Write this function here.
  if (node == nullptr) {
    return;
  }
  freeTree(node->zero);
  freeTree(node->one);
  delete node;
}

//
// *This function build the frequency map.  If isFile is true, then it reads
// from filename.  If isFile is false, then it reads from a string filename.
//
void buildFrequencyMap(string filename, bool isFile, hashmap &map) {
  // TO DO:  Write this function here.
  ifstream inFile(filename);
  char character;
  string line;
  int val = 0;
  // If isFile is true, filename is an actual file name. You open the file
  // & read each char from the input file stream (left case)
  if (isFile == true) {
    while (getline(inFile, line, '\0')) {
      for (int i = 0; i < line.length(); i++) {
        character = line[i];
        int key = (int)character;
        if (map.containsKey(key) == true) {
          val = map.get(key) + 1;
          // Create hashmap that maps char to its freq (number of times it appears on the file)
          map.put(key, val);
        } else {
          map.put(key, 1);
        }
      }
      map.put(PSEUDO_EOF, 1);
      return;
    }
  } else {  // If it’s false, treat filename as a string variable (right case)
    for (int i = 0; i < filename.length(); i++) {
      val = 0;
      character = filename[i];
      int key = (int)character;
      if (map.containsKey(key) == true) {
        val = map.get(key) + 1;
        map.put(key, val);
      } else {
        map.put(key, 1);
      }
    }
    map.put(PSEUDO_EOF, 1);
  }
}

//
// *This function builds an encoding tree from the frequency map.
//
HuffmanNode* buildEncodingTree(hashmap &map) {
  // TO DO:  Write this function here.
  if (map.size() == 0) {
    return nullptr;
  }
  // prioritized based on the Huffman Node’s count
  priority_queue< HuffmanNode*, vector <HuffmanNode*>, prioritize > myPQ;
  vector<int> keys = map.keys();

  for (int i = 0; i < keys.size(); i++) {
    int character = keys[i];
    int val = map.get(character);
    // for every k-v pair in frequency map, a new node is created
    // then it is inserted to the priority queue
    HuffmanNode *node = new HuffmanNode();
    node -> character = character;
    node -> count = map.get(character);
    node -> zero = nullptr;
    node -> one = nullptr;
    myPQ.push(node);
  }

  while (myPQ.size() > 1) {
    HuffmanNode *val1 = myPQ.top();
    myPQ.pop();
    HuffmanNode *val2 = myPQ.top();
    myPQ.pop();

    HuffmanNode * newNode = new HuffmanNode();
    newNode -> character = 257;
    newNode -> count = (val1 -> count) + (val2 -> count);
    newNode -> zero = val1;
    newNode -> one = val2;
    myPQ.push(newNode);
  }   
  return myPQ.top();  // TO DO: update this return
}

//
// *Helper function for buildEncodingMap
// This function is a recurrsive function
//
void _buildEncodingMap(HuffmanNode* node, string str,
                      mymap <int, string> & encodingMap) {
  if (node == nullptr) {
    return;
  } else if ((node->zero == nullptr) && (node->one == nullptr)) {
    encodingMap.put(node->character, str);
  }
  _buildEncodingMap(node->zero, str + "0", encodingMap);
  _buildEncodingMap(node->one, str + "1", encodingMap);
}


//
// *This function builds the encoding map from an encoding tree.
//
mymap <int, string> buildEncodingMap(HuffmanNode* tree) {
  mymap <int, string> encodingMap;
  // TO DO:  Write this function here.
  string str = "";
  _buildEncodingMap(tree, str, encodingMap);
  return encodingMap;  // TO DO: update this return
}

//
// *This function encodes the data in the input stream into the output stream
// using the encodingMap.  This function calculates the number of bits
// written to the output stream and sets result to the size parameter, which is
// passed by reference.  This function also returns a string representation of
// the output file, which is particularly useful for testing.
//
string encode(ifstream& input, mymap <int, string> &encodingMap,
              ofbitstream& output, int &size, bool makeFile) {
  // TO DO:  Write this function here.
  string str = "";
  char c;
  size = 0;
  input.get(c);

  while (!input.eof()) {
    string encode_str = encodingMap.get(c);
    if (makeFile == true) {
      for (char bit :encode_str) {
        if (bit == '0') {
          output.writeBit(0);
        } else {
          output.writeBit(1);
        }
      }
    }
    size += encode_str.size();
    str += encode_str;
    input.get(c);
  }
  string encode_str = encodingMap.get(PSEUDO_EOF);
  size += encode_str.size();
  str += encode_str;

  if (makeFile == true) {
    for (char bit :encode_str) {
      if (bit == '0') {
        output.writeBit(0);
      } else {
        output.writeBit(1);
      }
    }
  }
  return str;  // TO DO: update this return
}

//
// *Helper function for decode
// This function is a recurrsive function
//

int _decode(ifbitstream &input, HuffmanNode* node) {
  if (node == nullptr) {
    return -1;
  } else if ((node->zero == nullptr) && (node->one == nullptr)) {
    return node->character;
  } else {
    int flag = 0;
    flag = input.readBit();
    if (flag == 0) {
      return _decode(input, node->zero);
    } else if (flag == 1) {
      return _decode(input, node->one);
    }
  }
}
//
// *This function decodes the input stream and writes the result to the output
// stream using the encodingTree.  This function also returns a string
// representation of the output file, which is particularly useful for testing.
//
string decode(ifbitstream &input, HuffmanNode* encodingTree, ofstream &output) {
  // TO DO:  Write this function here.
  int c;
  string decoded = "";
  // for each bit in file, traverse the tree
  c = _decode(input, encodingTree);
  while (c != PSEUDO_EOF) {
    output.put(char(c));
    decoded += char(c);
    c = _decode(input, encodingTree);
  }
    return decoded;  // TO DO: update this return
}

//
// *This function completes the entire compression process.  Given a file,
// filename, this function (1) builds a frequency map; (2) builds an encoding
// tree; (3) builds an encoding map; (4) encodes the file (don't forget to
// include the frequency map in the header of the output file).  This function
// should create a compressed file named (filename + ".huf") and should also
// return a string version of the bit pattern.
//
string compress(string filename) {
  // TO DO:  Write this function here.
  hashmap frequencyMap;
  int size;
  buildFrequencyMap(filename, true, frequencyMap);
  HuffmanNode* encodingTree = buildEncodingTree(frequencyMap);

  mymap <int, string> encodingMap = buildEncodingMap(encodingTree);

  ifstream inFile(filename);
  // prepare an output bit stream to a new .huf file that'll be created
  ofbitstream output (filename + ".huf");
  output << frequencyMap;
  string str = encode(inFile, encodingMap, output, size, true);
  inFile.close();
  freeTree(encodingTree);
  return str;  // TO DO: update this return
}

//
// *This function completes the entire decompression process.  Given the file,
// filename (which should end with ".huf"), (1) extract the header and build
// the frequency map; (2) build an encoding tree from the frequency map; (3)
// using the encoding tree to decode the file.  This function should create a
// compressed file using the following convention.
// If filename = "example.txt.huf", then the uncompressed file should be named
// "example_unc.txt".  The function should return a string version of the
// uncompressed file.  Note: this function should reverse what the compress
// function did.
//
string decompress(string filename) {
  // TO DO:  Write this function here.
  string text = "";
  hashmap frequencyMap;
  HuffmanNode* encodingTree;
  int index = filename.find(".huf");

  if (index >= 0) {  // finding ".huf" to remove the extension
    filename = filename.substr(0, index);
  }

  index = filename.find(".");
  string ext = filename.substr(index, filename.length() - index);
  filename = filename.substr(0, index);

  ifbitstream input(filename + ext + ".huf");
  ofstream output(filename + "_unc" + ext);  // "_unc" extension added

  input >> frequencyMap;
  encodingTree = buildEncodingTree(frequencyMap);
  text = decode(input, encodingTree, output);
  freeTree(encodingTree);
  input.close();
  output.close();
  return text;  // TO DO: update this return
}
