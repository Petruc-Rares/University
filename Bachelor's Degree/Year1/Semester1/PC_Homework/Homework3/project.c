// PETRUC RARES 2019 Copyright
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include "include/bmp_header.h"
#define DIM 100

FILE * readBinary(char * inputFilename) {
  FILE * in = fopen(inputFilename, "rb");
  return in;
}

FILE * writeBinary(char * outputFilename) {
  FILE * out = fopen(outputFilename, "wb");
  return out;
}

// function that returns the third byte of a number
unsigned char getRed(int RGB) {
  unsigned char firstByte = RGB >> 16;
  return firstByte;
}

// function that returns the second byte of a number
unsigned char getGreen(int RGB) {
  unsigned char secondByte = RGB >> 8;
  return secondByte;
}

// function that returns the first byte of a number
unsigned char getBlue(int RGB) {
  unsigned char firstByte = RGB;
  return firstByte;
}

// function that returns the desired byte for task 3
unsigned char newC(int ** matrixColor, int ** matrix, int size) {
  int i, j, C = 0; // C = color
  for (i = 0; i < size; i++) {
    for (j = 0; j < size; j++) {
      C += matrixColor[i][j] * matrix[i][j];
    }
  }
  if (C < 0) return 0;
  else if (C > 255) return 255;
  return C;
}

// function that builds the red bytes matrix
void makeRM(int ** m, int ** nM, int size) {
  int i, j;
  for (i = 0; i < size; i++) {
    for (j = 0; j < size; j++) {
      m[i][j] = (unsigned char) getRed(nM[i][j]);
    }
  }
  return;
}

// function that builds the yellow bytes matrix
void makeGM(int ** m, int ** nM, int size) {
  int i, j;
  for (i = 0; i < size; i++) {
    for (j = 0; j < size; j++) {
      m[i][j] = (unsigned char) getGreen(nM[i][j]);
    }
  }
  return;
}

// function that builds the blue bytes matrix
void makeBM(int ** m, int ** nM, int size) {
  int i, j;
  for (i = 0; i < size; i++) {
    for (j = 0; j < size; j++) {
      m[i][j] = (unsigned char) getBlue(nM[i][j]);
    }
  }
  return;
}

// function that allocates a square matrix of size n with int elements
int ** allocMatrix(int n) {
  int i;
  int ** nM = (int ** ) calloc(n, sizeof(int * ));
  for (i = 0; i < n; i++) {
    nM[i] = (int * ) calloc(n, sizeof(int));
  }
  return nM;
}

// function that deallocates a square matrix of size n
void deAllocMatrix(int ** test_matrix, int n) {
  int i;
  for (i = 0; i < n; i++) {
    free(test_matrix[i]);
  }
  free(test_matrix);
}

// function used for allocation of a pointer of bmp_fileheader type
bmp_fileheader * alloc_fileheader() {
  bmp_fileheader * header = (bmp_fileheader *)calloc(1,
                                                       sizeof(bmp_fileheader));
  return header;
}

// function used for allocation of a pointer of bmp_infoheader type
bmp_infoheader * alloc_infoheader() {
  bmp_infoheader * header = (bmp_infoheader *)calloc(1,
                                                       sizeof(bmp_infoheader));
  return header;
}

// function used for deallocation of a pointer of bmp_fileheader type
void deAllocateFile(bmp_fileheader * header) {
  free(header);
  return;
}

// function used for deallocation of a pointer of bmp_infoheader type
void deAllocateInfo(bmp_infoheader * header) {
  free(header);
  return;
}

// function used for reading headers fields
void readFileHeader(bmp_fileheader * header, FILE * in) {
  fread(&(header -> fileMarker1), 1, 1, in);
  fread(&(header -> fileMarker2), 1, 1, in);
  fread(&(header -> bfSize), 1, 4, in);
  fread(&(header -> unused1), 1, 2, in);
  fread(&(header -> unused2), 1, 2, in);
  fread(&(header -> imageDataOffset), 1, 4, in);
  return;
}

// function used for reading infoHeaders fields
void readInfoHeader(bmp_infoheader * infoHeader, FILE * in) {
  fread(&(infoHeader -> biSize), 1, 4, in);
  fread(&(infoHeader -> width), 1, 4, in);
  fread(&(infoHeader -> height), 1, 4, in);
  fread(&(infoHeader -> planes), 1, 2, in);
  fread(&(infoHeader -> bitPix), 1, 2, in);
  fread(&(infoHeader -> biCompression), 1, 4, in);
  fread(&(infoHeader -> biSizeImage), 1, 4, in);
  fread(&(infoHeader -> biXPelsPerMeter), 1, 4, in);
  fread(&(infoHeader -> biYPelsPerMeter), 1, 4, in);
  fread(&(infoHeader -> biClrUsed), 1, 4, in);
  fread(&(infoHeader -> biClrImportant), 1, 4, in);
  fseek(in, 0, SEEK_SET);
  return;
}

