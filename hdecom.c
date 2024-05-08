#include <stdio.h>
#include <stdlib.h>

// Extracts Text from a compressed File

typedef struct node{

  char character;
  struct node* left;
  struct node* right;

}Node;

void printInOrder(Node* t){

  // NULL CHECK
  if (!t) return;

  printInOrder(t -> left);

  // Print Node
  if (t -> character == 0) printf("NULL\n");
  else printf("%c\n", t -> character);

  printInOrder(t -> right);
}

// Recreates the Huffman Tree
FILE* createTree(FILE* fp, Node* t){

  // NULL CHECK
  if (!fp || !t) return NULL;

  // Look at the Next Character and get its next code
  char c = fgetc(fp);

  // If no code, Can't Decompress
  if (!c) return NULL;
  if (c == 'X'){
    c = fgetc(fp);
    if(c == 'X') {
      printf("No Codes. Can Decompress :(");
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

        fputc(c,fp);
        fputc('X', fp);
      }
    }

    char glyph = c;
    // Create Node For Character
    /* Node* newNode = (Node* ) calloc(1, sizeof(Node)); */
    /* newNode -> character = c; */
    /* // Both Children are aleady set to NULL */

    // Check of Colon
    c = fgetc(fp);
    if (c != ':'){
      printf("Standard Broken / File Corrupted :(");
      exit(1);
    }

    c = fgetc(fp);
    if (c != '0' && c != '1'){
      printf("Standard Broken / File Corrupted :(");
      exit(1);
    }


    // Parse Code
    /* c = fgetc(fp); */
    Node* current = t;
    while(c == '0' || c == '1'){

      if(c == '0'){ // Going Left
        if (current -> left){ // Go to Node
          current = current -> left;
        }else{ // Create a new Node and go to it
          current -> left = (Node* ) calloc(1, sizeof(Node));
          /* printf("NumNodes: %d\n", ++numNodes); */
          current = current -> left;
        }

      }else{ // Going Right (1)

        if (current -> right){ // Go to Node
          current = current -> right;
        }else{ // Create a new Node and go to it
          current -> right = (Node* ) calloc(1, sizeof(Node));
          /* printf("NumNodes: %d\n", ++numNodes); */
          current = current -> right;
        }
      }

      c = fgetc(fp);
   }

    // The Current Node is a character Node
    current -> character = glyph;

    // Seperated By NULL
      if (c != (char) 127) {
        printf("Standard Broken / File Corrupted :(");
      }

      c = fgetc(fp);
    // At This Point, The Program has recieved the character and code

  }

  // The Program Should Have Broken out of the function by Now
  printf("Error Contructing Huffman Tree");
  exit(1);

}

int main(int argc, char *argv[]) {

  // Read Data From File
  FILE* fp;
  if(argc < 2){
    fp = fopen("compressed.txt", "r");
  } else {
    fp = fopen(argv[0], "r");
  }

  // Check if File Exists
  if (!fp){
    printf("File not found :(");
    return 1;
  }

  // Start parsing data
  Node* root = (Node* ) malloc(sizeof(Node*));
  fp  = createTree(fp, root);

  // At This Point, You Have Reconstructed the Huffman Tree

  // Create A File The Holds the extracted Text
  FILE* output;
  if (argc < 3){
    output = fopen("extracted.txt", "w");
   } else {
    output = fopen(argv[1], "w");
   }

  // Extract Data Until End of File
  char c = fgetc(fp);
  while(c == ' ' || c == '\n')  c = fgetc(fp);
  Node* current = root;
  int count = 0;
  while(c != EOF){

    if (count++ > 1000000){
      fclose(fp);
      fclose(output);
      exit(0);
    }

    // Check if it is binary Data
    if(c != '0' && c != '1') {
      printf("The Program is Trying to parse non-binary Data");
      exit(1);
    }

    // Go Left or Right
    if (c == '0'){
      current = current -> left;
    }else{
      current = current -> right;
    }

    // If the node has a character, you have decoded one letter
    if(current -> character != 0){
      fputc(current -> character, output);
      current = root;
    }

    c = fgetc(fp);
  }

  if (current != root){
    printf("The Program did not end on a character" );
  }

  fclose(fp);
  fclose(output);

}
