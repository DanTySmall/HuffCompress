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
    this->freq = freq;
    this->glyph = glyph;
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

class Heap {
public:
  std::priority_queue<Character, std::vector<Character>, compareChar> heap;

  Heap(int* freqs, int length){
    heap = createPQ(freqs, length);
  }

  priority_queue<Character, std::vector<Character>, compareChar> createPQ(int* freqs, int length){
    int aend = 1 << 8;
    std::priority_queue<Character, std::vector<Character>, compareChar> heap;

    if(length != aend || freqs == NULL) return heap;

    for(int i = 0; i < aend; i++){
      if (freqs[i] != 0){
        heap.emplace(freqs[i], i);
      }
    }
    return heap;
  }

  priority_queue<Character, std::vector<Character>, compareChar> getHeap(){
    return heap;
  }
};

class HuffmanTree {
public:
  int aend = (1 << 8);
  int count;
  Character* tree;
  char** codes;
  vector<char> path = {};

  HuffmanTree(priority_queue<Character, std::vector<Character>, compareChar> heap){
    tree = createHuffTree(heap);
    count = heap.size();

    codes = new char*[aend];
    std::fill(codes, codes + aend, (char*)NULL);
    getHuffcodes(tree, codes, path, 0);
  }

  Character* createHuffTree(priority_queue<Character, std::vector<Character>, compareChar> heap){
    if (heap.size() < 1) return NULL;

    if (heap.size() < 2) {
      Character c = heap.top();
      heap.pop();
      return new Character(c.freq, c.glyph);  // Fixed: correct order
    }

    while(heap.size() > 1){
      Character *left = new Character(heap.top());
      heap.pop();
      Character *right = new Character(heap.top());
      heap.pop();

      Character IM = Character(left->freq + right->freq, (char)0);
      IM.left = left;
      IM.right = right;
      heap.push(IM);
    }

    if (heap.size() < 1) {
      cout << "There is a problem creating the heap" << endl;
      return NULL;
    }

    Character* result = new Character(heap.top());
    heap.pop();
    return result;
  }

  void getHuffcodes(Character* tree, char** codes, vector<char> path, int position){
    if (tree == NULL) return;

    if(tree->glyph != (char)0){
      codes[(int)tree->glyph] = new char[position + 1];

      for (int i = 0; i < position; i++) {
        codes[(int)tree->glyph][i] = path.at(i);
      }
      codes[(int)tree->glyph][position] = '\0';
    }

    path.push_back('0');
    getHuffcodes(tree->left, codes, path, position + 1);
    path.pop_back();

    path.push_back('1');
    getHuffcodes(tree->right, codes, path, position + 1);
    path.pop_back();
  }

  void printHuffcodes(){
    cout << "Printing Huffman Codes for Tree:" << endl;
    for (int i = 0; i < aend; i++){
      if (codes[i]){
        cout << (char)i << " : " << codes[i] << endl;
      }
    }
    cout << endl;
  }

  void compressText(FILE* input, FILE* output){
    // Print Codes to File
    for(int i = 0; i < aend; i++) {
      if (codes[i]) {
        fprintf(output, "%c:%s%c", (char)i, codes[i], (char)127);
      }
    }

    // Separate Code From Text
    fprintf(output, "XX\n");

    // Print Compressed Text to File
    char c = getc(input);
    while(!feof(input)){
      cout << (int)c << endl;
      fprintf(output, "%s", codes[(int)c]);
      c = getc(input);
    }

    fclose(input);
    fclose(output);
  }
};

int main(int argc, char *argv[]){
  FILE* fp;
  if (argc < 2){
    fp = fopen("text.txt", "r");
  } else {
    fp = fopen(argv[1], "r");
  }

  if (!fp) {
    cout << "Error: Could not open input file" << endl;
    return 1;
  }

  int total = 0;
  int aend = 1 << 8;
  int* freq = new int[aend];
  std::fill(freq, freq + aend, 0);

  cout << "Printing File Contents\n";
  char c = getc(fp);
  while(c != EOF){
    if (c < 0 || c > 127){
      cout << endl << "ASCII Files Only!!" << endl;
      exit(2);
    }
    cout << c;
    if (c != '\0'){
      freq[(int)c]++;
      total++;
    }
    c = getc(fp);
  }
  fclose(fp);
  cout << endl;

  Heap PQ = Heap(freq, aend);
  priority_queue<Character, std::vector<Character>, compareChar> heap = PQ.getHeap();
  HuffmanTree tree = HuffmanTree(heap);

  tree.printHuffcodes();

  // Reopen File
  if (argc < 2){
    fp = fopen("text.txt", "r");
  } else {
    fp = fopen(argv[1], "r");
  }

  FILE* output;
  if (argc < 3){
    output = fopen("compressed.txt", "w");
  } else {
    output = fopen(argv[2], "w");
  }

  tree.compressText(fp, output);
  cout << "Compression Complete!" << endl;

  return 0;
}
