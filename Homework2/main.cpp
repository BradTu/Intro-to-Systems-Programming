//Made by Brad Tully
//Homework3
//17 April 2017
//This program creates a 64 bit (4*4*4) 3D voxel shape
//Shows that all of the VoxelModel functions work

#include "VoxelModel.h"
#include <cstdint>
#include <cstdlib>
#include <iostream>

using namespace std;

int main() {
	//Create a VoxelModel object
	VoxelModel vm;
	
	//Show that full model works and show several functions executed on it
	cout << "Full Model: " << hex << "0x" << vm.fullModel() <<  endl;
	cout << "Get the Set Bit: " << boolalpha << vm.getBit(vm.setBit(vm.fullModel(), 0, 0, 1), 0, 0, 1) << endl;
	cout << "Value Model Set Bit: " << hex << "0x" << vm.setBit(vm.fullModel(), 0, 0, 1) << endl;
	cout << "Value After Clear Bit: " << hex << "0x" << vm.clearBit(vm.fullModel(), 0, 0, 1) << endl;
	cout << "Value After Toggle Bit: " << hex << "0x" << vm.toggleBit(vm.fullModel(), 0, 0, 1) << endl;

	cout << " " << endl;

	//Show that empty model works and show several functions executed on it
	cout << "Empty Model: " << hex << "0x" << vm.emptyModel() << endl;
	cout << "Get the Set Bit: " << boolalpha << vm.getBit(vm.setBit(vm.emptyModel(), 0, 0, 1), 0, 0, 1) << endl;
	cout << "Value Model Set Bit: " << hex << "0x" << vm.setBit(vm.emptyModel(), 0, 0, 1) << endl;
	cout << "Value After Clear Bit: " << hex << "0x" << vm.clearBit(vm.emptyModel(), 0, 0, 1) << endl;
	cout << "Value After Toggle Bit: " << hex << "0x" << vm.toggleBit(vm.emptyModel(), 0, 0, 1) << endl;

	cout << " " << endl;

	//Shows the getIndex function and the getX/Y/Z functions
	cout << "Index: " << dec << +vm.getIndex(3, 3, 2) << endl;
	cout << "x: " << dec << +vm.getX(62) << endl;
	cout << "y: " << dec << +vm.getY(62) << endl;
	cout << "z: " << dec << +vm.getZ(62) << endl;

	return 0;
}