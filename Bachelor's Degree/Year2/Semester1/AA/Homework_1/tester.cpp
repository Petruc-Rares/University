#include <iostream>
#include <fstream>
#include <iterator>
#include <string>
#include <iostream>
#include <string>
#include <queue>
#include <unordered_map>
#include <fstream>
#include <streambuf>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <vector>
#include <bits/stdc++.h>

#include "readWriteToFile.h"
#include "Huffman.h"
#include "LZW.h"

#define COMPRESS true
#define DECOMPRESS false
#define HUFFMAN 1
#define LZW 0

typedef unsigned int int32;
typedef short int16;
typedef unsigned char byte;

using namespace std;

void show_usage() {
	cout << "USAGE: ./tester algorithm input_file compressed_file decompressed_file\n";
	cout << "DESCRIPTION:\n";
	cout << "Parameter algorithm can be either Huffman or LZW\n";
	cout << "The program takes the path to an input file, \n";
	cout << "it compresses and decompresses it and \n";
	cout << "writes the results in the target files: \"compressed_file\",\n";
	cout << "respectively \"decompressed_file\".\n";
}

int main (int argc, char* argv[]) {

	if (argc != 5) {
		show_usage();	
		return -1;
	}

	// set to 1, when is Huffman
	// set to 0, when is LZW
	// other value, when algorithm is not known
	unsigned algorithm_indicator;
	// Determine the desired compression algorithm
	string name_algorithm(argv[1]);
	if (name_algorithm == "Huffman") {
		algorithm_indicator = HUFFMAN;
	} else if (name_algorithm == "LZW") {
		algorithm_indicator = LZW;
	} else {
		cout << "Compression algorithm not known\n";
		show_usage();
		return -1;
	}

	string inputFileName(argv[2]);
	string compressedFileName(argv[3]);
	string decompressedFileName(argv[4]);
	string extension = inputFileName.substr(inputFileName.size() - 3);

	Node *root = nullptr;
	byte *inputData;
	int32 width;
	int32 height;
	int32 bytesPerPixel;
	int32 totalSize;

	if (algorithm_indicator == HUFFMAN) {

		if (extension == "bmp") {
			// read the input image
		    readImage(inputFileName.c_str(), &inputData, &width, &height, &bytesPerPixel, &totalSize);
		} else if (extension == "txt") {
			// read the input text
			inputData = readText(inputFileName.c_str(), &totalSize);
		}

		// encoded the inputData to text (compress it) and write the result
		// to "compressed_file"
		vector<byte> encodedTextBytes = encodeText(inputData, &root, totalSize);
		vector<byte> encodedTextBits = toBits(encodedTextBytes);
		writeText(compressedFileName, encodedTextBits);

		// decode the "compressed_file"
		vector<byte> retrievedEncodedText = readEncodedHuffman(compressedFileName);
		vector<byte> decodedText = decodeText(retrievedEncodedText, root);

		// write the result to "decompressed_file"		
		if (extension == "bmp") {
		    byte *p = &*decodedText.begin();
			writeImage(decompressedFileName.c_str(), p, width, height, bytesPerPixel);
		} else if (extension == "txt") {
			writeText(decompressedFileName, decodedText);
		}
	} else if (algorithm_indicator == LZW) {

		if (extension == "bmp") {
		    // read the input image
			readImage(inputFileName.c_str(), &inputData, &width, &height, &bytesPerPixel, &totalSize);
		} else if (extension == "txt") {
			// read the input text
			inputData = readText(inputFileName.c_str(), &totalSize);
		}
		// create the dictionary
		vector<int> output_code = encoding(inputData, totalSize);

		// write the compressed version of inputData to "compressed_file"
		vector<byte> encodedTextBits = encodeWithOutputCode(output_code);
		writeText(compressedFileName, encodedTextBits);

		// decode the compressed file
		vector<int> data = readEncodedLZW(compressedFileName);
		vector <byte> decodedText = decoding(data);

		// write part of the decompressed version of the "compressed_file"
		// to "decompressed_file"
		if (extension == "bmp") {
			byte *p = &*decodedText.begin();
    		writeImage(decompressedFileName.c_str(), p, width, height, bytesPerPixel);
		} else if (extension == "txt") {
			writeText(decompressedFileName, decodedText);
		}	
	}
	return 0;
}
