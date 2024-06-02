#include <iostream>
#include <fstream>
#include <vector>
using namespace std;


class Character{
public:
  char glyph;
  int freq;

  Character(int freq, char glyph){
    this->glyph = glyph;
    this->freq = freq;
  }
};

// Converts Frequency Array to Vector
std::vector<Character> freqArrayToVector(int* freqs, int length){

  int aend = 1 << 8;

  // Return Empty Vector if Array is not the right size
  vector<Character> vec = {};
  if(length != aend) return vec;

  // Place Character in Vector if Frequency is not Zero
  for(int i = 0; i < aend; i++){

    if (freqs[i] != 0){

      vec.emplace_back((char)i,freqs[i]);

    }

  }
  return vec;

}


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

 // Create Vector of Existing Characters
  cout << "From Vector: " << endl;
  std::vector<Character> chars = freqArrayToVector(freq, aend);


  // Make a Heap to Hold Frequencies
  // Create Huffman Tree
  // Compress Text
  // Write Text To File
  // Close All Files





  return 0;

}
