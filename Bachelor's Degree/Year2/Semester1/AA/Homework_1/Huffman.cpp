#include <unordered_map>
#include <vector>
#include <bitset>
#include <queue>
#include <fstream>
#include <streambuf>

typedef unsigned char byte;

using namespace std;

struct Node
{
	char ch;
	int freq;
	Node *left, *right;
};

struct comp
{
	bool operator()(Node* l, Node* r)
	{
		// highest priority item has lowest frequency
		return l->freq > r->freq;
	}
};

Node* getNode(char ch, int freq, Node* left, Node* right)
{
	Node* node = new Node();

	node->ch = ch;
	node->freq = freq;
	node->left = left;
	node->right = right;

	return node;
}

// traverse the Huffman Tree and store Huffman Codes
// in a map.
void encodeChars(Node* root, string str,
			unordered_map<char, string> &huffmanCode)
{
	if (root == nullptr)
		return;

	// found a leaf node
	if (!root->left && !root->right) {
		huffmanCode[root->ch] = str;
	}

	encodeChars(root->left, str + "0", huffmanCode);
	encodeChars(root->right, str + "1", huffmanCode);
}

// traverse the Huffman Tree and decode the encoded string
void decode(Node* root, int &index,	vector<byte> str, vector <byte> &decodedText)
{
	if (root == nullptr) {
		return;
	}

	// found a leaf node
	if (!root->left && !root->right)
	{
		decodedText.push_back(root->ch);
		return;
	}

	index++;

	if (str[index] =='0')
		decode(root->left, index, str, decodedText);
	else
		decode(root->right, index, str, decodedText);
}

vector <byte> decodeText(vector<byte> codedText, Node *root) {
	int index = -1;
	vector<byte> decodedText;
	while (index < (int)codedText.size() - 1) {
		decode(root, index, codedText, decodedText);
	}
	return decodedText;
}

// Builds Huffman Tree and decode given input text
unordered_map<char, string> buildHuffmanTree(byte *text, Node **root, int fsize)
{
	// count frequency of appearance of each character
	// and store it in a map
	unordered_map<char, int> freq;

  	while (fsize) {
		freq[text[fsize-1]]++;
		fsize--;
	}

	// Create a priority queue to store live nodes of
	// Huffman tree;
	priority_queue<Node*, vector<Node*>, comp> pq;

	// Create a leaf node for each character and add it
	// to the priority queue.
	for (auto pair: freq) {
		pq.push(getNode(pair.first, pair.second, nullptr, nullptr));
	}

	if (pq.size() == 1) {
		// create a dummy node
		pq.push(getNode('\0', -1, nullptr, nullptr));
	}

	// do till there is more than one node in the queue
	while (pq.size() != 1)
	{
		// Remove the two nodes of highest priority
		// (lowest frequency) from the queue
		Node *left = pq.top(); pq.pop();
		Node *right = pq.top();	pq.pop();

		// Create a new internal node with these two nodes
		// as children and with frequency equal to the sum
		// of the two nodes' frequencies. Add the new node
		// to the priority queue.
		int sum = left->freq + right->freq;
		pq.push(getNode('\0', sum, left, right));
	}

	// root stores pointer to root of Huffman Tree
	(*root) = pq.top();

	// traverse the Huffman Tree and store Huffman Codes
	// in a map. Also prints them
	unordered_map<char, string> huffmanCode;
	encodeChars((*root), "", huffmanCode);

	return huffmanCode;
}

// function that returns the encoded string of text
vector<byte> encodeText(byte* text, Node **root, int fsize)
{
	unordered_map<char, string> huffmanCode =  buildHuffmanTree(text, root, fsize);

	int currentSize = 0;
	vector<byte> array;

	while (currentSize <= fsize) {

		// get the corresponding codification
		string aux  = huffmanCode[text[currentSize]];
		int auxSize = aux.size();

		// create the vector <byte>
		for (int i = 0; i < auxSize; i++) {
			if (aux[i] == '0') {
				array.push_back(48);
			} else {
				array.push_back(49);
			}
		}

		currentSize++;
	}

	return array;
}

vector<byte>toBits(vector<byte> encodedText) {
		// sa scriem pe biti
	string encodedText_(encodedText.begin(), encodedText.end());

	int noOfBytes = encodedText_.length()/8;
	int noRemainingBits = encodedText_.length()%8;

	vector <byte> result;
	result.push_back(stoi(bitset<8>(8-noRemainingBits).to_string(), nullptr, 2));

	if (noRemainingBits) {
		noOfBytes++;

		// umple cu 0-uri ce a ramas
		while (8 - noRemainingBits) {
			encodedText_ += "0";
			noRemainingBits++;
		}
	}
	// primul byte retine numarul de biti ce raman neocupati


	int i;
	for (i = 0; i < noOfBytes; i++) {
		result.push_back(stoi(encodedText_.substr(i*8, 8) , nullptr, 2));
	}

	return result;
}

vector<byte> readEncodedHuffman(string inputFileName){
	ifstream file(inputFileName, ios::binary);
	vector<byte> x((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
	vector<byte> data;

	int bitsPadding = stoi(bitset<8>(x[0]).to_string(), nullptr, 2);
	string test = "";
	unsigned int i;
	for (i = 1; i < x.size() - 1; i++) {
		test += bitset<8>(x[i]).to_string();
		for (int j = 0; j < 8; j++)
			data.push_back(test[j]);
		test = "";
	}

	if (bitsPadding != 8) {
	test = bitset<8>(x[i]).to_string().substr(0, 8-bitsPadding);

	for (int j = 0; j < 8-bitsPadding; j++)
		data.push_back(test[j]);
	} else {
		test = bitset<8>(x[i]).to_string().substr(0, 8);

		for (int j = 0; j < 8; j++)
			data.push_back(test[j]);
	}

	return data;
}