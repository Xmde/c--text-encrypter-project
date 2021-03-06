//99% of code by eric

#include "encrypt.h"
#include "structures.h"
#include <vector>
#include <iostream>

//this part is by eric

void addRoundKey(unsigned char* state, unsigned char* roundKey) {
	for (int i = 0; i < 16; i++) {
		state[i] ^= roundKey[i];
	}
}

void subBox(unsigned char* state) {
	for (int i = 0; i < 16; i++) {
		state[i] = s[state[i]];
	}
}

void inverseSubBox(unsigned char* state) {
	for (int i = 0; i < 16; i++) {
		state[i] = inv_s[state[i]];
	}
}

void shiftRows(unsigned char* state) {
	unsigned char temp[16];

	temp[0] = state[0];
	temp[1] = state[5];
	temp[2] = state[10];
	temp[3] = state[15];

	temp[4] = state[4];
	temp[5] = state[9];
	temp[6] = state[14];
	temp[7] = state[3];

	temp[8] = state[8];
	temp[9] = state[13];
	temp[10] = state[2];
	temp[11] = state[7];

	temp[12] = state[12];
	temp[13] = state[1];
	temp[14] = state[6];
	temp[15] = state[11];

	for (int i = 0; i < 16; i++) {
		state[i] = temp[i];
	}
}

void inverseShiftRows(unsigned char* state) {
	unsigned char temp[16];

	temp[0] = state[0];
	temp[1] = state[13];
	temp[2] = state[10];
	temp[3] = state[7];

	temp[4] = state[4];
	temp[5] = state[1];
	temp[6] = state[14];
	temp[7] = state[11];

	temp[8] = state[8];
	temp[9] = state[5];
	temp[10] = state[2];
	temp[11] = state[15];

	temp[12] = state[12];
	temp[13] = state[9];
	temp[14] = state[6];
	temp[15] = state[3];

	for (int i = 0; i < 16; i++) {
		state[i] = temp[i];
	}
}

void mixColumns(unsigned char* state) {

	unsigned char temp[16];

	temp[0] = (unsigned char)mul2[state[0]] ^ mul3[state[1]] ^ state[2] ^ state[3];
	temp[1] = (unsigned char)state[0] ^ mul2[state[1]] ^ mul3[state[2]] ^ state[3];
	temp[2] = (unsigned char)state[0] ^ state[1] ^ mul2[state[2]] ^ mul3[state[3]];
	temp[3] = (unsigned char)mul3[state[0]] ^ state[1] ^ state[2] ^ mul2[state[3]];

	temp[4] = (unsigned char)mul2[state[4]] ^ mul3[state[5]] ^ state[6] ^ state[7];
	temp[5] = (unsigned char)state[4] ^ mul2[state[5]] ^ mul3[state[6]] ^ state[7];
	temp[6] = (unsigned char)state[4] ^ state[5] ^ mul2[state[6]] ^ mul3[state[7]];
	temp[7] = (unsigned char)mul3[state[4]] ^ state[5] ^ state[6] ^ mul2[state[7]];

	temp[8] = (unsigned char)mul2[state[8]] ^ mul3[state[9]] ^ state[10] ^ state[11];
	temp[9] = (unsigned char)state[8] ^ mul2[state[9]] ^ mul3[state[10]] ^ state[11];
	temp[10] = (unsigned char)state[8] ^ state[9] ^ mul2[state[10]] ^ mul3[state[11]];
	temp[11] = (unsigned char)mul3[state[8]] ^ state[9] ^ state[10] ^ mul2[state[11]];

	temp[12] = (unsigned char)mul2[state[12]] ^ mul3[state[13]] ^ state[14] ^ state[15];
	temp[13] = (unsigned char)state[12] ^ mul2[state[13]] ^ mul3[state[14]] ^ state[15];
	temp[14] = (unsigned char)state[12] ^ state[13] ^ mul2[state[14]] ^ mul3[state[15]];
	temp[15] = (unsigned char)mul3[state[12]] ^ state[13] ^ state[14] ^ mul2[state[15]];

	for (int i = 0; i < 16; i++) {
		state[i] = temp[i];
	}

}

