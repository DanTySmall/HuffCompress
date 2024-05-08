// Daniel T. Small
// hcompress.c
// Compresses a text file into a binary file

#include <stdio.h>
#include <stdlib.h>

// Overall Works, Needs to be Fortified
// TODO: Make Functions that free structures
// TODO: What do you do when percolating a null value
// TODO: Allow for custom file output name

typedef struct node{

  char character;
  int freq;
  struct node* left;
  struct node* right;

}Node;


typedef struct arrayList{

  Node* array;
  int size;
  int capacity;

}ArrayList;

typedef struct ArrayListRef{

  Node** array;
  int size;
  int capacity;

}ArrayListRef;

typedef struct heap{

  ArrayListRef* alr;

}Heap;


void freeNode(Node* n){

  // Free Children
  if (n -> left) freeNode(n -> left);
  if (n -> right) freeNode(n -> right);

  // Free Node
  free(n);

}

// Prints The Heap Contents
void printHeap(Heap* h){

  // Loop Through The Heap Array
  for(int pos = 0; pos < h -> alr -> size; pos++)
    printf("%c : %d \n", h -> alr -> array[pos] -> character, h -> alr -> array[pos] -> freq);

}

// ArrayList of Nodes
ArrayList* createAL(){

  // Allocate Space
  ArrayList* new = malloc(sizeof(ArrayList));
  new -> array = calloc(4, sizeof(Node));
  new -> size = 0;
  new -> capacity = 4;

  return new;
}

// Add Character to ArrayList
void append(ArrayList* al, char character, int freq){


  if(al -> size == al -> capacity){ // If theres no space, expand array

    Node* temp = realloc(al -> array, 2 * al->capacity * sizeof(Node));
    if (temp)
    {

     al -> array = temp;
     al -> capacity *= 2;
    }

  }


  // Add Node to array
  al -> array[al -> size].character = character;
  al -> array[al -> size].freq = freq;
  al -> array[al -> size].left= NULL;
  al -> array[al -> size].right = NULL;
  al -> size++;
}

// Prints the contents of a Frequency Array witout Zero Frequencies
void printArray(int* freqs){

  int aend = 1 << 8;
  for (int pos = 0; pos < aend; pos++)
    if(freqs[pos] != 0)
      printf("%c : %d \n", (char)pos, freqs[pos]);

}

// Creates ArrayList of Nodes Refrence
ArrayListRef* createALR(){

  // Allocate Space
  ArrayListRef* new = malloc(sizeof(ArrayListRef));
  new -> array = calloc(4, sizeof(Node*));
  new -> size = 0;
  new -> capacity = 4;

  return new;
}

// Adds Node to ArrayListRef
void appendR(ArrayListRef* alr, Node* n){

  if(alr -> size == alr -> capacity){ // If theres no space, expand array

    Node** temp = realloc(alr -> array, 2 * alr->capacity * sizeof(Node*));
    if (temp)
    {

     alr -> array = temp;
     alr -> capacity *= 2;
    }

  }


  // Add Node Pointer to array
  alr -> array[alr -> size] = n;
  alr -> size++;

}

// Creates a Heap
Heap* createH(){

  // Allocate Memory
  Heap* h = malloc(sizeof(Heap));
  h -> alr = createALR();

  // Return Heap
  return h;

}

void percolateUp(Heap* h, int index){

  // NULL Check
  if(!h || !index) return;

  // Check if node's frequency is less than the node above it swap
  int parent = (index - 1) /2;

  if(h -> alr -> array[index]->freq < h -> alr -> array[parent]->freq)
  {
    // Swap
    Node* temp = h -> alr -> array[index];
    h -> alr -> array[index] = h -> alr -> array[parent];
    h -> alr -> array[parent] = temp;

  }else{
    return;
  }

  percolateUp(h,parent);

}

// Inserts Node into Heap
void insert(Heap* h, Node* n){

  // Place Node at End of Heap
  appendR(h -> alr, n);

  // Percolate up
  percolateUp(h, h -> alr -> size - 1);

}

// Heapifies an ArrayList
Heap* heapify (ArrayList* al){

  // Allocate Memory
  Heap* h = createH();

  // Place every node in heap
  for(int position = 0; position < al -> size; position++){

    insert(h, &al -> array[position]);

  }

  // Return Heap
  return h;


}