// functio used for copy of header
void copyHeader(bmp_fileheader * header, FILE * in , FILE * out) {
  signed char c;
  unsigned int k = 0;
  while (k < (header -> imageDataOffset)) {
    c = fgetc(in);
    fputc(c, out);
    k++;
  }
  return;
}

// function that allocs a matrix after the dimensions
// that are shown in infoHeader
int ** allocCopyMatrix(int paddingBytes, bmp_infoheader * infoHeader) {
  int ** test_matrix = (int ** ) calloc(infoHeader -> height, sizeof(int * ));
  for (int i = 0; i < infoHeader -> height; i++) {
    if (paddingBytes == 0) test_matrix[i] = (int * )
                                       calloc(infoHeader -> width, sizeof(int));
    // the paddingBytes might be in number of 1, 2 or 3,
    // so we will add a byte if they exist
    else
    test_matrix[i] = (int * ) calloc(infoHeader -> width + 1, sizeof(int));
  }
  if (test_matrix) return test_matrix;
  return NULL;
}

// function that checks how many bytes are required
// for each row of the photo in order
// to respect the BMP format
int getPaddingBytes(bmp_infoheader * infoHeader) {
  signed int aux = infoHeader -> width * 3;
  while (aux % 4 != 0) {
    aux++;
  }
  int paddingBytes = aux - infoHeader -> width * 3;
  return paddingBytes;
}

// function that creates a matrix of size "size" with the neighbors
// of the "matrix[i][j]" element
void neighborsMatrix(int ** photoMatrix, int i, int j, int ** matrix,
bmp_infoheader * infoHeader, int ** photoFilteredMatrix, int size) {
  int ** nM = allocMatrix(size), k, q, auxi, auxj; // nM = neighborsMatrix
  int ** rM = allocMatrix(size); // red matrix
  int ** gM = allocMatrix(size); // green matrix
  int ** bM = allocMatrix(size); // blue matrix
  unsigned char nR, nG, nB; // newRed, newGreen, newBlue
  auxi = i;
  auxj = j;
  i--;
  for (k = 0; k < size; k++) {
    i++;
    j = auxj;
    for (q = 0; q < size; q++) {
      if ((i - size / 2 < 0) || (j - size / 2 < 0) ||
          (i - size / 2 >= infoHeader -> height) ||
          (j - size / 2 >= infoHeader -> width)) nM[k][q] = 0;
      else
      nM[k][q] = photoMatrix[i - size / 2][j - size / 2];
      j++;
    }
  }
  makeRM(rM, nM, size);
  makeGM(gM, nM, size);
  makeBM(bM, nM, size);
  nR = newC(rM, matrix, size);
  nG = newC(gM, matrix, size);
  nB = newC(bM, matrix, size);
  photoFilteredMatrix[auxi][auxj] = nR * pow(2, 16) + nG * pow(2, 8) + nB;
  deAllocMatrix(nM, size);
  deAllocMatrix(rM, size);
  deAllocMatrix(gM, size);
  deAllocMatrix(bM, size);
  return;
}

// function that applies a filter for each element of the "photoMatrix"
void applyFilter(int ** photoMatrix, bmp_infoheader * infoHeader,
int ** matrix, int ** photoFilteredMatrix, int size) {
  int i, j;
  for (i = 0; i < infoHeader -> height; i++) {
    for (j = 0; j < infoHeader -> width; j++) {
      neighborsMatrix(photoMatrix, i, j, matrix,
                      infoHeader, photoFilteredMatrix, size);
    }
  }
  return;
}

// functions that turns the matrix upside down
void rotateMatrix(int ** photoMatrix, bmp_infoheader * infoHeader) {
  int temp;
  int i, j;
  for (i = 0; i < infoHeader -> height / 2; i++) {
    for (j = 0; j < infoHeader -> width; j++) {
      temp = photoMatrix[i][j];
      photoMatrix[i][j] = photoMatrix[infoHeader -> height - i - 1][j];
      photoMatrix[infoHeader -> height - i - 1][j] = temp;
    }
  }
  return;
}

