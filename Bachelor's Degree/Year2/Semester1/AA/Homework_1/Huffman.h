struct Node
{
	char ch;
	int freq;
	Node *left, *right;
};

// Comparison object to be used to order the heap
struct comp
{
	bool operator()(Node* l, Node* r)
	{
		// highest priority item has lowest frequency
		return l->freq > r->freq;
	}
};

Node* getNode(char, int, Node *, Node *);
void decode(Node *, int&, vector<byte>, vector <byte> &);
vector <byte> decodeText(vector<byte>, Node *);
unordered_map<char, string> buildHuffmanTree(byte *, Node **, int);
vector<byte> encodeText(byte *, Node **, int );
vector<byte>toBits(vector<byte>);
vector<byte> readEncodedHuffman(string);