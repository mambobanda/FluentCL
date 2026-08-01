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
#include "ContextPolicy.h"

class Context{
private:
	bool ready;
	cl_context _context;
	vector<DeviceInfo> _cpu;
	vector<DeviceInfo> _gpu;
	vector<cl_command_queue> cpuCommandQueues;
	vector<cl_command_queue> gpuCommandQueues;
	std::string platformName;
	std::string platformVendor;
	std::string platformVersion;
	int numCpu;
	int numGpu;
public:
	Context();
	int newContext(ContextPolicy policy, PlatformInfo &info);
	
	int getNumDevices();
	int getNumCpuDevices();
	int getNumGpuDevices();

	std::string getCpuDeviceInfo(int);
	std::string getGpuDeviceInfo(int);
	std::string getPlatformName();
	std::string getPlatformVendor();
	std::string getPlatformVersion();

	cl_device_id getCpuDevice(int );
	cl_device_id getGpuDevice(int );
	cl_context   getContext();

	cl_command_queue getCpuCommandQueue(int);
	cl_command_queue getGpuCommandQueue(int);
};