// function that creates the filtered matrix
void copyMatrixCL(bmp_infoheader * infoHeader, int paddingBytes,
int ** photoMatrix, FILE * in , FILE * out,
int ** filterMatrix, int ** cpyMatrix, int size) {
  unsigned n = 0;
  signed int j = 0;
  signed int i = 0;
  while (i < infoHeader -> height) {
    j = 0;
    while (j < infoHeader -> width) {
      fread(&n, 1, 3, in);
      photoMatrix[i][j] = n;
      j++;
    }
    if (paddingBytes) {
      fseek(in, paddingBytes, SEEK_CUR);
      photoMatrix[i][infoHeader -> width] = 0;
    }
    i++;
  }
  rotateMatrix(photoMatrix, infoHeader);
  applyFilter(photoMatrix, infoHeader, filterMatrix, cpyMatrix, size);
  for (i = infoHeader -> height - 1; i >= 0; i--) {
    for (j = 0; j < infoHeader -> width; j++)
      fwrite(&(cpyMatrix[i][j]), 1, 3, out);
    if (paddingBytes) fwrite(&(cpyMatrix[i][infoHeader -> width]),
                             1, paddingBytes, out);
  }
}

// function that deallocates a matrix with sizes specified in infoHeader
void freeCopyMatrix(bmp_infoheader * infoHeader, int ** test_matrix) {
  int i;
  for (i = 0; i < infoHeader -> height; i++) {
    free(test_matrix[i]);
  }
  free(test_matrix);
  return;
}

// function that returns the minimum color in the "matrixColor"
unsigned char newMinC(int ** matrixColor, int size) {
  int i, j, C = 255; // C = color
  for (i = 0; i < size; i++) {
    for (j = 0; j < size; j++) {
      if (matrixColor[i][j] < C) {
        C = matrixColor[i][j];
      }
    }
  }
  return C;
}

// function that returns the maximum byte in the "matrixColor"
unsigned char newMaxC(int ** matrixColor, int size) {
  int i, j, C = 0; // C = color
  for (i = 0; i < size; i++) {
    for (j = 0; j < size; j++) {
      if (matrixColor[i][j] > C) {
        C = matrixColor[i][j];
      }
    }
  }
  return C;
}

// function that creates the neighbors of photoMatrix[i][j] of size
// "size" and in which desired bytes are switched
// case letter = m
void neighborsMatrixMin(int ** photoMatrix, int i, int j,
bmp_infoheader * infoHeader, int ** photoPooledMatrix, int size) {
  // nM = neighborsMatrix
  int ** nM = allocMatrix(size), k, q, auxi, auxj;
  int ** rM = allocMatrix(size); // red matrix
  int ** gM = allocMatrix(size); // green matrix
  int ** bM = allocMatrix(size); // blue matrix
  unsigned char nR, nG, nB; // newRed, newGreen, newBlue
  auxi = i;
  auxj = j;
  i--;
  for (k = 0; k < size; k++) {
    i++;
    j = auxj;
    for (q = 0; q < size; q++) {
      if ((i - size / 2 < 0) || (j - size / 2 < 0) ||
          (i - size / 2 >= infoHeader -> height) ||
          (j - size / 2 >= infoHeader -> width)) nM[k][q] = 0;
      else
      nM[k][q] = photoMatrix[i - size / 2][j - size / 2];
      j++;
    }
  }
  makeRM(rM, nM, size);
  makeGM(gM, nM, size);
  makeBM(bM, nM, size);
  nR = newMinC(rM, size);
  nG = newMinC(gM, size);
  nB = newMinC(bM, size);
  photoPooledMatrix[auxi][auxj] = nR * pow(2, 16) + nG * pow(2, 8) + nB;
  deAllocMatrix(nM, size);
  deAllocMatrix(rM, size);
  deAllocMatrix(gM, size);
  deAllocMatrix(bM, size);
  return;
}

// function that creates the neighbors of photoMatrix[i][j] of size "size"
// and in which desired bytes are switched
// case letter = M
void neighborsMatrixMax(int ** photoMatrix, int i, int j,
            bmp_infoheader * infoHeader, int ** photoPooledMatrix, int size) {
  // nM = neighborsMatrix
  int ** nM = allocMatrix(size), k, q, auxi, auxj;
  int ** rM = allocMatrix(size); // red matrix
  int ** gM = allocMatrix(size); // green matrix
  int ** bM = allocMatrix(size); // blue matrix
  unsigned char nR, nG, nB; // newRed, newGreen, newBlue
  auxi = i;
  auxj = j;
  i--;
  for (k = 0; k < size; k++) {
    i++;
    j = auxj;
    for (q = 0; q < size; q++) {
      if ((i - size / 2 < 0) || (j - size / 2 < 0) ||
          (i - size / 2 >= infoHeader -> height) ||
          (j - size / 2 >= infoHeader -> width)) nM[k][q] = 0;
      else
      nM[k][q] = photoMatrix[i - size / 2][j - size / 2];
      j++;
    }
  }
  makeRM(rM, nM, size);
  makeGM(gM, nM, size);
  makeBM(bM, nM, size);
  nR = newMaxC(rM, size);
  nG = newMaxC(gM, size);
  nB = newMaxC(bM, size);
  photoPooledMatrix[auxi][auxj] = nR * pow(2, 16) + nG * pow(2, 8) + nB;
  deAllocMatrix(nM, size);
  deAllocMatrix(rM, size);
  deAllocMatrix(gM, size);
  deAllocMatrix(bM, size);
  return;
}