void percDown(Heap* h, int index){

  // If out of heap, return
  if (index >= h -> alr -> size) return;

  // Children
  int left = (index * 2) + 1;
  int right = left + 1;

  int target = 0;
  if (left < h -> alr -> size && right < h -> alr -> size) { // If Both left and right are in heap

    // Check Which one one is whick one is less
    // If left is less than right and left is less index
    if (h -> alr -> array[left] -> freq <= h -> alr -> array[right] -> freq && h -> alr -> array[left] -> freq < h -> alr -> array[index] -> freq ) { // Left is the Target
      target = left;
    } else if (h -> alr -> array[right] -> freq < h -> alr -> array[index] -> freq ){ // Right is the Target
      target = right;
    } else return;

    // Swap Index with Target
    Node* temp = h -> alr -> array[index];
    h -> alr -> array[index] = h -> alr -> array[target];
    h -> alr -> array[target] = temp;

    // Recursive Call
    percDown(h, target);

  } else if (left < h -> alr -> size){ // Only has left node

    // Check if left is less than target
    if (h -> alr -> array[left] < h -> alr -> array[index]  ){

      // Swap The Index and Left Child
      Node* temp = h -> alr -> array[index];
      h -> alr -> array[index] = h -> alr -> array[left];
      h -> alr -> array[left] = temp;

    }
  }


}

// Returns the Character with the Lowest Frequency
Node* pop(Heap* h){

  Node* top = h -> alr -> array[0];

  // Put last node as first
  h -> alr -> array[0] = h -> alr -> array[h -> alr -> size - 1];

  // Reduce the size by 1
  h -> alr -> size--;

  // Put top node in the right place
  percDown(h, 0);

  return top;
}

// Pops all of the characters in the Heap
void popAll(Heap* h){

  // While The Heap is Not Empty Pop The Next Node
  while(h -> alr -> size > 0){
    Node* current = pop(h);

    // Print Characters
    printf("%c : %d \n", current -> character, current -> freq);


    // Set Position to 0
    current = NULL;
  }

}

// Creates a Huffman Tree Based on contents of a heap
Node* createHuffTree(Heap* h){

  // If heap is empty or doesnt exist return NULL
  if (!h || h -> alr -> size == 0) return NULL;

  // While the contents of the heap is greater in one
  while (h -> alr -> size > 1){

    // Get Two nodes with the lowest frequecies
    Node* left = pop(h);
    Node* right = pop(h);

    // Create Intermediate Node
    Node* IN = (Node*) calloc (1, sizeof(Node));
    // IN -> char is already 0
    IN -> freq = left -> freq + right -> freq;

    // Connect nodes to an intermediate node
    IN -> left = left;
    IN -> right = right;

    // Put Intermediate node intermediate node in heap with children attached
    insert(h,IN);

  }

  // At this point there should only be one node that is the roor of the Huffman Tree

  // Return Root of Tree
  return h -> alr -> array[0];


}

// Traverses The Huffman Tree
void printHuffTree(Node* t){

  // If Null Return
  if (!t) return;

  // Print This Node
  printf("%c : %d\n", t -> character, t -> freq);

  // Print Child Nodes
  printHuffTree(t -> left);
  printHuffTree(t -> right);

}

void printInOrder(Node* t){

  // NULL CHECK
  if (!t) return;

  printInOrder(t -> left);

  // Print Node
  if (t -> character == 0) printf("NULL\n");
  else printf("%c\n", t -> character);

  printInOrder(t -> right);
}


// Find the Total number of character nodes
int charNodes(Node* t){

  // If Node does not exist return o
  if (!t) return 0;

  // Check if this node is a character node
  int total = 0;
  if (t -> character != 0 ) total = 1;

  // Find # of child nodes
  total = charNodes(t -> left) + charNodes(t -> right);

  // Return # of character nodes
  return total;

}

// Prints The Abbreviation of all used characters
/* void printAbbrs(Node* t, char* path, int position){ */

/*   // If no node, return */
/*   if (!t) return; */

/*   // If no path, create an array with size of unique characters */
/*   if (!path) path = (char* ) malloc(charNodes(t) * sizeof(char)); */