void inverseMixColumns(unsigned char* state) {
	unsigned char temp[16];

	temp[0] = (unsigned char)mul14[state[0]] ^ mul11[state[1]] ^ mul13[state[2]] ^ mul9[state[3]];
	temp[1] = (unsigned char)mul9[state[0]] ^ mul14[state[1]] ^ mul11[state[2]] ^ mul13[state[3]];
	temp[2] = (unsigned char)mul13[state[0]] ^ mul9[state[1]] ^ mul14[state[2]] ^ mul11[state[3]];
	temp[3] = (unsigned char)mul11[state[0]] ^ mul13[state[1]] ^ mul9[state[2]] ^ mul14[state[3]];

	temp[4] = (unsigned char)mul14[state[4]] ^ mul11[state[5]] ^ mul13[state[6]] ^ mul9[state[7]];
	temp[5] = (unsigned char)mul9[state[4]] ^ mul14[state[5]] ^ mul11[state[6]] ^ mul13[state[7]];
	temp[6] = (unsigned char)mul13[state[4]] ^ mul9[state[5]] ^ mul14[state[6]] ^ mul11[state[7]];
	temp[7] = (unsigned char)mul11[state[4]] ^ mul13[state[5]] ^ mul9[state[6]] ^ mul14[state[7]];

	temp[8] = (unsigned char)mul14[state[8]] ^ mul11[state[9]] ^ mul13[state[10]] ^ mul9[state[11]];
	temp[9] = (unsigned char)mul9[state[8]] ^ mul14[state[9]] ^ mul11[state[10]] ^ mul13[state[11]];
	temp[10] = (unsigned char)mul13[state[8]] ^ mul9[state[9]] ^ mul14[state[10]] ^ mul11[state[11]];
	temp[11] = (unsigned char)mul11[state[8]] ^ mul13[state[9]] ^ mul9[state[10]] ^ mul14[state[11]];

	temp[12] = (unsigned char)mul14[state[12]] ^ mul11[state[13]] ^ mul13[state[14]] ^ mul9[state[15]];
	temp[13] = (unsigned char)mul9[state[12]] ^ mul14[state[13]] ^ mul11[state[14]] ^ mul13[state[15]];
	temp[14] = (unsigned char)mul13[state[12]] ^ mul9[state[13]] ^ mul14[state[14]] ^ mul11[state[15]];
	temp[15] = (unsigned char)mul11[state[12]] ^ mul13[state[13]] ^ mul9[state[14]] ^ mul14[state[15]];

	for (int i = 0; i < 16; i++) {
		state[i] = temp[i];
	}
}

void round(unsigned char* state, unsigned char* key) {
	subBox(state);
	shiftRows(state);
	mixColumns(state);
	addRoundKey(state, key);
}

void inverseRound(unsigned char* state, unsigned char* key) {
	addRoundKey(state, key);
	inverseMixColumns(state);
	inverseShiftRows(state);
	inverseSubBox(state);
}

void finalRound(unsigned char* state, unsigned char* key) {
	subBox(state);
	shiftRows(state);
	addRoundKey(state, key);
}

void initRound(unsigned char* state, unsigned char* key) {
	addRoundKey(state, key);
	inverseShiftRows(state);
	inverseSubBox(state);
}

void AESEncryption(unsigned char* mess, unsigned char* key, unsigned char* encryptedMessage) {
	unsigned char state[16];
	unsigned char expandedKey[176];
	KeyExpantion(key, expandedKey);
	int numOfRounds = 9;

	for (int i = 0; i < 16; i++) {
		state[i] = mess[i];
	}

	addRoundKey(state, expandedKey);

	for (int i = 0; i < numOfRounds; i++) {
		round(state, expandedKey + (16 * (i + 1)));
	}

	finalRound(state, expandedKey + (16 * (numOfRounds + 1)));

	for (int i = 0; i < 16; i++) {
		encryptedMessage[i] = state[i];
	}
}

