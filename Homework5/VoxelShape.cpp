//Brad Tully
//Homework 5: Class Encapsulation
//8 May 2017
//Uses the BitVector to make a model

#include "VoxelShape.h"
#include <algorithm>
#include <cstdio>
#include <cstdint>

//Constructor makes a bit vector with the correct dimensions
VoxelShape::VoxelShape(int numX, int numY, int numZ) {
	nx = numX;
	ny = numY;
	nz = numZ;
	bitVec = new BitVector(nx * ny * nz);
}

//Destructor
VoxelShape::~VoxelShape(){
	bitVec->~BitVector();
}

//Clears all of the bits in the model
void VoxelShape::Clear() {
	bitVec->clearBits();
}

//Turns on all of the bits in the model
void VoxelShape::Fill() {
	bitVec->fillBits();
}

//Returns the value of the specified bit
bool VoxelShape::getBit(int x, int y, int z) {
	int index = getIndex(x, y, z);
	return bitVec->getBit(index);
}

//Sets the specified bit to be on
void VoxelShape::setBit(int x, int y, int z) {
	int index = getIndex(x, y, z);
	bitVec->setBit(index, true);
}

//Clears the specified bit to be off
void VoxelShape::clearBit(int x, int y, int z) {
	int index = getIndex(x, y, z);
	bitVec->setBit(index, false);
}

//Reverses the state of the bit
void VoxelShape::toggleBit(int x, int y, int z) {
	int index = getIndex(x, y, z);
	bool tf = bitVec->getBit(index);
	if (tf == true) {
		bitVec->setBit(index, false);
	}
	else if (tf == false) {
		bitVec->setBit(index, true);
	}
}
//Written by Ben Jones with edits from Brad Tully
//Adds a sphere shape to the model
void VoxelShape::addSphere(float x, float y, float z, float r) {
	int index = 0;
	int minX = std::max(0, static_cast<int>(x - r));
	int maxX = std::min(nx, static_cast<int>(x + r));
	int minY = std::max(0, static_cast<int>(y - r));
	int maxY = std::min(ny, static_cast<int>(y + r));
	int minZ = std::max(0, static_cast<int>(z - r));
	int maxZ = std::min(nz, static_cast<int>(z + r));
	for (int i = minX; i < maxX; i++) {
		for (int j = minY; j < maxY; j++) {
			for (int k = minZ; k < maxZ; k++) {
				if (((x - i)*(x - i) + (y - j)*(y - j) + (z - k)*(z - k)) < r*r) {
					index = getIndex(i, j, k);
					bitVec->setBit(index, true);
				}
			}
		}
	}
}
//Written by Ben Jones with edits from Brad Tully
//Removes a sphere shape from the model
void VoxelShape::subtractSphere(float x, float y, float z, float r) {
	int minX = std::max(0, static_cast<int>(x - r));
	int maxX = std::min(nx, static_cast<int>(x + r));
	int minY = std::max(0, static_cast<int>(y - r));
	int maxY = std::min(ny, static_cast<int>(y + r));
	int minZ = std::max(0, static_cast<int>(z - r));
	int maxZ = std::min(nz, static_cast<int>(z + r));
	for (int i = minX; i < maxX; i++) {
		for (int j = minY; j < maxY; j++) {
			for (int k = minZ; k < maxZ; k++) {
				if (((x - i)*(x - i) + (y - j)*(y - j) + (z - k)*(z - k)) < r*r) {
					int index = getIndex(i, j, k);
					bitVec->setBit(index, false);
				}
			}
		}
	}
}
//Written by Ben Jones with edits from Brad Tully
//Toggles a shpere shape in the model
void VoxelShape::toggleSphere(float x, float y, float z, float r) {
	int minX = std::max(0, static_cast<int>(x - r));
	int maxX = std::min(nx, static_cast<int>(x + r));
	int minY = std::max(0, static_cast<int>(y - r));
	int maxY = std::min(ny, static_cast<int>(y + r));
	int minZ = std::max(0, static_cast<int>(z - r));
	int maxZ = std::min(nz, static_cast<int>(z + r));
	for (int i = minX; i < maxX; i++) {
		for (int j = minY; j < maxY; j++) {
			for (int k = minZ; k < maxZ; k++) {
				if (((x - i)*(x - i) + (y - j)*(y - j) + (z - k)*(z - k)) < r*r) {
					int index = getIndex(i, j, k);
					bool tf = bitVec->getBit(index);
					if (tf == true) {
						bitVec->setBit(index, false);
					}
					else if (tf == false) {
						bitVec->setBit(index, true);
					}
				}
			}
		}
	}
}

