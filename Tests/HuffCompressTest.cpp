
#include <fstream>
#include <string>
#include <vector>
#include <cctype>
#include <algorithm>
#include <gtest/gtest.h>
#include "../hcompress.hpp"
#include "../hdecom.hpp"



bool compareASCIIFiles(const std::string& file1, const std::string& file2) {
    std::ifstream f1(file1);
    std::ifstream f2(file2);

    if (!f1.is_open() || !f2.is_open()) {
        return false;
    }

    char ch1, ch2;
    f1.get(ch1);
    f2.get(ch2);
    do {
        if (ch1 != ch2) {
            return false;
        }
        f1.get(ch1);
        f2.get(ch2);
    }while (!f1.eof() && !f2.eof());

    // Both files should end at the same time
    return f1.eof() && f2.eof();
}


TEST (CompressionTest, SameAfterCompression){
  system("../compress test1.txt && ../decompress compressed.txt test1decompressed.txt");
  EXPECT_TRUE(compareASCIIFiles("./test1.txt", "./test1decompressed.txt"));
}
