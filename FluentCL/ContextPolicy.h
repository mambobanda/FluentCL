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
#include <vector>

using namespace std;

class ContextPolicy{
public:
	ContextPolicy();

	bool includeCpuDevices;
	bool includeGpuDevices;
	bool available;
	bool compilerAvailable;

	bool useMinClVersion;

	bool useMinGlobalMemorySize;
	bool useMaxGlobalMemorySize;

	bool useMinAddressBits;
	bool useMaxAddressBits;

	cl_float minClVersion;

	cl_uint minGlobalMemorySize;
	cl_uint maxGlobalMemorySize;

	cl_uint minAddressBits;
	cl_uint maxAddressBits;

	cl_uint minCpuDeviceCount;
	cl_uint maxCpuDeviceCount;

	cl_uint minGpuDeviceCount;
	cl_uint maxGpuDeviceCount;

	bool useSpecificExtensions;
	vector<string> extensions;
};