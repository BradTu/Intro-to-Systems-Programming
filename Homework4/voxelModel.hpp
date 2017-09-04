//Brad Tully
//Homework 4: Bigger Models
//1 May 2017
//Makes a larger 3D model using voxels

#pragma once
#include <cstdint>

//Struct that stores the model
struct VoxelModel {
	int nx = NULL;
	int ny = NULL;
	int nz = NULL;
	int size = NULL;
	uint8_t* theModel;
};

VoxelModel allocateModel(int nx, int ny, int nz);
void clearModel(VoxelModel& model);
void fillModel(VoxelModel& model);
void deleteModel(VoxelModel& model);

//indexing functions
//index = z + y * nz + x * ny * nz
int getIndex(const VoxelModel& model, int x, int y, int z);
void getXYZ(const VoxelModel& model, int index, int& x, int& y, int& z);

int getByteNumber(int index);
uint8_t getBitNumber(int index);

bool getBit(const VoxelModel& model, int x, int y, int z);
void setBit(VoxelModel& model, int x, int y, int z);
void clearBit(VoxelModel& model, int x, int y, int z);
void toggleBit(VoxelModel& model, int x, int y, int z);

//Write the shape to an stl
void writeSTL(VoxelModel& model, const char* filename);

//Circle functions
void addSphere(VoxelModel& model, float cx, float cy, float cz, float radius);
void subtractSphere(VoxelModel& model, float cx, float cy, float cz, float radius);
void toggleSphere(VoxelModel& model, float cx, float cy, float cz, float radius);