/*   // If this is a character node, print its abbreviation */
/*   if (t -> character != 0) { */

/*     // Print character and Abbreviation */
/*     printf("%c:", t -> character); */
/*     for (int i = 0; i < position ; i++){ */

/*       if ((int)path[i] % 2){ */
/*         printf("1"); */
/*       }else{ */
/*         printf("0"); */
/*       } */

/*     } */

/*     printf("XX"); */

/*   } */

/*   // Print Childrens' Abbreviations */
/*   path[position] = (char) 0; */
/*   printAbbrs(t -> left, path, position + 1); */
/*   path[position] = (char) 1; */
/*   printAbbrs(t -> right, path, position + 1); */



/* }  */

// OPTIMIZE: you can calloc the memory and you string methods
// Generates a matrix with all Huffman codes
void getHuffCodes(Node* t, char** codes, char* path, int position){

  // If no tree or code matrix, return
  if (!t || !codes) return;

  // Check if this is a char node. if so, add it to the matrix
  if (t -> character){

    codes[(int)t -> character] = (char* ) malloc(1 + position * sizeof(char));

    // First the Letter
    codes[(int)t -> character][0] = t -> character;

    // Then the Path
    for (int i = 0; i < position; i++) codes[(int)t -> character][i] = path[i];

    // Null Character
    codes[(int)t -> character][position] = '\0';
  }


  // Generate Codes for Children
  // Update Path

  path[position] = '0';
  getHuffCodes(t -> left, codes, path, position + 1);
  path[position] = '1';
  getHuffCodes(t -> right, codes, path, position + 1);
  path[position] = '\0';

  // The matrix should be filled with all of the huffman codes
}

int main(int argc, char const *argv[]) {

  // Read From a File

  // If File specified, open it
    // If not, use text.txt
  FILE* fp;
    if (argc < 2){
      fp = fopen("best.txt", "r");
    }else{
      fp = fopen(argv[0], "r");
    }

  // Print Contents of File and Create Freq Array
  int total = 0;
  int aend = 1 << 8; // Size of ascii
  int* freq = calloc(aend, sizeof(int));
  char c = getc(fp);

  while(c != EOF){
    // Prints Character

    // Adds to freq array
    if (c == '\0'){
      c = getc(fp);
      continue;
    }

    freq[(int)c]++;
    total++;
    c = getc(fp);
  }
  fclose(fp);
  /* printf("Total # of characters captured: %d", total); */

  /* printf("\n\n"); */

  // Add to Frequencies the ArrayList
  ArrayList* arrayL = createAL();
  for(int i = 0; i < aend; i++ ){

    // If frequency is not 0 add to AL
    if (freq[i] == 0) continue;
    append(arrayL, (char)i, freq[i]);

  }

   // Make Heap
   Heap* heap = heapify(arrayL);
   int count = heap -> alr -> size;
   // At This point you have a heap with all the characters in the text

   // Create The Huffman Tree
   Node* tree = createHuffTree(heap);
   /* printInOrder(tree); */

   // Print The Huffman Tree
   /* printHuffTree(tree); */

   // Create List of Abbreviations
   // 2D Array

   // Matrix for Codes
   char** codes = (char**) calloc(aend, sizeof(char*));
   char path [count];
   getHuffCodes(tree, codes, path, 0);

   // Compressing The Text


   // Reopen Text File for Compression
   if (argc < 2){
     fp = fopen("best.txt", "r");
    }else{
     fp = fopen(argv[0], "r");
   }

   // Open New File for Writing
   FILE* output;
   if (argc < 3){
     output = fopen("compressed.txt", "w");
   } else {
     output = fopen(argv[1], "w");
   }

   // Print Huffman Codes to File
   // [Character] : [Code][Special Character]
   for(int i = 0; i < aend; i++) if (codes[i]) fprintf(output, "%c:%s%c", (char)i, codes[i], (char) 127);

   // Separate Codes and Text with n
   fprintf(output, "XX\n ");

   // Go Through Text and compress
   c = getc(fp);
   while(c != EOF){

     // Looks up Character and Prints Code to File
     char* code = codes[(int) c];
     fprintf(output,"%s", code);

     // Adds to freq array
     c = getc(fp);
   }




  fclose(output);
  fclose(fp);

  return 0;
}
