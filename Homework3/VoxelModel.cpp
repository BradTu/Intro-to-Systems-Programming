//Made by Brad Tully
//Homework3
//17 April 2017
//This program creates a 64 bit (4*4*4) 3D voxel shape
//This file contains the functions for the VoxelModel class
//Writes an STL file that represents a voxel shape

#define _CRT_SECURE_NO_WARNINGS
#include "VoxelModel.h"
#include "surfaceExtraction.hpp"
#include <cstdint>
#include <iostream>
#include <cmath>
#include <assert.h>
using namespace std;

//No argument constructor
VoxelModel::VoxelModel() {

}

//Constructor that takes the model input
VoxelModel::VoxelModel(uint64_t model) {
	theModel = model;
}

//Takes inputs 0-3, returns the bit that represents the voxel, output [0, 63]
uint8_t VoxelModel::getIndex(int x, int y, int z) {
	//Calculate which bit x,y,z is
	theIndex = (x * 16) + (y * 4) + (z);
	return theIndex;
}

//Inverse of getIndex returns x bit
uint8_t VoxelModel::getX(uint8_t index) {
	//Make sure there is an x value
	if (index < 16) {
		theX = 0;
	}
	//If there is an x value calculate it
	if (index >= 16) {
		theX = floor(index / 16);
	}
	return theX;
}

//Inverse of getIndex returns y bit
uint8_t VoxelModel::getY(uint8_t index) {
	//Find the index after finding the x value
	int remainingValue;
	remainingValue = index - (getX(index) * 16);

	//Find out if there is a y value
	if (remainingValue < 4) {
		theY = 0;
	}
	//If there's a y value calculate it
	if (remainingValue >= 4) {
		theY = floor(remainingValue / 4);
	}

	return theY;
}

//Inverse of getIndex returns z bit
uint8_t VoxelModel::getZ(uint8_t index) {
	//Calculate the x and y values so the z value can be found
	int remainingValue;
	remainingValue = index - (getX(index) * 16) - (getY(index) * 4);

	//Check if a z value exists
	if (remainingValue < 1) {
		theZ = 0;
	}
	//Calculate the z value
	if (remainingValue >= 1) {
		theZ = remainingValue;
	}

	return theZ;
}

//Returns a model with all bits turned off
uint64_t VoxelModel::emptyModel() {
	//make an empty model by setting theModel to uint64_t minimum value
	theModel = 0;
	return theModel;
}

//Returns a model with all bits turned on
uint64_t VoxelModel::fullModel() {
	//make a full model by setting theModel to uint64_t maximum value
	theModel = 0xFFFFFFFFFFFFFFFF;
	return theModel;
}

//Returns true if the bit at x,y,z is 1 and false if it's 0
bool VoxelModel::getBit(uint64_t model, int x, int y, int z) {
	//Written by Ben Jones
	return (model & (uint64_t{ 1 } << getIndex(x, y, z))) != 0;
}

//Sets the bit at x,y,z to 1
uint64_t VoxelModel::setBit(uint64_t model, int x, int y, int z) {
	//Set model to theModel
	theModel = model;

	//Calculate which bit x,y,z is
	theIndex = (x * 16) + (y * 4) + (z);

	//set the individual bit to 1
	theModel |= uint64_t{ 1 } << theIndex;
	return theModel;
}

//Sets the bit at x,y,z to 0
uint64_t VoxelModel::clearBit(uint64_t model, int x, int y, int z) {
	//Set model to theModel
	theModel = model;

	//Calculate which bit x,y,z is
	theIndex = (x * 16) + (y * 4) + (z);

	//clear the individual bit
	theModel &= ~(uint64_t{ 1 } << theIndex);
	return theModel;
}

//Switches the bit located at x,y,z
uint64_t VoxelModel::toggleBit(uint64_t model, int x, int y, int z) {
	//Set model to theModel
	theModel = model;

	//Calculate which bit x,y,z is
	theIndex = (x * 16) + (y * 4) + (z);

	//toggle the individual bit
	theModel ^= uint64_t{ 1 } << theIndex;
	return theModel;
}

