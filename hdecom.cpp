#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
using namespace std;

class Node {
public:
  char glyph;
  Node* right;
  Node* left;

  Node(char glyph){
    this->glyph = glyph;
    this->right = NULL;
    this->left = NULL;
  }
};

namespace Decompress{
  class HuffmanTree {
  public:
    Node* tree = new Node('\0');

    FILE* createTree(FILE* fp){
      if (!fp) return NULL;

      char c = fgetc(fp);
      if (c == EOF) return NULL;

      // Read character codes until we hit "XX"
      while(!feof(fp)){
        // Check for "XX" separator
        if (c == 'X'){
          char next = fgetc(fp);
          if(next == 'X') {
            // Found "XX", skip the newline and return
            char newline = fgetc(fp);
            return fp;
          } else {
            // Not "XX", put the character back and continue
            ungetc(next, fp);
            if (c == 'X') {
              // This 'X' is actually a character to be encoded
              // Continue processing it normally
            }
          }
        }

        char glyph = c;

        // Expect colon
        c = fgetc(fp);
        if (c != ':'){
          cout << "Standard Broken / File Corrupted (missing colon)" << endl;
          exit(1);
        }

        // Read the code
        c = fgetc(fp);
        if (c != '0' && c != '1'){
          cout << "Standard Broken / File Corrupted (invalid code start)" << endl;
          exit(1);
        }

        // Parse the binary code and build tree
        Node* current = tree;
        while(c == '0' || c == '1'){
          if(c == '0'){ // Going Left
            if (current->left == NULL){
              current->left = new Node('\0');
            }
            current = current->left;
          } else { // Going Right (1)
            if (current->right == NULL){
              current->right = new Node('\0');
            }
            current = current->right;
          }
          c = fgetc(fp);
        }

        // The current node represents this character
        current->glyph = glyph;

        // Expect delimiter (ASCII 127)
        if (c != (char)127) {
          cout << "Standard Broken / File Corrupted (missing delimiter)" << endl;
          cout << "Expected ASCII 127, got: " << (int)c << endl;
          exit(1);
        }

        c = fgetc(fp);
      }

      cout << "Error Constructing Huffman Tree - unexpected end of file" << endl;
      exit(1);
    }

    void decompressText(FILE* input, FILE* output){
      char c = fgetc(input);

      // Skip any whitespace after "XX\n"
      while(c == ' ' || c == '\n' || c == '\r') {
        c = fgetc(input);
      }

      Node* current = tree;

      while(c != EOF){
        // Check if it is binary data
        if(c != '0' && c != '1') {
          cout << "The Program is Trying to parse non-binary Data: " << c << " (ASCII " << (int)c << ")" << endl;
          break;
        }

        // Traverse tree based on bit
        if (c == '0'){
          current = current->left;
        } else {
          current = current->right;
        }

        // Check for null pointer (corrupted data)
        if (current == NULL) {
          cout << "Invalid code sequence - null pointer reached" << endl;
          break;
        }

        // If we reached a leaf node (character), output it
        if(current->glyph != '\0'){
          fputc(current->glyph, output);
          cout << current->glyph;
          current = tree; // Reset to root
        }

        c = fgetc(input);
      }

      if (current != tree){
        cout << "Warning: The Program did not end on a complete character" << endl;
      }

      fclose(input);
      fclose(output);
    }
  };
}

  
int main(int argc, char** argv) {
  FILE* fp;

  if(argc < 2){
    fp = fopen("compressed.txt", "r");
  } else {
    fp = fopen(argv[1], "r");  // Fixed: was argv[0]
  }

  // Check if File Exists
  if (!fp){
    cout << "File not found :(" << endl;
    return 1;
  }

  // Create Huffman Tree
  Decompress::HuffmanTree tree = Decompress::HuffmanTree();
  fp = tree.createTree(fp);

  if (!fp) {
    cout << "Failed to create tree" << endl;
    return 1;
  }

  // Create output file
  FILE* output;
  if (argc < 3){
    output = fopen("extracted.txt", "w");
  } else {
    output = fopen(argv[2], "w");
  }

  if (!output) {
    cout << "Could not create output file" << endl;
    fclose(fp);
    return 1;
  }

  // Decompress Text
  tree.decompressText(fp, output);

  cout << "\nDecompression Complete!" << endl;
  return 0;
}
