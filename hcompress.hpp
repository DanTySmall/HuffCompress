#ifndef HCOMPRESS_H
#define HCOMPRESS_H

#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <cstdio>

class Character {
public:
    char glyph;
    int freq;
    Character* left;
    Character* right;

    // Constructor
    Character(int freq, char glyph);
};

// Comparator for Characters
struct compareChar {
    bool operator()(Character a, Character b);
};

class Heap {
public:
    std::priority_queue<Character, std::vector<Character>, compareChar> heap;

    // Constructor
    Heap(int* freqs, int length);

    // Methods
    std::priority_queue<Character, std::vector<Character>, compareChar> createPQ(int* freqs, int length);
    std::priority_queue<Character, std::vector<Character>, compareChar> getHeap();
};

namespace Compress{
class HuffmanTree {
public:
    int aend;
    int count;
    Character* tree;
    char** codes;
    std::vector<char> path;

    // Constructor
    HuffmanTree(std::priority_queue<Character, std::vector<Character>, compareChar> heap);

    // Destructor (recommended to prevent memory leaks)
    ~HuffmanTree();

    // Methods
    Character* createHuffTree(std::priority_queue<Character, std::vector<Character>, compareChar> heap);
    void getHuffcodes(Character* tree, char** codes, std::vector<char> path, int position);
    void printHuffcodes();
    void compressText(FILE* input, FILE* output);

private:
    // Helper method to clean up memory
    void cleanupCodes();
    void cleanupTree(Character* node);
};
}

#endif // HCOMPRESS_H
