#include <iostream>
#include <fstream>
using namespace std;

int main(int argc, char *argv[]){

  // Read File
  fstream source;

  // Source Handling
  FILE* fp;
  if (argc < 2){
    fp = fopen("text.txt", "r");
  }else{
    fp = fopen(argv[1], "r");
  }

  // Print Contents of File
  cout << "Printing File Contents\n";
  char c = getc(fp);
  while(c != EOF){

    // Prints Character
    cout << c;
    c = getc(fp);

  }
  fclose(fp);







  // Put Contents of File to a Frequency Array
  // Make a Heap to Hold Frequencies
  // Create Huffman Tree
  // Compress Text
  // Write Text To File
  // Close All Files





  return 0;

}
