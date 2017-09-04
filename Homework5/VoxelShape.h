//Brad Tully
//Homework 5: Class Encapsulation
//8 May 2017
//Uses the BitVector to make a model

#pragma once
#include "BitVector.h"

class VoxelShape {
public:
	VoxelShape(int nx, int ny, int nz);
	~VoxelShape();
	void Clear();
	void Fill();
	bool getBit(int x, int y, int z);
	void setBit(int x, int y, int z);
	void clearBit(int x, int y, int z);
	void toggleBit(int x, int y, int z);
	void addSphere(float x, float y, float z, float r);
	void subtractSphere(float x, float y, float z, float r);
	void toggleSphere(float x, float y, float z, float r);
	int nx, ny, nz;
private:
	BitVector* bitVec;
	int getIndex(int x, int y, int z);
};

void writeSTL(VoxelShape& model, const char* filename);