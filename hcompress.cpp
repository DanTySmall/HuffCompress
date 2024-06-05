#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
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


// // Converts Frequency Array to Vector
// std::vector<Character> freqArrayToVector(int* freqs, int length){

//   int aend = 1 << 8;

//   // Return Empty Vector if Array is not the right size
//   vector<Character> vec = {};
//   if(length != aend) return vec;

//   // Place Character in Vector if Frequency is not Zero
//   for(int i = 0; i < aend; i++){

//     if (freqs[i] != 0){

//       vec.emplace_back(freqs[i], i);


//     }

//   }
//   return vec;


// }

  struct compareChar{
    bool operator()(Character& a, Character& b){

      return a.freq > b.freq;

    }
  };

// Take The Frequency Array and create a Priority Queue
priority_queue<Character, std::vector<Character>, compareChar>createPQ(int* freqs, int length){

  int aend = 1 << 8;

  // Return Empty Vector if Array is not the right size
  std::priority_queue<Character, std::vector<Character>, compareChar> heap;
  if(length != aend) return heap;

  // Place Character in Priority Queue if Frequency is not Zero
  for(int i = 0; i < aend; i++){

    if (freqs[i] != 0){

      // Add to the Priorty Queue
      cout << i << ": " << (char) freqs[i] << " : " << heap.size() << endl;
      heap.emplace(freqs[i], i);

    }

  }
  return heap;

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



  // Use C++'s priority queue class to hold the characters

  // Create a Priority Queue
  std::priority_queue<Character, std::vector<Character>, compareChar> heap = createPQ(freq, aend);

  cout << heap.size() << endl;
  while (!heap.empty()) {
    cout << heap.top().freq << " " << endl;
    heap.pop();
  }

  // Create Huffman Tree
  // Compress Text
  // Write Text To File
  // Close All Files





  return 0;

}
