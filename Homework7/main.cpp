//
//  main.cpp
//  Homework2
//
//  Created by Ben Jones on 1/4/17.
//  Copyright © 2017 Ben Jones. All rights reserved.
//

//Edited by Brad Tully
//22 May 2017
//Homework 7
#define NOMINMAX
#include <iostream>

#include <random>
#include <cassert>
#include <chrono>
#include "SurfaceExtraction.hpp"

void TestBitVector();

int main(int argc, const char * argv[])
{

	VoxelShape m(50, 100, 150);
	m.AddSphere(20, 30, 40, 40);
	m.SubtractSphere(20, 50, 40, 30);
	m.AddSphere(40, 70, 120, 40);

	//Faster method
	WriteSTL(m, "FastModel.stl");

	//Slower original method
	SlowWriteSTL(m, "SlowModel.stl");

}

void TestBitVector()
{
	BitVector b(999);
	BitVector b2(20);

	for (int x = 0; x < 999; x += 2)
		b.Set(x, 1);

	b2 = b;

	assert(b.Size() == b2.Size());
	for (int x = 0; x < 999; x++)
		assert(b.Get(x) == b2.Get(x));

	b.Resize(0);
	b.Resize(999);
	for (int x = 0; x < 999; x++)
	{
		assert(b.Get(x) == 0);
		b.Set(x, 1);
	}
	b.Resize(1002);
	for (int x = 999; x < b.Size(); x++)
	{
		assert(b.Get(x) == 0);
	}


	BitVector bv(1);
	bv.Fill();
	bv.Resize(64);
	assert(bv.Get(0));
	for (int i = 1; i < 64; i++) {
		assert(!bv.Get(i));
	}


	printf("BitVector passed tests\n");
}
