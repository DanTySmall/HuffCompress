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

  return 0;
}
