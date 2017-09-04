//Brad Tully
//Homework 4: Bigger Models
//1 May 2017
//Makes a larger 3D model using voxels

#define _CRT_SECURE_NO_WARNINGS
#include "voxelModel.hpp"
#include <math.h> 
#include <iostream>
#include "surfaceExtraction.hpp"
#include <cstdint>
#include <iostream>
#include <cmath>
#include <assert.h>

using namespace std;

//Returns the index of a bit
int getIndex(const VoxelModel& model, int x, int y, int z) {
	return z + y * model.nz + x * model.ny * model.nz;
}

//Changes the x y and z values based on the index
void getXYZ(const VoxelModel& model, int index, int& x, int& y, int& z) {
	//X value
	int remainingVal = 0;
	if (index >= model.nz * model.ny) {
		x = floor(index / (model.ny * model.nz));
	}
	else {
		x = 0;
	}

	//Y value
	remainingVal = index - x * model.ny * model.nz;
	if (remainingVal >= model.nz) {
		y = (int) floor(remainingVal / model.nz);
	}
	else {
		y = 0;
	}

	//Z value
	remainingVal = remainingVal - (y * model.nz);
	if (remainingVal >= 1) {
		z = remainingVal;
	}
	else {
		z = 0;
	}
}

//Finds which byte the index is in
int getByteNumber(int index) {
	return (int) floor(index / 8);
}

//Returns the bit number that the index is at
uint8_t getBitNumber(int index) {
	return index - getByteNumber(index) * 8;
}

//Returns a boolean value of the bit
bool getBit(const VoxelModel& model, int x, int y, int z) {
	uint8_t theByte = model.theModel[getByteNumber(getIndex(model, x, y, z))];
	return (theByte & (uint8_t{ 1 } << getBitNumber(getIndex(model, x, y, z)))) != 0;
}

//Turns the bit on
void setBit(VoxelModel& model, int x, int y, int z) {
	model.theModel[getByteNumber(getIndex(model, x, y, z))] |= uint8_t{ 1 } << getBitNumber(getIndex(model, x, y, z));
}

//Turns the bit off
void clearBit(VoxelModel& model, int x, int y, int z) {
	uint8_t full = 0xff;
	model.theModel[getByteNumber(getIndex(model, x, y, z))] &= ~(uint8_t{ 1 } << getBitNumber(getIndex(model, x, y, z)));
}

//Inverses the value of the bit
void toggleBit(VoxelModel& model, int x, int y, int z) {
	if (getBit(model, x, y, z) == true) {
		clearBit(model, x, y, z);
	}
	else {
		setBit(model, x, y, z);
	}
}

//Makes a new voxelmodel
VoxelModel allocateModel(int nx, int ny, int nz) {
	VoxelModel mod;
	mod.nx = nx;
	mod.ny = ny;
	mod.nz = nz;
	mod.size = (int) ceil((nx * ny * nz) / 8);
	mod.theModel = new uint8_t[mod.size];
	clearModel(mod);
	return mod;
}

//Clears all of the bits in the model
void clearModel(VoxelModel& model) {
	for (int i = 0; i < model.size; i++) {
		model.theModel[i] = 0;
	}
}

//Turns all of the bits in the model on
void fillModel(VoxelModel& model) {
	for (int i = 0; i < model.size; i++) {
		model.theModel[i] = 0xff;
	}
}

//Removes the heap memory of the model and sets everything else to null
void deleteModel(VoxelModel& model) {
	delete[] model.theModel;
	model.theModel = nullptr;
	model.size = 0;
	model.nx = NULL;
	model.ny = NULL;
	model.nz = NULL;
}

//Makes a sphere of voxels
void addSphere(VoxelModel& model, float cx, float cy, float cz, float radius) {

}

//Removes a sphere of voxels
void subtractSphere(VoxelModel& model, float cx, float cy, float cz, float radius) {

}

//Toggle a spherical section of voxels
void toggleSphere(VoxelModel& model, float cx, float cy, float cz, float radius) {

}

//Writes the voxel model to an stl file to be used in CAD software
void writeSTL(VoxelModel& model, const char* filename) {
	//Make file, numTriangles, triangle structs, int to represent the end of a triangle in the stl
	FILE* fp = fopen(filename, "wb+");
	uint32_t numTriangles = 0;
	uint16_t endTriangle = 0;
	Triangle t1, t2;

	//Make the header and write to the file, make sure it worked
	char header[80] = {};
	size_t ret = fwrite(&header, sizeof(header), 1, fp);
	assert(ret == 1);

	//Write number of triangles to file
	ret = fwrite(&numTriangles, sizeof(numTriangles), 1, fp);
	assert(ret == 1);

	//Check each bit
	for (int x = 0; x < model.nx; x++) {
		for (int y = 0; y < model.ny; y++) {
			for (int z = 0; z < model.nz; z++) {


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