void applyPooling(int ** photoMatrix, bmp_infoheader * infoHeader, char letter,
                  int ** photoPooledMatrix, int size) {
  int i, j;
  if (letter == 'm') {
    for (i = 0; i < infoHeader -> height; i++) {
      for (j = 0; j < infoHeader -> width; j++) {
        neighborsMatrixMin(photoMatrix, i, j, infoHeader,
                           photoPooledMatrix, size);
      }
    }
  } else {
    for (i = 0; i < infoHeader -> height; i++) {
      for (j = 0; j < infoHeader -> width; j++) {
        neighborsMatrixMax(photoMatrix, i, j, infoHeader,
                           photoPooledMatrix, size);
      }
    }
  }
  return;
}

void copyPooledMatrix(bmp_infoheader * infoHeader, int paddingBytes,
int ** photoMatrix, FILE * in , FILE * out, char letter,
int ** cpy_matrix, int size) {
  unsigned n = 0;
  signed int j = 0;
  signed int i = 0;
  while (i < infoHeader -> height) {
    j = 0;
    while (j < infoHeader -> width) {
      fread(&n, 1, 3, in);
      photoMatrix[i][j] = n;
      j++;
    }
    if (paddingBytes) {
      fseek(in , paddingBytes, SEEK_CUR);
      photoMatrix[i][infoHeader -> width] = 0;
    }
    i++;
  }
  rotateMatrix(photoMatrix, infoHeader);
  applyPooling(photoMatrix, infoHeader, letter, cpy_matrix, size);
  for (i = infoHeader -> height - 1; i >= 0; i--) {
    for (j = 0; j < infoHeader -> width; j++)
      fwrite(&(cpy_matrix[i][j]), 1, 3, out);
    if (paddingBytes) fwrite(&(cpy_matrix[i][infoHeader -> width]), 1,
                             paddingBytes, out);
  }
}

void makeItCL(bmp_fileheader * header, bmp_infoheader * infoHeader,
              FILE * in , FILE * out, FILE * filter) {
  int size, i, j;
  fscanf(filter, "%d", & size);
  int ** filterMatrix = allocMatrix(size);
  for (i = 0; i < size; i++) {
    for (j = 0; j < size; j++) {
      fscanf(filter, "%d", & filterMatrix[i][j]);
    }
  }
  copyHeader(header, in , out);
  int paddingBytes = getPaddingBytes(infoHeader);
  int ** photoMatrix = allocCopyMatrix(paddingBytes, infoHeader);
  int ** cpyMatrix = allocCopyMatrix(paddingBytes, infoHeader);
  copyMatrixCL(infoHeader, paddingBytes, photoMatrix, in , out, filterMatrix,
               cpyMatrix, size); // task 3!
  freeCopyMatrix(infoHeader, photoMatrix);
  freeCopyMatrix(infoHeader, cpyMatrix);
  deAllocMatrix(filterMatrix, size);
  fclose(out);
  fseek(in , 0, SEEK_SET);
  fclose(filter);
  return;
}

// function that applies pooling filter to a photo
void makeItPooling(bmp_fileheader * header, bmp_infoheader * infoHeader,
                   FILE * in , FILE * out, FILE * pooling) {
  char letter;
  int size;
  letter = fgetc(pooling);
  fscanf(pooling, "%d", & size);
  copyHeader(header, in , out);
  int paddingBytes = getPaddingBytes(infoHeader);
  int ** photoMatrix = allocCopyMatrix(paddingBytes, infoHeader);
  int ** photoPooledMatrix = allocCopyMatrix(paddingBytes, infoHeader);
  copyPooledMatrix(infoHeader, paddingBytes, photoMatrix, in , out, letter,
                   photoPooledMatrix, size); // task 4!
  freeCopyMatrix(infoHeader, photoMatrix);
  freeCopyMatrix(infoHeader, photoPooledMatrix);
  fclose(out);
  fseek(in , 0, SEEK_SET);
  fclose(pooling);
  return;
}

