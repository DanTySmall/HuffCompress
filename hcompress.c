#include <stdio.h>
#include <stdlib.h>


typedef struct node{

  char character;
  int freq;
  struct node* left;
  struct node* right;

}Node;


typedef struct ArrayList{

  Node* array;
  int size;
  int capacity;

}ArrayList;

// ArrayList of Nodes
ArrayList* createAL(){

  // Allocate Space
  ArrayList* new = malloc(sizeof(ArrayList));
  new -> array = calloc(4, sizeof(Node));
  new -> size = 0;
  new -> capacity = 4;

  return new;
}

ArrayList* append(ArrayList* al, char character, int freq){


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
  al -> array[al -> size].left = NULL;
  al -> array[al -> size].right = NULL;
  al -> size++;

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
  int aend = 1 >> 8; // Size of ascii
  int* freq = calloc(1>>8, sizeof(int));
  char c = getc(fp);

  printf("File Contents: \n");
  while(c != EOF){
    // Prints Character
    printf("%c", c);

    // Adds to freq array
    freq[(int)c]++;

    c = getc(fp);
  }

  // printf("\n\n");


  return 0;
}
