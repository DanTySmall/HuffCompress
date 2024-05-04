// Daniel T. Small
// hcompress.c
// Compresses a text file into a binary file

#include <stdio.h>
#include <stdlib.h>

// TODO: Last Night's Notes: Too Tired to Think. In the morning finish the the percDown Function and finish the heap
// TODO: Make Functions that free structures
// TODO: What do you do when percolating a null value

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

// !!! IMPORTANT !!! : At the time of making this, All Node do not have childree (For Huffman Tree)
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


int main(int argc, char const *argv[]) {

  // Read From a File

  // If File specified, open it
    // If not, use text.txt
  FILE* fp;
    if (argc < 2){
      fp = fopen("text.txt", "r");
    }else{
      fp = fopen(argv[0], "r");
    }

  // Print Contents of File and Create Freq Array
  int total = 0;
  int aend = 1 << 8; // Size of ascii
  int* freq = calloc(aend, sizeof(int));
  char c = getc(fp);

  /* printf("File Contents: \n"); */
  while(c != EOF){
    // Prints Character
    /* printf("%c", c); */

    // Adds to freq array
    freq[(int)c]++;
    total++;
    c = getc(fp);
  }
  printf("Total # of characters captured: %d", total);

  printf("\n\n");


  // Add to Frequencies the ArrayList
  ArrayList* arrayL = createAL();
  for(int i = 0; i < aend; i++ ){

    // If frequency is not 0 add to AL
    if (freq[i] == 0) continue;
    append(arrayL, (char)i, freq[i]);

  }

   Heap* heap = heapify(arrayL);
   /* printHeap(heap); */
   /* printf("\n\n After: \n\n"); */
   /* popAll(heap); */

   // At This point you have a heap with all the characters in the text

   // Create The Huffman Tree
   Node* tree = createHuffTree(heap);

   /* printf("Heap Size: %d", heap -> alr -> size); */
   /* printf("Root of the Tree is %c with a frequency of %d", tree -> character, tree -> freq); */
   printf("The Left of the Root is %c with a frequency of %d\n", tree -> left -> character, tree -> left -> freq);
   printf("The Right of the Root is %c with a frequency of %d\n", tree -> right -> character, tree -> right -> freq);


  /* // Reopen file fsing */
  /* fclose(fp); */
  /*   if (argc < 2){ */
  /*     fp = fopen("text.txt", "r"); */
  /*   }else{ */
  /*     fp = fopen(argv[0], "r"); */
  /*   } */


  /* fclose(fp); */

  return 0;
}
