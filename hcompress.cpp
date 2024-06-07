#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
using namespace std;
// TODO: Be more Object Oriented
//TODO: What if getHuffcodes gets a single character with only one node?
//TODO: Properly Free Structures
//TODO: Make a more user friendly interface

class Character{
public:
  char glyph;
  int freq;
  Character* left;
  Character* right;

  Character(int freq, char glyph){
    this->glyph = glyph;
    this->freq = freq;
    this->left = NULL;
    this->right = NULL;
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
      // cout << i << ": " << (char) freqs[i] << " : " << heap.size() << endl;
      heap.emplace(freqs[i], i);

    }

  }
  return heap;

}

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

// Creates a Matrix of Huffman for Characters
void getHuffcodes (Character* tree, char** codes, vector<char> path, int position){

  // NULL Check
  // cout << "abc123"<< endl;
  if (tree == NULL) return;
  // Check if Character Node
  // cout << tree -> glyph << endl;
  if(tree -> glyph != (char)0){

    // cout << "At Character " << tree -> glyph;
    // Create an array For Character path
    codes[(int)tree -> glyph] = new char[1 + position * sizeof(char)];

    // Write Path for Character
    for (int i = 0; i < position; i++) codes[(int)tree -> glyph][i] = path.at(i);

    // Null Character
    codes[(int)tree -> glyph][position] = '\0';

  }

  // Generate Codes for Children and Update Path
  path.push_back('0');
  getHuffcodes(tree -> left, codes, path, position + 1);
  path.pop_back();
  path.push_back('1');
  getHuffcodes(tree -> right, codes, path, position + 1);
  path.pop_back();



  // The matrix should be filled with all of the huffman codes
  }

// Prints Characters in Huffman Tree and their frequencies
void printHuffFreqs(Character * tree){

  if(tree == NULL) return;

  // Print This Node
  cout << tree -> glyph << " : " << tree -> freq ;

  // Print Child Nodes
  printHuffFreqs(tree -> left);
  printHuffFreqs(tree -> right);

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
  cout << endl;

  // Use C++'s priority queue class to hold the characters
  // Create a Priority Queue
  std::priority_queue<Character, std::vector<Character>, compareChar> heap = createPQ(freq, aend);

  // Create Huffman Tree
  Character* Tree = createHuffTree(heap);

  // Print Codes From Huffman Tree
  //   Matrices for Huffman Codes
  char** codes = new char*[aend];
  vector<char> path = {};
  std::fill(codes, codes + aend, (char*)NULL);

  getHuffcodes(Tree, codes, path, 0);

  //   Prints all the Huffman codes
  for (int i = 0; i < aend; i++){

    if (codes[i] != NULL){
      cout << (char)i << " : " << codes[i] << endl;
    }

  }

  // Compress Text
  //   Reopen File
  if (argc < 2){
    fp = fopen("text.txt", "r");
  }else{
    fp = fopen(argv[1], "r");
  }
  // Write Text To File
  // Open New File for Writing

  FILE* output;
  if (argc < 3){
    output = fopen("compressed.txt", "w");
  } else {
    output = fopen(argv[2], "w");
  }

  // Printing the Huffman Codes to File
  for(int i = 0; i < aend; i++) if (codes[i]) fprintf(output, "%c:%s%c", (char)i, codes[i], (char) 127);

  // Separate Code From Text
  fprintf(output, "XX\n ");

  // Print Compressed Text to File
   c = getc(fp);
   while(c != EOF){

     // Looks up Character and Prints Code to File
     fprintf(output,"%s", codes[(int)c]);

     c = getc(fp);
   }

   cout << endl;
   // Close All Files
   fclose(output);
   fclose(fp);

   cout << "Compression Complete!" << endl;

  return 0;

}