// here the Bitmap of the black & white photo is created
void copyBWMatrix(bmp_infoheader * infoHeader, int paddingBytes,
                  int ** photoMatrix, FILE * in , FILE * out) {
  unsigned char R, G, B, X;
  signed int j = 0;
  signed int i = 0;
  while (i < infoHeader -> height) {
    j = 0;
    while (j < infoHeader -> width) {
      fread(&R, 1, 1, in);
      fread(&G, 1, 1, in);
      fread(&B, 1, 1, in);
      X = (R + G + B) / 3;
      // 3 bytes created identically
      photoMatrix[i][j] = X + X * pow(2, 8) + X * pow(2, 16);
      j++;
    }
    if (paddingBytes) {
      fseek(in, paddingBytes, SEEK_CUR); // get over the padding bytes;
      photoMatrix[i][infoHeader -> width] = 0;
    }
    i++;
  }
  for (i = 0; i < infoHeader -> height; i++) {
    for (j = 0; j < infoHeader -> width; j++) {
      fwrite(&(photoMatrix[i][j]), 1, 3, out);
    }
    if (paddingBytes) fwrite(&(photoMatrix[i][infoHeader -> width]), 1,
                             paddingBytes, out);
  }
}

// function that makes a photo black & white
void makeItBlackWhite(bmp_fileheader * header, bmp_infoheader * infoHeader,
                      FILE * in , FILE * out) {
  copyHeader(header, in , out);
  int paddingBytes = getPaddingBytes(infoHeader);
  int ** photoMatrix = allocCopyMatrix(paddingBytes, infoHeader);
  copyBWMatrix(infoHeader, paddingBytes, photoMatrix, in , out);
  freeCopyMatrix(infoHeader, photoMatrix);
  fclose(out);
  fseek(in , 0, SEEK_SET);
  return;
}

// functie that determines the minimum between two sizes of a matrix
signed int minSize(int height, int width) {
  signed int min;
  if (height > width) {
    min = width;
  } else {
    min = height;
  }
  return min;
}

// function that determins in which case are we (width </>/= height)
int getSize(int height, int width, int sizeBefore) {
  if (sizeBefore < width) {
    return 2; // width > height
  } else if (sizeBefore < height) {
    return 1; // height > width
  } else {
    return 0; // height = width
  }
}

// modify the Header after the case we are in
void modifyHeader(bmp_fileheader * header, bmp_infoheader * infoHeader,
                  FILE * in , FILE * out, signed int sizeBefore) {
  signed char c;
  unsigned int k = 0;
  if (getSize(infoHeader -> height, infoHeader -> width, sizeBefore) == 0) {
    while (k < (header -> imageDataOffset)) {
      c = fgetc(in);
      fputc(c, out);
      k++;
    }
  } else if (getSize(infoHeader -> height, infoHeader -> width,
                     sizeBefore) == 1) {
    while (k < 18) { // 18 bytes until width
      c = fgetc(in);
      fputc(c, out);
      k++;
    }
    fwrite(&(infoHeader -> height), 1, 4, out);
    fseek(in, 4, SEEK_CUR); // get over width's bytes
    k += 4;
    while (k < (header -> imageDataOffset)) {
      c = fgetc(in);
      fputc(c, out);
      k++;
    }
  } else if (getSize(infoHeader -> height, infoHeader -> width,
             sizeBefore) == 2) {
    while (k < 22) { // 22 bytes until height
      c = fgetc(in);
      fputc(c, out);
      k++;
    }
    fwrite(&(infoHeader -> width), 1, 4, out);
    fseek(in, 4, SEEK_CUR); // get over height's bytes
    k += 4;
    while (k < (header -> imageDataOffset)) {
      c = fgetc(in);
      fputc(c, out);
      k++;
    }
  }
  return;
}

// w > h case
void copyNcMatrix(bmp_infoheader * infoHeader, int paddingBytes,
int ** photoMatrix, FILE * in , FILE * out, signed int sizeBefore) {
  unsigned n = 0;
  signed int j = 0;
  signed int i = 0;
  signed int dif = infoHeader -> width - sizeBefore, dif2;
  if (dif % 2 == 0) {
    dif2 = dif / 2;
  } else {
    dif2 = dif / 2 + 1;
  }
  while (i < dif2) {
    j = 0;
    while (j < infoHeader -> width) {
      photoMatrix[i][j] = 16777215;
      j++;
    }
    if (paddingBytes) {
      photoMatrix[i][infoHeader -> width] = 0;
    }
    i++;
  }
  while (i < (sizeBefore + dif2)) {
    j = 0;
    while (j < infoHeader -> width) {
      fread(&n, 1, 3, in);
      photoMatrix[i][j] = n;
      j++;
    }
    if (paddingBytes) {
      fseek(in, paddingBytes, SEEK_CUR);
      photoMatrix[i][infoHeader -> width] = 0;
    }
    i++;
  }
  while (i < infoHeader -> height) {
    j = 0;
    while (j < infoHeader -> width) {
      photoMatrix[i][j] = 16777215;
      j++;
    }
    if (paddingBytes) {
      photoMatrix[i][infoHeader -> width] = 0;
    }
    i++;
  }
  for (i = 0; i < infoHeader -> height; i++) {
    for (j = 0; j < infoHeader -> width; j++)
      fwrite(&(photoMatrix[i][j]), 1, 3, out);
    if (paddingBytes) fwrite(&(photoMatrix[i][infoHeader -> width]), 1,
                             paddingBytes, out);
  }
}

