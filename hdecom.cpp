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
class HuffmanTree {
public:
  Node* tree = new Node('\0');
  FILE* createTree(FILE* fp){

  // NULL CHECK
  if (!fp ) return NULL;

  // Look at the Next Character and get its next code
  char c = fgetc(fp);

  // If no code, Can't Decompress
  if (!c) return NULL;
  if (c == 'X'){
    c = fgetc(fp);
    if(c == 'X') {
      cout << "No Codes. Can Decompress :(" << endl;
      return NULL;
    } else{
      fputc(c,fp);
      fputc('X', fp);
    }
  }


  // Create Nodes for All Characters
  while(1){

   // STOP CHECK
    if (c == 'X'){

      c = fgetc(fp);
      if(c == 'X') {

        c = fgetc(fp);
        if (c != '\n') fputc(c,fp);
        return fp;
      } else{

        if (c == ':') c = 'X';
      }
    }

    char glyph = c;

    // Check of Colon
    c = fgetc(fp);
    if (c != ':'){
      cout << "Standard Broken / File Corrupted :(" << endl;
      exit(1);
    }

    c = fgetc(fp);
    if (c != '0' && c != '1'){
      cout << "Standard Broken / File Corrupted :(" << endl;
      exit(1);
    }

    // Parse Code
    Node* current = tree;
    while(c == '0' || c == '1'){

      if(c == '0'){ // Going Left
        if (current -> left){ // Go to Node
          current = current -> left;
        }else{ // Create a new Node and go to it
          current -> left = new Node('\0');
          current = current -> left;
        }

      }else{ // Going Right (1)

        if (current -> right){ // Go to Node
          current = current -> right;
        }else{ // Create a new Node and go to it
          current -> right = (Node* ) calloc(1, sizeof(Node));
          current -> right = new Node('\0');
          current = current -> right;
        }
      }

      c = fgetc(fp);
   }

    // The Current Node is a character Node
    current -> glyph = glyph;

    // Seperated By NULL
    if (c != (char) 127) {
      cout << "Standard Broken / File Corrupted :(" << endl;
    }

    c = fgetc(fp);
    // At This Point, The Program has recieved the character and code
  }


  // The Program Should Have Broken out of the function by Now
  printf("Error Contructing Huffman Tree");
  exit(1);

  }

  void decompressText(FILE* input, FILE* output){


    // Extract Data Until End of File
    char c = fgetc(input);
    while(c == ' ' || c == '\n')  c = fgetc(input);
    Node* current = tree;
    int count = 0;

    while(c != EOF){

      if (count++ > 1000000){
        fclose(input);
        fclose(output);
        exit(0);
      }

      // Check if it is binary Data
      if(c != '0' && c != '1') {
        cout << "The Program is Trying to parse non-binary Data" << endl;
        exit(1);
      }

      // Go Left or Right
      if (c == '0'){
        current = current -> left;
      }else{
        current = current -> right;
      }

      // If the node has a character, you have decoded one letter
      if(current -> glyph != 0){
        fputc(current -> glyph, output);
        cout << current -> glyph;
        current = tree;
      }

      c = fgetc(input);
    }

    if (current != tree){
      printf("The Program did not end on a character" );
    }

    fclose(input);
    fclose(output);

  }

};

int main(int argc, char** argv) {


  cout << "===> Welcome to Huffcompress <===" << endl;
  cout << "=====> Extraction  Program <=====" << endl << endl;

  FILE* fp;
  if(argc < 2){
    fp = fopen("compressed.txt", "r");
    cout << "No File Specified. Extracting compressed.txt " << endl;
    cout << "Compressing File: compressed.txt" << endl << endl;
  } else {
    fp = fopen(argv[1], "r");
    cout << "Compressing File: " << argv[1] << endl << endl;
  }

  // Check if File Exists
  if (!fp){
    cout << "File was not found. Exiting Extraction." << endl;
    cout << "Specify file name after the command" << endl;
    cout << "compressed.txt is selected by default" << endl << endl;
    cout << "For Example: extract compressed.txt" << endl;
    return 1;
  }

  if (argc < 3){
    cout << "No Destination File Specified. Saving to extracted.txt" << endl;
    cout << "Saving to File: extracted.txt" << endl << endl;
  } else {

    cout << "Saving to File: " << argv[2] << endl << endl;
  }

  // Create Huffman Tree
  HuffmanTree tree = HuffmanTree();
  tree.createTree(fp);

  // Create A File The Holds the extracted Text
  FILE* output;
  if (argc < 3){
    output = fopen("extracted.txt", "w");
   } else {
    output = fopen(argv[1], "w");
   }

  // Decompress Text
  tree.decompressText(fp,output);

  // Done!
  cout << endl << "Extraction Complete" << endl;
  if(argc < 3){
    cout << "Extracted File Saved As: extracted.txt" << endl;
  }else {
    cout << "Extracted File Saved As: " << argv[2] << endl;
  }

  return 0;
}