//Returns the index of the bit
int VoxelShape::getIndex(int x, int y, int z) {
	return x * ny * nz + y * nz + z;
}
//FaceType Written by Ben Jones
enum FaceType {
	NX,
	NY,
	NZ,
	PX,
	PY,
	PZ
};
//Triangle Written by Ben Jones
struct Triangle {
	float normal[3];
	float v1[3];
	float v2[3];
	float v3[3];
};
//fillPlane Written by Ben Jones
inline void fillPlane(int a1, int a2, int b1, int b2, int c, int cInd, Triangle& t1, Triangle& t2) {
	t1.v1[cInd] = c;
	t2.v1[cInd] = c;
	t1.v2[cInd] = c;
	t2.v2[cInd] = c;
	t1.v3[cInd] = c;
	t2.v3[cInd] = c;
	int aInd = (cInd + 1) % 3;
	int bInd = (cInd + 2) % 3;

	t1.v1[aInd] = a1;
	t1.v2[aInd] = a2;
	t1.v3[aInd] = a2;

	t2.v1[aInd] = a1;
	t2.v2[aInd] = a2;
	t2.v3[aInd] = a1;

	t1.v1[bInd] = b1;
	t1.v2[bInd] = b1;
	t1.v3[bInd] = b2;

	t2.v1[bInd] = b1;
	t2.v2[bInd] = b2;
	t2.v3[bInd] = b2;

}
//extractFace Written by Ben Jones
void extractFace(int x, int y, int z, FaceType face, Triangle& t1, Triangle& t2) {
	for (int i = 0; i < 3; i++) {
		t1.normal[i] = 0;
		t2.normal[i] = 0;
	}
	switch (face) {
	case NX:
		t1.normal[0] = -1;
		t2.normal[0] = -1;
		fillPlane(y + 1, y, z, z + 1, x, 0, t1, t2);
		break;
	case NY:
		t1.normal[1] = -1;
		t2.normal[1] = -1;
		fillPlane(z + 1, z, x, x + 1, y, 1, t1, t2);
		break;
	case NZ:
		t1.normal[2] = -1;
		t2.normal[2] = -1;
		fillPlane(x + 1, x, y, y + 1, z, 2, t1, t2);
		break;
	case PX:
		t1.normal[0] = 1;
		t2.normal[0] = 1;
		fillPlane(y, y + 1, z, z + 1, x + 1, 0, t1, t2);
		break;
	case PY:
		t1.normal[1] = 1;
		t2.normal[1] = 1;
		fillPlane(z, z + 1, x, x + 1, y + 1, 1, t1, t2);
		break;
	case PZ:
		t1.normal[2] = 1;
		t2.normal[2] = 1;
		fillPlane(x, x + 1, y, y + 1, z + 1, 2, t1, t2);
		break;
	}
}

void writeTriangle(FILE* f, const Triangle& t) {
	fwrite(t.normal, sizeof(float), 3, f);
	fwrite(t.v1, sizeof(float), 3, f);
	fwrite(t.v2, sizeof(float), 3, f);
	fwrite(t.v3, sizeof(float), 3, f);
	uint16_t zero = 0;
	fwrite(&zero, sizeof(zero), 1, f);
}

//Writes the shape to an stl file that can be used in CAD software
void writeSTL(VoxelShape& model, const char* filename) {

	FILE* f = fopen(filename, "wb");
	uint8_t header[80] = { 0 };
	fwrite(header, sizeof(uint8_t), 80, f);
	uint32_t numTris = 0;
	fwrite(&numTris, sizeof(numTris), 1, f);
	Triangle t1, t2;
	for (int x = 0; x < model.nx; x++) {
		for (int y = 0; y < model.ny; y++) {
			for (int z = 0; z < model.nz; z++) {
				if (model.getBit(x, y, z)){
					if (((x - 1) < 0) || !model.getBit(x - 1, y, z)) {
						extractFace(x, y, z, NX, t1, t2);
						writeTriangle(f, t1);
						writeTriangle(f, t2);
						numTris += 2;
					}

					if (((x + 1) >= model.nx) || !model.getBit(x + 1, y, z)) {
						extractFace(x, y, z, PX, t1, t2);
						writeTriangle(f, t1);
						writeTriangle(f, t2);
						numTris += 2;
					}

					if (((y - 1) < 0) || !model.getBit(x, y - 1, z)) {
						extractFace(x, y, z, NY, t1, t2);
						writeTriangle(f, t1);
						writeTriangle(f, t2);
						numTris += 2;
					}
					if (((y + 1) >= model.ny) || !model.getBit(x, y + 1, z)) {
						extractFace(x, y, z, PY, t1, t2);
						writeTriangle(f, t1);
						writeTriangle(f, t2);
						numTris += 2;
					}


					if ((z - 1 < 0) || !model.getBit(x, y, z - 1)) {

						extractFace(x, y, z, NZ, t1, t2);
						writeTriangle(f, t1);
						writeTriangle(f, t2);
						numTris += 2;

					}

					if ((z + 1 >= model.nz) || !model.getBit(x, y, z + 1)) {
						extractFace(x, y, z, PZ, t1, t2);
						writeTriangle(f, t1);
						writeTriangle(f, t2);
						numTris += 2;

					}
				}
			}
		}
	}
	fseek(f, 80, SEEK_SET);
	fwrite(&numTris, sizeof(numTris), 1, f);
	fclose(f);
}