// h > w case
void copyNc1Matrix(bmp_infoheader * infoHeader, int ** photoMatrix, FILE * in ,
                   FILE * out, signed int sizeBefore) {
  unsigned n = 0;
  signed int j = 0, i = 0, dif = infoHeader -> width - sizeBefore, dif2;
  if (dif % 2 == 0) {
    dif2 = dif / 2;
  } else {
    dif2 = dif / 2 + 1;
  }
  while (j < dif2) {
    i = 0;
    while (i < infoHeader -> width) {
      photoMatrix[j][i] = 16777215;
      i++;
    }
    j++;
  }
  i = 0;
  while (i < infoHeader -> width) {
    j = dif2;
    while (j < (sizeBefore + dif2)) {
      fread(&n, 1, 3, in);
      photoMatrix[j][i] = n;
      j++;
    }
    i++;
  }
  while (j < infoHeader -> height) {
    i = 0;
    while (i < infoHeader -> width) {
      photoMatrix[j][i] = 16777215;
      i++;
    }
    j++;
  }
  for (j = 0; j < infoHeader -> height; j++) {
    for (i = 0; i < infoHeader -> width; i++)
      fwrite(&(photoMatrix[i][j]), 1, 3, out);
  }
  return;
}

// function that copies the photoMatrix
void copyMatrix(bmp_infoheader * infoHeader, int paddingBytes,
               int ** photoMatrix, FILE * in , FILE * out) {
  unsigned n = 0;
  signed int j = 0;
  signed int i = 0;
  while (i < infoHeader -> height) {
    j = 0;
    while (j < infoHeader -> width) {
      fread(&n, 1, 3, in);
      photoMatrix[i][j] = n;
      j++;
    }
    if (paddingBytes) {
      fseek(in, paddingBytes, SEEK_CUR);
      photoMatrix[i][infoHeader -> width] = 0;
    }
    i++;
  }
  for (i = 0; i < infoHeader -> height; i++) {
    for (j = 0; j < infoHeader -> width; j++)
      fwrite(&(photoMatrix[i][j]), 1, 3, out);
    if (paddingBytes) fwrite( & (photoMatrix[i][infoHeader -> width]), 1,
                             paddingBytes, out);
  }
}

void makeItNoCrop(bmp_fileheader * header, bmp_infoheader * infoHeader,
                 FILE * in , FILE * out) {
  signed int sizeBefore = minSize(infoHeader -> height, infoHeader -> width);
  modifyHeader(header, infoHeader, in , out, sizeBefore);
  int paddingBytes = getPaddingBytes(infoHeader);
  if (getSize(infoHeader -> height, infoHeader -> width, sizeBefore) == 2) {
    infoHeader -> height = infoHeader -> width;
    int ** photoMatrix = allocCopyMatrix(paddingBytes, infoHeader);
    copyNcMatrix(infoHeader, paddingBytes, photoMatrix, in , out, sizeBefore);
    freeCopyMatrix(infoHeader, photoMatrix);
  } else if (getSize(infoHeader -> height, infoHeader -> width,
             sizeBefore) == 1) {
    infoHeader -> width = infoHeader -> height;
    int ** photoMatrix = allocCopyMatrix(paddingBytes, infoHeader);
    copyNc1Matrix(infoHeader, photoMatrix, in , out, sizeBefore);
    freeCopyMatrix(infoHeader, photoMatrix);
  } else if (getSize(infoHeader -> height, infoHeader -> width,
                     sizeBefore) == 0) {
    int ** photoMatrix = allocCopyMatrix(paddingBytes, infoHeader);
    copyMatrix(infoHeader, paddingBytes, photoMatrix, in , out);
    freeCopyMatrix(infoHeader, photoMatrix);
  }
  fclose(in);
  fclose(out);
  deAllocateFile(header);
  deAllocateInfo(infoHeader);
  return;
}

int checkThreshold(int a, int b, int threshold) {
  unsigned char R, G, B, R1, G1, B1;
  int S;
  R = getRed(a);
  G = getGreen(a);
  B = getBlue(a);
  R1 = getRed(b);
  G1 = getGreen(b);
  B1 = getBlue(b);
  S = abs(R - R1) + abs(G - G1) + abs(B - B1);
  if (S <= threshold) return 1;
  return 0;
}

