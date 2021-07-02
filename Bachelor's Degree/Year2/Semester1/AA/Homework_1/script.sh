#/bin/bash

# iterate through the text files in the input
# and apply both Huffman and LZW encoding/decoding

compressedHuff="compressedHuff/"
decompressedHuff="decompressedHuff/"

compressedLZW="compressedLZW/"
decompressedLZW="decompressedLZW/"

outDirectory="../out/"

cd in/
for textFile in `find . -name "*.txt" -type f`; do

	../tester Huffman $textFile $outDirectory$compressedHuff$textFile $outDirectory$decompressedHuff$textFile
 	../tester LZW $textFile $outDirectory$compressedLZW$textFile $outDirectory$decompressedLZW$textFile
done

for bmpFile in `find . -name "*.bmp" -type f`; do
       	../tester Huffman $bmpFile $outDirectory$compressedHuff$bmpFile $outDirectory$decompressedHuff$bmpFile
	../tester LZW $bmpFile $outDirectory$compressedLZW$bmpFile $outDirectory$decompressedLZW$bmpFile
done
