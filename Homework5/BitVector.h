//Brad Tully
//Homework 5: Class Encapsulation
//8 May 2017
//Class stores an array of uint8_ts that represent a voxel model

#pragma once

#include <cstdint>

class BitVector {
public:
	BitVector(int numBits);
	~BitVector();
	void fillBits();
	void clearBits();
	bool getBit(int bitNumber);
	void setBit(int bitNumber, bool onOff);
	int sizeBits();
private:
	int numBytes;
	uint8_t* data;
};