//Writes the voxel model to an stl file to be used in CAD software
void VoxelModel::writeSTL(uint64_t model, const char* filename) {
	//Make file, numTriangles, triangle structs, int to represent the end of a triangle in the stl
	FILE* fp = fopen(filename, "wb+");
	uint32_t numTriangles = 0;
	uint16_t endTriangle = 0;
	Triangle t1, t2;

	//Make the header and write to the file, make sure it worked
	char header[80] = { };
	size_t ret = fwrite(&header, sizeof(header), 1, fp);
	assert(ret == 1);

	//Write number of triangles to file
	ret = fwrite(&numTriangles, sizeof(numTriangles), 1, fp);
	assert(ret == 1);

	//Check each bit
	for (int x = 0; x < 4; x++) {
		for (int y = 0; y < 4; y++) {
			for (int z = 0; z < 4; z++) {


				//Check directions
				if (getBit(model, x, y, z) == 1) {

					//Negative x
					if (getBit(model, x - 1, y, x) == 0 || (x - 1) < 0) {

						//Extract Face
						extractFace(x, y, z, NX, t1, t2);

						//First triangle
						ret = fwrite(&t1, sizeof(t1), 1, fp);
						assert(ret == 1);
						ret = fwrite(&endTriangle, sizeof(endTriangle), 1, fp);
						assert(ret == 1);

						//Second triangle
						ret = fwrite(&t2, sizeof(t2), 1, fp);
						assert(ret == 1);
						ret = fwrite(&endTriangle, sizeof(endTriangle), 1, fp);
						assert(ret == 1);

						//Increment total triangles
						numTriangles = numTriangles + 2;
					}

					//Positive x
					if (getBit(model, x + 1, y, z) == 0 || (x + 1) > 3) {

						//Extract Face
						extractFace(x, y, z, PX, t1, t2);

						//First triangle
						ret = fwrite(&t1, sizeof(t1), 1, fp);
						assert(ret == 1);
						ret = fwrite(&endTriangle, sizeof(endTriangle), 1, fp);
						assert(ret == 1);

						//Second triangle
						ret = fwrite(&t2, sizeof(t2), 1, fp);
						assert(ret == 1);
						ret = fwrite(&endTriangle, sizeof(endTriangle), 1, fp);
						assert(ret == 1);

						//Increment total triangles
						numTriangles = numTriangles + 2;
					}

					//Negative y
					if (getBit(model, x, y - 1, z) == 0 || (y - 1) < 0) {

						//Extract Face
						extractFace(x, y, z, NY, t1, t2);

						//First triangle
						ret = fwrite(&t1, sizeof(t1), 1, fp);
						assert(ret == 1);
						ret = fwrite(&endTriangle, sizeof(endTriangle), 1, fp);
						assert(ret == 1);

						//Second triangle
						ret = fwrite(&t2, sizeof(t2), 1, fp);
						assert(ret == 1);
						ret = fwrite(&endTriangle, sizeof(endTriangle), 1, fp);
						assert(ret == 1);

						//Increment total triangles
						numTriangles = numTriangles + 2;
					}

					//Positive y
					if (getBit(model, x, y + 1, z) == 0 || (y + 1) > 3) {

						//Extract Face
						extractFace(x, y, z, PY, t1, t2);

						//First triangle
						ret = fwrite(&t1, sizeof(t1), 1, fp);
						assert(ret == 1);
						ret = fwrite(&endTriangle, sizeof(endTriangle), 1, fp);
						assert(ret == 1);

						//Second triangle
						ret = fwrite(&t2, sizeof(t2), 1, fp);
						assert(ret == 1);
						ret = fwrite(&endTriangle, sizeof(endTriangle), 1, fp);
						assert(ret == 1);

						//Increment total triangles
						numTriangles = numTriangles + 2;
					}

					//Negative z
					if (getBit(model, x, y, z - 1) == 0 || (z - 1) < 0) {

						//Extract Face
						extractFace(x, y, z, NZ, t1, t2);

						//First triangle
						ret = fwrite(&t1, sizeof(t1), 1, fp);
						assert(ret == 1);
						ret = fwrite(&endTriangle, sizeof(endTriangle), 1, fp);
						assert(ret == 1);

						//Second triangle
						ret = fwrite(&t2, sizeof(t2), 1, fp);
						assert(ret == 1);
						ret = fwrite(&endTriangle, sizeof(endTriangle), 1, fp);
						assert(ret == 1);

						//Increment total triangles
						numTriangles = numTriangles + 2;
					}

					//Positive z
					if (getBit(model, x, y, z + 1) == 0 || (z + 1) > 3) {

						//Extract Face
						extractFace(x, y, z, PZ, t1, t2);

						//First triangle
						ret = fwrite(&t1, sizeof(t1), 1, fp);
						assert(ret == 1);
						ret = fwrite(&endTriangle, sizeof(endTriangle), 1, fp);
						assert(ret == 1);

						//Second triangle
						ret = fwrite(&t2, sizeof(t2), 1, fp);
						assert(ret == 1);
						ret = fwrite(&endTriangle, sizeof(endTriangle), 1, fp);
						assert(ret == 1);

						//Increment total triangles
						numTriangles = numTriangles + 2;
					}


				}



			}
		}
	}

	//Overwrite the number of triangles written to the file
	fseek(fp, sizeof(header), SEEK_SET);
	ret = fwrite(&numTriangles, sizeof(numTriangles), 1, fp);
	assert(ret == 1);

	//Close the file
	fclose(fp);
}

