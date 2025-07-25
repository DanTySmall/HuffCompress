#ifndef HDECOM_H
#define HDECOM_H

#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <cstdio>

class Node {
public:
    char glyph;
    Node* right;
    Node* left;

    // Constructor
    Node(char glyph);

    // Destructor (recommended to prevent memory leaks)
    ~Node();
};

namespace Decompress{
  class HuffmanTree {
  public:
    Node* tree;

    // Constructor
    HuffmanTree();

    // Destructor (recommended to prevent memory leaks)
    ~HuffmanTree();

    // Methods
    FILE* createTree(FILE* fp);
    void decompressText(FILE* input, FILE* output);

  private:
    // Helper method to clean up tree memory
    void cleanupTree(Node* node);
  };
}

#endif // HDECOM_H
