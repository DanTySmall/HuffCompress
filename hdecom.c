#include <stdio.h>
#include <stdlib.h>

// Extracts Text from a compressed File
// TODO: If you change to binary you will have to change enforcement of standards

typedef struct node{

  char character;
  struct node* left;
  struct node* right;

}Node;

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


    // Create Node For Character
    Node* newNode = (Node* ) calloc(1, sizeof(Node));
    newNode -> character = c;
    // Both Children are aleady set to NULL

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
    c = fgetc(fp);
    Node* current = t;
    while(c == '0' || c == '1'){

      if(c == '0'){ // Going Left
        if (current -> left){ // Go to Node
          current = current -> left;
        }else{ // Create a new Node and go to it
          current -> left = (Node* ) calloc(1, sizeof(Node));
          current = current -> left;
        }

      }else{ // Going Right (1)

        if (current -> right){ // Go to Node
          current = current -> right;
        }else{ // Create a new Node and go to it
          current -> right = (Node* ) calloc(1, sizeof(Node));
          current = current -> right;
        }
      }

    }

    // At This Point, The Program has recieved the character and code

   printf("Error Contructing Huffman Tree");
   exit(1);
  }


}

int main(int argc, char *argv[]) {

  // Read Data From File
  FILE* fp;
  if(argc < 2){
    fp = fopen("compressed.text", "r");
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

}
