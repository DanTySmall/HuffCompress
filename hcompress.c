#include <stdio.h>
#include <stdlib.h>


int main(int argc, char const *argv[]) {
  /* code */
  // Read From a File

  // If File specified, open it
  // If not, use text.txt
  FILE* fp;
    if (argc < 2){
      fp = fopen("text.txt", "r");
    }else{
      fp = fopen(argv[0], "r");
    }

  printf("File Contents: \n");


  // Print Contents of File and Create Freq Array
  int aend = 1 >> 8; // Size of ascii
  int* freq = calloc(1>>8, sizeof(int));
  char c = getc(fp);
  while(c != EOF){
    // Prints Character
    printf("%c", c);

    // Adds to freq array
    freq[(int)c]++;

    c = getc(fp);
  }

  printf("\n\n");

  return 0;
}
