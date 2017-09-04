//Made by Brad Tully
//Homework3
//17 April 2017
//This program creates a 64 bit (4*4*4) 3D voxel shape
//This file contains the functions for the VoxelModel class

#include "VoxelModel.h"
#include <cstdint>
#include <iostream>
#include <cmath>

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
	//Set model to theModel and a temporary bit int
	theModel = model;
	int bit;

	//Calculate which bit x,y,z is and if it is 1 or 0
	theIndex = (x * 16) + (y * 4) + (z);
	//The following line was written with the help of this forum on stack overflow credit: forefinger
	// http://stackoverflow.com/questions/2249731/how-do-i-get-bit-by-bit-data-from-an-integer-value-in-c
	bit = (theModel & (1 << theIndex)) >> theIndex;

	//Check whether or not that bit is 1 or 0 and return true or false
	if (bit >= 1) {
		onOrOff = true;
	}
	else if (bit == 0) {
		onOrOff = false;
	}
	return onOrOff;
}

//Sets the bit at x,y,z to 1
uint64_t VoxelModel::setBit(uint64_t model, int x, int y, int z) {
	//Set model to theModel
	theModel = model;

	//Calculate which bit x,y,z is
	theIndex = (x * 16) + (y * 4) + (z);

	//set the individual bit to 1
	theModel |= 1 << theIndex;
	return theModel;
}

//Sets the bit at x,y,z to 0
uint64_t VoxelModel::clearBit(uint64_t model, int x, int y, int z) {
	//Set model to theModel
	theModel = model;

	//Calculate which bit x,y,z is
	theIndex = (x * 16) + (y * 4) + (z);

	//clear the individual bit
	theModel &= ~(1 << theIndex);
	return theModel;
}

//Switches the bit located at x,y,z
uint64_t VoxelModel::toggleBit(uint64_t model, int x, int y, int z) {
	//Set model to theModel
	theModel = model;

	//Calculate which bit x,y,z is
	theIndex = (x * 16) + (y * 4) + (z);

	//toggle the individual bit
	theModel ^= 1 << theIndex;
	return theModel;
}

void VoxelModel::writeSTL(uint64_t model, const char* filename) {

}
