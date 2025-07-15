##
# HuffCompress
#
# Makefile Used for Fast and Easy Compilation
#
# @file
# @version 0.1

.PHONY: all clean

compress:
	g++ hcompress.cpp -o compress

decompress:
	g++ hdecom.cpp -o decompress

all: compress decompress

clean:
	rm -f compress decompress

# end
