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

#include "ComputeBuffer.h"

ComputeBuffer::ComputeBuffer(cl_mem buff, size_t si){
	buffer  = buff;
	//size    = si;
	null    = false;
}

ComputeBuffer::ComputeBuffer(){
	null = true;
}

bool ComputeBuffer::isNull(){
	return null;
}

cl_mem ComputeBuffer::getBuffer(){
	return buffer;
}

size_t ComputeBuffer::getSize(){
	return sizeof(buffer);
}

void ComputeBuffer::release(){
	clReleaseMemObject(buffer);
}