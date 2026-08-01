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

#pragma once

#include <CL\cl.h>
#include <string>
#include <vector>
#include "ComputeKernel.h"


class ComputeKernel{
private:
	std::string  name;
	cl_kernel    kernel;  
public:
	ComputeKernel(std::string, cl_kernel);

	std::string getName();
	cl_kernel   getKernel();
};