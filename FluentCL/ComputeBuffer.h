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

class ComputeBuffer{
private:
	cl_mem buffer;
	//size_t size;
	bool null;
public:
	ComputeBuffer(cl_mem, size_t);
	ComputeBuffer();
	
	bool isNull();

	cl_mem getBuffer();
	size_t getSize();

	void release();
};