//Brad Tully
//Homework 4: Bigger Models
//1 May 2017
//Makes a larger 3D model using voxels

#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <random>
#include <cassert>
#include "voxelModel.hpp"
#include "surfaceExtraction.hpp"

using namespace std;

int main() {
	VoxelModel* vm;
	const char* name = "shapes.stl";

	vm = &allocateModel(20, 15, 10);
	fillModel(*vm);

	//clearModel(*vm);
	//int x = 3;
	//int y = 3;
	//int z = 3;
	//cout << getIndex(*vm, x, y, z) << endl;
	//getXYZ(*vm, 62, x, y, z);
	//cout << x << " " << y << " " << z << endl;
	//cout << getByteNumber(11) << endl;
	//cout << +getBitNumber(16) << endl;
	//clearBit(*vm, 3, 3, 3);
	//setBit(*vm, 3, 3, 3);
	//toggleBit(*vm, 3, 3, 3);
	//cout << getBit(*vm, 3, 3, 3) << endl;

	writeSTL(*vm, name);
	deleteModel(*vm);

	return 0;
}
