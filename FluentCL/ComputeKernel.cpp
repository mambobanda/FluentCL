/*
 * The Mailo Today Solutions Software License
 *
 * Copyright 2017 Mailo Today Solutions.
 *
 * No Permission is granted, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software.
 */

#include "ComputeKernel.h"

ComputeKernel::ComputeKernel(std::string nm, cl_kernel kern){
	name   = nm;
	kernel = kern;
}

std::string ComputeKernel::getName(){
	return name;
}

cl_kernel ComputeKernel::getKernel(){
	return kernel;
} 