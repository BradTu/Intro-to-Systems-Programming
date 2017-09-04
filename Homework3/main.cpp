//Made by Brad Tully
//Homework3
//17 April 2017
//This program creates a 64 bit (4*4*4) 3D voxel shape
//Writes an STL file that represents a voxel shape

#define _CRT_SECURE_NO_WARNINGS
#include "VoxelModel.h"
#include <cstdint>
#include <cstdlib>
#include <iostream>

using namespace std;

int main() {
	//Make a voxel model object and write it to an stl file
	VoxelModel vm;
	vm.writeSTL(0x123456789F, "Output.stl");

	return 0;
}