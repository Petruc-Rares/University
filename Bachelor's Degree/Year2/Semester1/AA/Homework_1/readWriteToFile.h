typedef unsigned char byte;
typedef unsigned int int32;

using namespace std;

byte* readText(const char *, unsigned int *);
void writeText(string, vector<byte>);
void readImage(const char *,byte **, int32 *, int32 *, int32 *, int32 *);
void writeImage(const char *, byte *, int32, int32, int32);