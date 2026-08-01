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

#include <vector>
#include <CL\cl.h>
#include "Constants.h"

class DeviceInfo{
public:
	cl_device_id id;

	std::string name; 
	std::string vendor;
	std::string extensions;

	cl_ulong globalMemorySize;
	cl_uint  addressBits;
	cl_bool  available;
	cl_bool  compilerAvailable;
	std::string type;

	std::string toString();
};

class PlatformInfo{
public:
	cl_platform_id id;
	
	std::string name;
	std::string vendor;
	std::string version;
	std::string profile;
	std::string extensions;

	std::vector<DeviceInfo> cpuDevices;
	std::vector<DeviceInfo> gpuDevices;

	cl_uint numCpuDevices;
	cl_uint numGpuDevices;

	int platformState;

	std::string toString();
};