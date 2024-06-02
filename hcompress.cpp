#include <iostream>
#include <fstream>
using namespace std;

int main(int argc, char *argv[]){

  // Read File
  FILE* fp;
  if (argc < 2){
    fp = fopen("text.txt", "r");
  }else{
    fp = fopen(argv[1], "r");
  }


  // Create Array to Hold Character Frequencies
  int total = 0;
  int aend = 1 << 8;
  int* freq = new int[aend];
  std::fill(freq, freq + aend, 0);

  // Print Contents of File
  cout << "Printing File Contents\n";
  char c = getc(fp);
  while(c != EOF){

    // Prints Character and adds to Frequency Array
    cout << c;

    if (c == '\0'){
      c = getc(fp);
      continue;
    }

    freq[(int)c]++;
    total++;
    c = getc(fp);


  }
  fclose(fp);

  // Print all characters in array
  cout << "Character Frequencies\n";
  for (int i = 0; i < aend; i++){

    if (freq[i] != 0){
      cout << (char)i << " : " << freq[i] << endl;
    }

  }





  // Put Contents of File to a Frequency Array
  // Make a Heap to Hold Frequencies
  // Create Huffman Tree
  // Compress Text
  // Write Text To File
  // Close All Files





  return 0;

}
