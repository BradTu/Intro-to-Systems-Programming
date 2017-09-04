//Brad Tully
//Homework 5: Class Encapsulation
//8 May 2017
//Class stores an array of uint8_ts that represent a voxel model

#include "BitVector.h"
#include <math.h>
#include <iostream>
#include <cmath>

//Constructor makes a bit vector of a certain number of bits
BitVector::BitVector(int numBits) {
	int size = std::ceil(numBits / 8.0);
	data = new uint8_t[size];
	numBytes = ceil(numBits / 8);
	clearBits();
}

//Destructor
BitVector::~BitVector() {
	//delete[] data;
	//data = nullptr;
}

//Turns on all of the bits in the model
void BitVector::fillBits() {
	for (int i = 0; i <= numBytes; i++) {
		data[i] = 0xFF;
	}
}

//Turns off all of the bits in the model
void BitVector::clearBits() {
	for (int i = 0; i <= numBytes; i++) {
		data[i] = 0;
	}
}

//Returns the value of said bit
bool BitVector::getBit(int bitNumber) {
	int byteNumber = floor(bitNumber / 8);
	uint8_t bitPosition = bitNumber % 8;
	return (data[byteNumber] & (uint8_t{ 1 } << bitPosition)) != 0;
}

//Changes the value of a bit to said value
void BitVector::setBit(int bitNumber, bool onOff) {
	if (onOff == true) {
		int byteNumber = floor(bitNumber / 8);
		uint8_t bitPosition = bitNumber % 8;
		data[byteNumber] |= (uint8_t{ 1 } << bitPosition);
	}
	else if (onOff == false) {
		int byteNumber = floor(bitNumber / 8);
		uint8_t bitPosition = bitNumber % 8;
		data[byteNumber] &= (uint8_t{ 0xFF } ^ (uint8_t{ 1 } << bitPosition));
	}
}

//Returns the number of bits availible in the model
int BitVector::sizeBits() {
	int size = 0;
	for (int i = 0; i < numBytes; i++) {
		size++;
	}
	size = size * 8;
	return size;
}
