// Daniel T. Small
// hcompress.c
// Compresses a text file into a binary file

#include <stdio.h>
#include <stdlib.h>

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

// Returns the Character with the Lowest Frequency
Node* pop(Heap* h){

  Node* top = h -> alr -> array[0];

  // Move a the lower nodes up
  int curIndex = 0;

  // While still in heap
  while(curIndex < h->alr->size){

    // Place lower child in current index
    int left = (curIndex * 2) + 1;
    int right = left + 1;

    /* Percolate Down */
    if(h -> alr -> array[left] -> freq <= h -> alr -> array[right] -> freq ){ // Left is less

      h -> alr -> array[curIndex] = h -> alr -> array[left];
      h -> alr -> array[left] = NULL;
      curIndex = left;

    } else { // Right is less

      h -> alr -> array[curIndex] = h -> alr -> array[right];
      h -> alr -> array[right] = NULL;
      curIndex = right;

    }


  }

  // Reduce the size by 1
  h -> alr -> size = 1;

  return top;
}

// Pops all of the characters in the Heap

void popAll(Heap* h){

  // While The Heap is Not Empty Pop The Next Node
  while(h -> alr -> size > 0){
    Node* current = pop(h);

    // Print Characters
    printf("%c : %d", current -> character, current -> freq);

    // Free The Node
    freeNode(current);

  }

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
  int aend = 1 << 8; // Size of ascii
  int* freq = calloc(aend, sizeof(int));
  char c = getc(fp);

  /* printf("File Contents: \n"); */
  while(c != EOF){
    // Prints Character
    /* printf("%c", c); */

    // Adds to freq array
    freq[(int)c]++;

    c = getc(fp);
  }

  printf("\n\n");


  // Add to Frequencies the ArrayList
  ArrayList* arrayL = createAL();
  for(int i = 0; i < aend; i++ ){

    // If frequency is not 0 add to AL
    if (freq[i] == 0) continue;
    append(arrayL, (char)i, freq[i]);

  }

   Heap* heap = heapify(arrayL);
   printHeap(heap);


   // At This point you have a heap with all the characters in the text


  /* // Reopen file for parsing */
  /* fclose(fp); */
  /*   if (argc < 2){ */
  /*     fp = fopen("text.txt", "r"); */
  /*   }else{ */
  /*     fp = fopen(argv[0], "r"); */
  /*   } */


  /* fclose(fp); */

  printf("Howdy!\n");
  return 0;
}
