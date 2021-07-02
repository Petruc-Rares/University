#include <vector>
#include <string>
#include <unordered_map>
#include <iostream>
#include <bitset>
#include <fstream>

typedef unsigned char byte;

using namespace std;

vector<int> encoding(byte* s1, int s1Length) 
{  
	unordered_map<string, int> table; 
	for (int i = 0; i <= 255; i++) { 
		string ch = ""; 
		ch += char(i); 
		table[ch] = i; 
	} 

	string p = "", c = ""; 
	p += s1[0]; 
	int code = 256; 
	vector<int> output_code; 
	for (int i = 0; i < s1Length; i++) { 
		if (i != s1Length - 1) 
			c += s1[i + 1]; 
		if (table.find(p + c) != table.end()) { 
			p = p + c; 
		} 
		else {
			output_code.push_back(table[p]); 
			table[p + c] = code; 
			code++; 
			p = c; 
		} 
		c = ""; 
	}
	output_code.push_back(table[p]);
	if (table.size() > 4095) {
		cout << "File can't be compressed\n";
		cout << "as encodings exceed 12bits representation.\n";
		cout << "Encode a smaller image/text/file\n";
		exit(1);
	}

	return output_code; 
} 

vector <byte> decoding(vector<int> op) 
{ 
	string decodedText = "";
	unordered_map<int, string> table; 
	for (int i = 0; i <= 255; i++) { 
		string ch = ""; 
		ch += char(i); 
		table[i] = ch; 
	} 
	int old = op[0];
	int n; 
	string s = table[old]; 
	string c = ""; 
	c += s[0]; 
	decodedText += s;
	int count = 256; 
	for (int i = 0; i < op.size() - 1; i++) { 
		n = op[i + 1]; 
		if (table.find(n) == table.end()) { 
			s = table[old]; 
			s = s + c; 
		} 
		else { 
			s = table[n]; 
		}
		decodedText += s;
		c = ""; 
		c += s[0]; 
		table[count] = table[old] + c; 
		count++; 
		old = n; 
	}

	vector <byte> array;
	for (int i = 0; i < decodedText.size(); i++) {
		array.push_back(decodedText[i]);
	} 
	return array;
} 

vector<byte> encodeWithOutputCode(vector<int> output_code) {
	string encodedText= "";
	for (int i = 0; i < output_code.size(); i++) {
		encodedText += bitset<12>(output_code[i]).to_string();
	}

	int noOfBytes = encodedText.length()/8;
	int noRemainingBits = encodedText.length()%8;

	vector <byte> result;
	result.push_back(stoi(bitset<8>(8-noRemainingBits).to_string(), nullptr, 2));

	if (noRemainingBits) {
		noOfBytes++;

		// umple cu 0-uri ce a ramas
		while (8 - noRemainingBits) {
			encodedText += "0";
			noRemainingBits++;
		}
	}
	// primul byte retine numarul de biti ce raman neocupati


	int i;
	for (i = 0; i < noOfBytes; i++) {
		result.push_back(stoi(encodedText.substr(i*8, 8) , nullptr, 2));
	}

	return result;
}

vector<int> readEncodedLZW(string compressedFileName) {
	ifstream file(compressedFileName.c_str(), ios::binary);
	vector<byte> x((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
	vector<int> data;

	int bitsPadding = stoi(bitset<8>(x[0]).to_string(), nullptr, 2);

	string test = "";

	int noGrupulete = (x.size() - 1) * 2 / 3;
	byte flag = 0;

	int contor = 0;
	int i;
	for (i = 1; contor != noGrupulete - 1; i++) {
		if (flag == 0) {
			test += bitset<8>(x[i]).to_string();
			test += (bitset<8>(x[i+1])).to_string().substr(0, 4);
		} else {
			test += (bitset<8>(x[i])).to_string().substr(4, 8);
			test +=  bitset<8>(x[i+1]).to_string();
			i++;	
		}

		data.push_back(stoi(test, nullptr, 2));

		test = "";
		
		contor++;

		flag++;
		flag %= 2;
	}

	// daca sunt pusi bits de padding citeste ultimii 8 ramas
	if (bitsPadding == 8) {
		test += (bitset<8>(x[i])).to_string().substr(4, 8);
		test +=  bitset<8>(x[i+1]).to_string();
	} else {
		test += (bitset<8>(x[i])).to_string();
		test += (bitset<8>(x[i+1]).to_string()).substr(0, 4);
	}
	data.push_back(stoi(test, nullptr, 2));

	return data;
}