int createArea(int i, int j, int ** indexMatrix, int ** a, int index,
int threshold, int height, int width, int auxi, int auxj, int * sumR,
int * sumG, int * sumB) {
  // check neighbor above
  int k = 0;
  if (i - 1 >= 0) {
    if ((indexMatrix[i - 1][j] == 0) && (checkThreshold(a[i - 1][j],
                                         a[auxi][auxj], threshold))) {
      indexMatrix[i - 1][j] = index;
      * sumR += getRed(a[i - 1][j]);
      * sumG += getGreen(a[i - 1][j]);
      * sumB += getBlue(a[i - 1][j]);
      k++;
    }
  }
  // check neighbor at right
  if (j + 1 < width) {
    if ((indexMatrix[i][j + 1] == 0) && (checkThreshold(a[i][j + 1],
                                         a[auxi][auxj], threshold))) {
      indexMatrix[i][j + 1] = index;
      * sumR += getRed(a[i][j + 1]);
      * sumG += getGreen(a[i][j + 1]);
      * sumB += getBlue(a[i][j + 1]);
      k++;
    }
  }
  // check neighbor beneath
  if (i + 1 < height) {
    if ((indexMatrix[i + 1][j] == 0) && (checkThreshold(a[i + 1][j],
                                         a[auxi][auxj], threshold))) {
      indexMatrix[i + 1][j] = index;
      * sumR += getRed(a[i + 1][j]);
      * sumG += getGreen(a[i + 1][j]);
      * sumB += getBlue(a[i + 1][j]);
      k++;
    }
  }
  // check neighbor at left
  if (j - 1 >= 0) {
    if ((indexMatrix[i][j - 1] == 0) && (checkThreshold(a[i][j - 1],
                                        a[auxi][auxj], threshold))) {
      indexMatrix[i][j - 1] = index;
      * sumR += getRed(a[i][j - 1]);
      * sumG += getGreen(a[i][j - 1]);
      * sumB += getBlue(a[i][j - 1]);
      k++;
    }
  }
  return k;
}

void applyCluster(int ** a, int ** indexMatrix, int threshold, int width,
                  int height) {
  if (!threshold) return;
  int p, q, i = 0, index = 1, nb = 0, j, sumR = 0, sumG = 0, sumB = 0,
      elements = 1, elements1 = 0, auxi, auxj;
  unsigned char R, G, B;
  FindNextRGB: {}
  for (i = 0; i < height; i++) {
    for (j = 0; j < width; j++) {
      if (indexMatrix[i][j] == 0) {
        sumR = 0;
        sumG = 0;
        sumB = 0;
        indexMatrix[i][j] = index;
        sumR += getRed(a[i][j]);
        sumG += getGreen(a[i][j]);
        sumB += getBlue(a[i][j]);
        auxi = i;
        auxj = j;
        elements = 1;
        while (1) {
          elements1 = 0;
          for (p = 0; p < height; p++) {
            for (q = 0; q < width; q++) {
              if (indexMatrix[p][q] == index) {
                nb = createArea(p, q, indexMatrix, a, index, threshold, height,
                                    width, auxi, auxj, & sumR, & sumG, & sumB);
                elements1 += nb;
              }
            }
          }
          if (elements1) {
            elements += elements1;
          } else {
            R = sumR / elements;
            G = sumG / elements;
            B = sumB / elements;
            for (i = 0; i < height; i++) {
              for (j = 0; j < width; j++) {
                if (indexMatrix[i][j] == index) {
                  a[i][j] = R * pow(2, 16) + G * pow(2, 8) + B;
                }
              }
            }
            index++;
            goto FindNextRGB;
          }
        }
      }
    }
  }
}

void copyClusteredMatrix(bmp_infoheader * infoHeader, int paddingBytes,
int ** test_matrix, int ** indexMatrix, FILE * in , FILE * out, int threshold) {
  unsigned n = 0;
  signed int j = 0;
  signed int i = 0;
  while (i < infoHeader -> height) {
    j = 0;
    while (j < infoHeader -> width) {
      fread(&n, 1, 3, in);
      test_matrix[i][j] = n;
      j++;
    }
    if (paddingBytes) {
      fseek(in , paddingBytes, SEEK_CUR);
      test_matrix[i][infoHeader -> width] = 0;
    }
    i++;
  }
  rotateMatrix(test_matrix, infoHeader);
  applyCluster(test_matrix, indexMatrix, threshold, infoHeader -> width,
                                                    infoHeader -> height);
  for (i = infoHeader -> height - 1; i >= 0; i--) {
    for (j = 0; j < infoHeader -> width; j++)
      fwrite(&(test_matrix[i][j]), 1, 3, out);
    if (paddingBytes) fwrite( & (test_matrix[i][infoHeader -> width]), 1,
                                                       paddingBytes, out);
  }
}

