#include <iostream>
#include <fstream>
using namespace std;

int main(){

  // Read File
  fstream source;
  source.open("text.txt", ios::in);
  char message[100];

  source >> message;

  cout << "Printing File Contents";
  cout << message;
  // Put Contents of File to a Frequency Array
  // Make a Heap to Hold Frequencies
  // Create Huffman Tree
  // Compress Text
  // Write Text To File
  // Close All Files





  return 0;

}
