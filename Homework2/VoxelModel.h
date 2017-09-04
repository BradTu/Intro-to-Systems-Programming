//Made by Brad Tully
//Homework3
//17 April 2017
//This program creates a 64 bit (4*4*4) 3D voxel shape
//The outline of the VoxelModel class contains variable names and function headers


#include <cstdint>
#include <cstdlib>


class VoxelModel {
	int xCoord, yCoord, zCoord;
	uint8_t theIndex, theX, theY, theZ;
	uint64_t theModel;
	bool onOrOff;

public : 
	VoxelModel();
	VoxelModel(uint64_t model);
	uint8_t getIndex(int x, int y, int z);
	uint8_t getX(uint8_t index);
	uint8_t getY(uint8_t index);
	uint8_t getZ(uint8_t index);
	uint64_t emptyModel();
	uint64_t fullModel();
	bool getBit(uint64_t model, int x, int y, int z);
	uint64_t setBit(uint64_t model, int x, int y, int z);
	uint64_t clearBit(uint64_t model, int x, int y, int z);
	uint64_t toggleBit(uint64_t model, int x, int y, int z);
	void writeSTL(uint64_t model, const char* filename);
};