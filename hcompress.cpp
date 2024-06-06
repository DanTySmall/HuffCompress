#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
using namespace std;


class Character{
public:
  char glyph;
  int freq;
  Character* left;
  Character* right;

  Character(int freq, char glyph){
    this->glyph = glyph;
    this->freq = freq;
  }

};

class TreeNode{
public:
  char glyph;
  int freq;
  TreeNode* left;
  TreeNode* right;

  TreeNode(char glyph, int freq){
    this->glyph = glyph;
    this->freq = freq;
  }
};

// Comparator for Characters
struct compareChar{
    bool operator()(Character a, Character b){

      return a.freq > b.freq;

    }
  };

// Take The Frequency Array and create a Priority Queue
priority_queue<Character, std::vector<Character>, compareChar>createPQ(int* freqs, int length){

  int aend = 1 << 8;

  // Return Empty Vector if Array is not the right size
  std::priority_queue<Character, std::vector<Character>, compareChar> heap;
  if(length != aend || freqs == NULL) return heap;

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

// FIXME: Create Huffman Tree with Refrence Heaps
// FIXME: This might not work
// Takes a heap of Character Refrences and creates a Huffman Tree
Character* createHuffTree(priority_queue<Character, std::vector<Character>, compareChar> heap){

  // Trivial Sizes
  if (heap.size() < 1) return NULL;

  if (heap.size() < 2) {
    Character c = heap.top();
    heap.pop();
    return new Character(c.glyph, c.freq);
  }

  // While The Heap has more than one Character, Connect the two smallest
  while(heap.size() > 1){

    // Take The Smallest Nodes
    Character *left = new Character (heap.top());
    heap.pop();
    Character *right = new Character (heap.top());
    heap.pop();

    // Connect the via Intermediate Node
    Character IM = Character(left->freq + right->freq, (char)0);
    IM . left = left;
    IM . right = right;
    heap.push(IM);

  }

  if (heap.size() < 1) cout << "There is a problem creating the heap";
  // Retrieve  The Single Character in heap

  Character* result = new Character (heap.top());
  heap.pop();
  return result;

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

  // Compress Text
  // Write Text To File
  // Close All Files





  return 0;

}
