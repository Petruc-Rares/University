build: tester.o

run_tests: build_final
	./script.sh

build_final: tester.o readWriteToFile.o Huffman.o LZW.o
	g++ tester.o readWriteToFile.o Huffman.o LZW.o -o tester

tester.o: tester.cpp
	g++ tester.cpp -c

readWriteToFile.o: readWriteToFile.cpp
	g++ readWriteToFile.cpp -c

Huffman.o: Huffman.cpp
	g++ Huffman.cpp -c

LZW.o: LZW.cpp
	g++ LZW.cpp -c

.PHONY: clean
clean:
	rm -rf *.o tester
	rm -rf out/compressedHuff/*
	rm -rf out/decompressedHuff/*
	rm -rf out/compressedLZW/*
	rm -rf out/decompressedLZW/*
