//
//  surfaceExtraction.cpp
//  Homework3
//
//  Created by Ben Jones on 1/6/17.
//  Copyright © 2017 Ben Jones. All rights reserved.
//

//Edited by Brad Tully
//22 May 2017
//Homework 7
#define NOMINMAX
#include "SurfaceExtraction.hpp"
#include "MyArray.hpp"
#include "SharedQueue.h"

#include <cstdio>
#include <cstdint>
#include <thread>
#include <iostream>

enum FaceType {
	NX,
	NY,
	NZ,
	PX,
	PY,
	PZ
};

struct Triangle {
	float normal[3];
	float v1[3];
	float v2[3];
	float v3[3];
};

inline void FillPlane(int a1, int a2, int b1, int b2, int c, int cInd, Triangle& t1, Triangle& t2)
{
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

void ExtractFace(int x, int y, int z, FaceType face, Triangle& t1, Triangle& t2)
{
	for (int i = 0; i < 3; i++)
	{
		t1.normal[i] = 0;
		t2.normal[i] = 0;
	}
	switch (face) {
	case NX:
		t1.normal[0] = -1;
		t2.normal[0] = -1;
		FillPlane(y + 1, y, z, z + 1, x, 0, t1, t2);
		break;
	case NY:
		t1.normal[1] = -1;
		t2.normal[1] = -1;
		FillPlane(z + 1, z, x, x + 1, y, 1, t1, t2);
		break;
	case NZ:
		t1.normal[2] = -1;
		t2.normal[2] = -1;
		FillPlane(x + 1, x, y, y + 1, z, 2, t1, t2);
		break;
	case PX:
		t1.normal[0] = 1;
		t2.normal[0] = 1;
		FillPlane(y, y + 1, z, z + 1, x + 1, 0, t1, t2);
		break;
	case PY:
		t1.normal[1] = 1;
		t2.normal[1] = 1;
		FillPlane(z, z + 1, x, x + 1, y + 1, 1, t1, t2);
		break;
	case PZ:
		t1.normal[2] = 1;
		t2.normal[2] = 1;
		FillPlane(x, x + 1, y, y + 1, z + 1, 2, t1, t2);
		break;
	}
}
//Brad Tully
void WriteTriangle(FILE* f, const Triangle& t) {
	fwrite(t.normal, sizeof(float), 3, f);
	fwrite(t.v1, sizeof(float), 3, f);
	fwrite(t.v2, sizeof(float), 3, f);
	fwrite(t.v3, sizeof(float), 3, f);
	uint16_t zero = 0;
	fwrite(&zero, sizeof(zero), 1, f);
}
//Brad Tully
//Stores the x and y values of the chunk of work
struct ThreadWork {
	int x, y;
};
//Brad Tully
//Writes the triangles to an array
void WorkerThread(VoxelShape model, int x, int y, MyArray<Triangle>* t) {
	Triangle t1, t2;
	int nx, ny, nz;
	model.GetSize(nx, ny, nz);
	//Draw and write triangles
	for (int z = 0; z < nz; z++) {
		if (model.GetBit(x, y, z))
		{
			if (((x - 1) < 0) || !model.GetBit(x - 1, y, z))
			{
				ExtractFace(x, y, z, NX, t1, t2);
				t->PushBack(t1);
				t->PushBack(t2);
			}
			if (((x + 1) >= nx) || !model.GetBit(x + 1, y, z))
			{
				ExtractFace(x, y, z, PX, t1, t2);
				t->PushBack(t1);
				t->PushBack(t2);
			}
			if (((y - 1) < 0) || !model.GetBit(x, y - 1, z))
			{
				ExtractFace(x, y, z, NY, t1, t2);
				t->PushBack(t1);
				t->PushBack(t2);
			}
			if (((y + 1) >= ny) || !model.GetBit(x, y + 1, z))
			{
				ExtractFace(x, y, z, PY, t1, t2);
				t->PushBack(t1);
				t->PushBack(t2);
			}
			if ((z - 1 < 0) || !model.GetBit(x, y, z - 1))
			{

				ExtractFace(x, y, z, NZ, t1, t2);
				t->PushBack(t1);
				t->PushBack(t2);

			}
			if ((z + 1 >= nz) || !model.GetBit(x, y, z + 1))
			{
				ExtractFace(x, y, z, PZ, t1, t2);
				t->PushBack(t1);
				t->PushBack(t2);
			}
		}
	}
}
//Brad Tully
//Takes the work from the queue and saves it to an array
void MainThread(VoxelShape model, MyArray<Triangle>* t, SharedQueue<ThreadWork>* sq, std::mutex* lock) {
	ThreadWork tw = { NULL };
	MyArray<Triangle> temp;
	//Extract work and call function to draw triangles
	while (true) {
		while (sq->Remove(tw) == false)
		{
			std::this_thread::yield();
		}
		if (tw.x == -1 || tw.y == -1) {
			break;
		}
		else {
			int x, y;
			x = tw.x;
			y = tw.y;
			WorkerThread(model, x, y, &temp);
		}
	}
	//Write triangles to main array safely
	int inc = temp.Size();
	lock->lock();
	for (int i = 0; i < inc; i++) {
		t->PushBack(temp.Get(i));
	}
	lock->unlock();
}
//Brad Tully
//Writes triangles to an stl file with multiple threads
void WriteSTL(const VoxelShape &model, const char* filename) {

	MyArray<Triangle> triangles;
	SharedQueue<ThreadWork> sq;
	std::mutex l;
	ThreadWork r = { NULL };

	Triangle t1, t2;
	int nx, ny, nz;
	model.GetSize(nx, ny, nz);

	//Make threads
	std::thread **threads;
	unsigned num_threads = std::thread::hardware_concurrency();
	threads = new std::thread*[num_threads];

	std::chrono::time_point<std::chrono::system_clock> start, end;
	start = std::chrono::system_clock::now();

	//Give work to threads
	for (int i = 0; i < num_threads; i++) {
		threads[i] = new std::thread(MainThread, model, &triangles, &sq, &l);
	}

	//Add work to queue
	for (int x = 0; x < nx; x++)
	{
		for (int y = 0; y < ny; y++)
		{
			r.x = x;
			r.y = y;
			sq.Add(r);
		}
	}

	//End of work conditions
	for (int i = 0; i < num_threads; i++) {
		r.x = -1;
		r.y = -1;
		sq.Add(r);
	}

	//join and delete threads
	for (int x = 0; x < num_threads; x++)
	{
		threads[x]->join();
		delete threads[x];
	}
	delete[] threads;

	end = std::chrono::system_clock::now();
	std::chrono::duration<double> elapsed_seconds = end - start;
	std::cout << "Elapsed time with multiple threads: " << elapsed_seconds.count() << "s\n";

	//Send to file
	FILE* f = fopen(filename, "wb+");
	uint8_t header[80] = { 0 };
	fwrite(header, sizeof(uint8_t), 80, f);
	uint32_t numTris = triangles.Size();
	fwrite(&numTris, sizeof(numTris), 1, f);
	while (triangles.Size() > 0) {
		Triangle t = triangles.Back();
		triangles.PopBack();
		WriteTriangle(f, t);
	}
	fclose(f);

}
//Brad Tully
//Writes triangles to file with one thread
void SlowWriteSTL(const VoxelShape &model, const char* filename) {

	MyArray<Triangle> triangles;
	SharedQueue<ThreadWork> sq;
	std::mutex l;
	ThreadWork r = { NULL };

	Triangle t1, t2;
	int nx, ny, nz;
	model.GetSize(nx, ny, nz);

	//Create thread
	std::thread **threads;
	threads = new std::thread*[1];

	std::chrono::time_point<std::chrono::system_clock> start, end;
	start = std::chrono::system_clock::now();

	//Assign work to thread
	for (int i = 0; i < 1; i++) {
		threads[i] = new std::thread(MainThread, model, &triangles, &sq, &l);
	}

	//Add work to queue
	for (int x = 0; x < nx; x++)
	{
		for (int y = 0; y < ny; y++)
		{
			r.x = x;
			r.y = y;
			sq.Add(r);
		}
	}

	//Add end condition
	for (int i = 0; i < 1; i++) {
		r.x = -1;
		r.y = -1;
		sq.Add(r);
	}

	//Join threads and delete them
	for (int x = 0; x < 1; x++)
	{
		threads[x]->join();
		delete threads[x];
	}
	delete[] threads;

	end = std::chrono::system_clock::now();
	std::chrono::duration<double> elapsed_seconds = end - start;
	std::cout << "Elapsed time with one thread: " << elapsed_seconds.count() << "s\n";

	//Write to file
	FILE* f = fopen(filename, "wb+");
	uint8_t header[80] = { 0 };
	fwrite(header, sizeof(uint8_t), 80, f);
	uint32_t numTris = triangles.Size();
	fwrite(&numTris, sizeof(numTris), 1, f);
	while (triangles.Size() > 0) {
		Triangle t = triangles.Back();
		triangles.PopBack();
		WriteTriangle(f, t);
	}
	fclose(f);

}
