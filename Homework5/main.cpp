//Brad Tully
//Homework 5: Class Encapsulation
//8 May 2017
//Test all of the classes/ functions

#include <iostream>
#include <random>
#include <cassert>
#include "VoxelShape.h"

using namespace std;

int main(int argc, const char * argv[]) {
	VoxelShape vs(50, 50, 50);
	
	vs.Clear();
	vs.Fill();

	vs.subtractSphere(50, 50, 25, 25);
	vs.addSphere(50, 50, 25, 25);
	vs.toggleSphere(50, 50, 25, 25);
	vs.toggleSphere(0, 0, 25, 25);
	vs.subtractSphere(50, 0, 25, 25);
	vs.subtractSphere(0, 50, 25, 25);

	cout << vs.getBit(0, 0, 0) << endl;
	vs.clearBit(0, 0, 0);
	cout << vs.getBit(0, 0, 0) << endl;
	vs.setBit(0, 0, 0);
	cout << vs.getBit(0, 0, 0) << endl;
	vs.toggleBit(0, 0, 0);
	cout << vs.getBit(0, 0, 0) << endl;
	vs.toggleBit(0, 0, 0);
	cout << vs.getBit(0, 0, 0) << endl;

	writeSTL(vs, "TheModel.stl");

	vs.~VoxelShape();
}
