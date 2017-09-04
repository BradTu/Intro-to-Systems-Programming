//
//  surfaceExtraction.hpp
//  Homework3
//
//  Created by Ben Jones on 1/6/17.
//  Copyright © 2017 Ben Jones. All rights reserved.
//

//Edited by Brad Tully
//22 May 2017
//Homework 7

#pragma once
#define NOMINMAX
#include <cstdint>
#include "VoxelShape.hpp"

void WriteSTL(const VoxelShape &model, const char* filename);
void SlowWriteSTL(const VoxelShape &model, const char* filename);