void AESDecrypt(unsigned char* encryptedMessage, unsigned char* key, unsigned char* decryptedMessage) {
	unsigned char state[16];
	unsigned char expandedKey[176];
	KeyExpantion(key, expandedKey);
	int numOfRounds = 9;

	for (int i = 0; i < 16; i++) {
		state[i] = encryptedMessage[i];
	}

	initRound(state, expandedKey + (16 * (numOfRounds + 1)));

	for (int i = numOfRounds - 1; i >= 0; i--) {
		inverseRound(state, expandedKey + (16 * (i + 1)));
	}

	addRoundKey(state, expandedKey);

	for (int i = 0; i < 16; i++) {
		decryptedMessage[i] = state[i];
	}
}

//this part is by django

using namespace std;

string enc::encrypt(string in) {
	string out = "";

	// Pad message to 16 bytes
	int originalLen = in.length();

	//adds more spacers to the input based on the length
	for (int x = 0; x <= (in.length() % 16) + 1; x++) {
		in.append("                ");
	}

		int paddedMessageLen = originalLen;

	if ((paddedMessageLen % 16) != 0) {
		paddedMessageLen = (paddedMessageLen / 16 + 1) * 16;
	}

	unsigned char* paddedMessage = new unsigned char[paddedMessageLen];
	for (int i = 0; i < paddedMessageLen; i++) {
		if (i >= originalLen) {
			paddedMessage[i] = ' ';
		}
		else {
			paddedMessage[i] = in.at(i);
		}
	}

	string debug;

	for (int x = 0; x < paddedMessageLen; x++) {
		debug.push_back(paddedMessage[x]);
	}

	cout << debug;

	unsigned char* encryptedMessage = new unsigned char[paddedMessageLen];


	unsigned char key[16];

	//hard coded key
	for (int x = 0; x < 16; x++) {
		key[x] = 15 + x;
	}

	unsigned char expandedKey[176];

	KeyExpantion(key, expandedKey);

	for (int i = 0; i < paddedMessageLen; i += 16) {
		AESEncryption(paddedMessage + i, expandedKey, encryptedMessage + i);
	}

	for (int x = 0; x < paddedMessageLen; x++) {
		int v = (int)encryptedMessage[x];
		out.append(to_string(v)+" ");
	}

	delete[] paddedMessage;
	delete[] encryptedMessage;

	return out;
}

string enc::decrypt(string in) {
	string out = "";

	vector<unsigned char> nums;

	nums.reserve(in.length() / 4);

	string tempVal = "";
	int apends = 0;
	for (int x = 0; x < in.length(); x++) {
		if ((in.at(x) == ' ') && ((x-1) != in.length())) {
			nums.push_back((unsigned char)stoi(tempVal));
			apends++;
			tempVal.clear();
		}
		else {
			tempVal.push_back(in.at(x));
		}
	}

	
	unsigned char* encryptedMessage = new unsigned char[nums.size()];
	for (int i = 0; i < nums.size(); i++) {
		encryptedMessage[i] = nums.at(i);
	}


	unsigned char key[16];

	//hard coded key
	for (int x = 0; x < 16; x++) {
		key[x] = 15 + x;
	}

	unsigned char expandedKey[176];

	KeyExpantion(key, expandedKey);

	int messageLen = strlen((const char*)encryptedMessage);

	unsigned char* decryptedMessage = new unsigned char[messageLen];

	for (int i = 0; i < messageLen; i += 16) {
		AESDecrypt(encryptedMessage + i, expandedKey, decryptedMessage + i);
	}

	for (int x = 0; x < messageLen - 14; x++) {
		out.push_back(decryptedMessage[x]);
	}

	//removes spacer spaces
	int lastChar = 0;
	for (int x = 0; x < out.length(); x++) {
		if (out.at(x) != ' ') {
			lastChar = x;
		}
	}
	out = out.substr(0, lastChar+1);

	return out;
}