void makeItCluster(bmp_fileheader * header, bmp_infoheader * infoHeader,
                   FILE * in , FILE * out, FILE * clustering) {
  int threshold;
  fscanf(clustering, "%d", & threshold);
  copyHeader(header, in , out);
  int paddingBytes = getPaddingBytes(infoHeader);
  int ** photoMatrix = allocCopyMatrix(paddingBytes, infoHeader);
  int ** indexMatrix = allocCopyMatrix(paddingBytes, infoHeader);
  copyClusteredMatrix(infoHeader, paddingBytes, photoMatrix,
                      indexMatrix, in , out, threshold); // task 5!
  freeCopyMatrix(infoHeader, photoMatrix);
  freeCopyMatrix(infoHeader, indexMatrix);
  fclose(out);
  fseek(in , 0, SEEK_SET);
  fclose(clustering);
  return;
}

int main() {
  FILE * input, * clustering, * in , * pooling, * filter,
                   * out1, * out2, * out3, * out4, * out5;
  input = fopen("input.txt", "r");
  char * image = (char * ) calloc(DIM, sizeof(char));
  char * input_filename = (char * ) calloc(DIM, sizeof(char));
  char * bw_filename = (char * ) calloc(DIM, sizeof(char));
  char * nc_filename = (char * ) calloc(DIM, sizeof(char));
  char * filterPath = (char * ) calloc(DIM, sizeof(char));
  char * cl_filename = (char * ) calloc(DIM, sizeof(char));
  char * poolingPath = (char * ) calloc(DIM, sizeof(char));
  char * pl_filename = (char * ) calloc(DIM, sizeof(char));
  char * cls_filename = (char * ) calloc(DIM, sizeof(char));
  char * clusteringPath = (char * ) calloc(DIM, sizeof(char));
  fgets(image, 10, input);
  fseek(input, 1, SEEK_CUR);
  fgets(filterPath, 26, input);
  fseek(input, 1, SEEK_CUR);
  fgets(poolingPath, 27, input);
  fseek(input, 1, SEEK_CUR);
  fgets(clusteringPath, 30, input);
  fclose(input);
  strcpy(input_filename, image);
  strncpy(bw_filename, image, 5);
  strcat(bw_filename, "_black_white.bmp");
  strncpy(nc_filename, image, 5);
  strcat(nc_filename, "_nocrop.bmp");
  strncpy(cl_filename, image, 5);
  strcat(cl_filename, "_filter.bmp");
  strncpy(pl_filename, image, 5);
  strcat(pl_filename, "_pooling.bmp");
  strncpy(cls_filename, image, 5);
  strcat(cls_filename, "_clustered.bmp");
  if (readBinary(filterPath)) filter = readBinary(filterPath);
  else
  return -1;
  if (readBinary(poolingPath)) pooling = readBinary(poolingPath);
  else
  return -1;
  if (readBinary(clusteringPath)) clustering = readBinary(clusteringPath);
  else
  return -1;
  if (readBinary(input_filename)) in = readBinary(input_filename);
  else
  return -1;
  if (writeBinary(bw_filename)) out1 = writeBinary(bw_filename);
  else
  return -1;
  if (writeBinary(nc_filename)) out2 = writeBinary(nc_filename);
  else
  return -1;
  if (writeBinary(cl_filename)) out3 = writeBinary(cl_filename);
  else
  return -1;
  if (writeBinary(pl_filename)) out4 = writeBinary(pl_filename);
  else
  return -1;
  if (writeBinary(cls_filename)) out5 = writeBinary(cls_filename);
  else
  return -1;
  bmp_fileheader * header = alloc_fileheader(); // this is the file header
  readFileHeader(header, in);
  bmp_infoheader * infoHeader = alloc_infoheader(); // this is the info header
  readInfoHeader(infoHeader, in);
  makeItCluster(header, infoHeader, in , out5, clustering); // Task 5
  makeItPooling(header, infoHeader, in , out4, pooling); // Task 4
  makeItCL(header, infoHeader, in , out3, filter); // Task 3
  makeItBlackWhite(header, infoHeader, in , out1); // Task 1
  makeItNoCrop(header, infoHeader, in , out2); // Task 2
  free(image);
  free(input_filename);
  free(bw_filename);
  free(nc_filename);
  free(filterPath);
  free(cl_filename);
  free(poolingPath);
  free(pl_filename);
  free(cls_filename);
  free(clusteringPath);
  return 0;
}
