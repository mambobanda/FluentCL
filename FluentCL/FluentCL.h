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

#define CL_USE_DEPRECATED_OPENCL_1_2_APIS

#include <CL\cl.h>
#include <sstream>
#include <string>
#include <unordered_map>
#include "ComputeBuffer.h"
#include "ComputeKernel.h"
#include "ComputeBuffer.h"
#include "ContextPolicy.h"
#include "Constants.h"
#include "PlatformInfo.h"
#include "PlatformState.h"
#include "Context.h"


class FluentCL{
private:
	static int platformState;
	Context context;
	std::string compilationOutput;
	vector<ComputeKernel> kernelMap; 

	void addPlatformDevice(cl_device_id &, std::vector<DeviceInfo> &, string);
	int  buildProgram(const char ** , int , const size_t *, double );
	std::string buildCompilationLog(cl_program);

	void waitAndReleaseDevice(cl_event event);
#ifdef _WIN32
	wchar_t *_charToLPCWSTR(const char* arr);
	bool _fileExists(const char* path);
#endif

public:
	static bool infoInitialised;
	static std::vector<PlatformInfo> clPlatforms;
public:
	FluentCL();
	void initInfo();
	int  getPlatformState();
	std::string compilePlatformInfo();
	Context     &getContext();
	void clearContext();
	int  createContext(ContextPolicy );

	int loadKernelsFromSource(std::string path, double clversion);
	int loadKernelsFromSource(vector<std::string> paths, double clversion);
	int loadKernelsFromSource(const char** source, int size, double clversion);

	ComputeBuffer createReadWriteBuffer(void* data,  size_t size);
	ComputeBuffer createReadWriteBuffer(size_t size);
	ComputeBuffer createReadOnlyBuffer(void* data,  size_t size);
	ComputeBuffer createWriteOnlyBuffer(size_t size);
	ComputeBuffer createReadWriteLocalBuffer(void* data,  size_t size);
	ComputeBuffer createReadOnlyLocalBuffer(void* data,  size_t size);

	int executeKernel(string name, int deviceType, int deviceIndex);
	int executeKernelParallel1D(string name, int deviceType, int deviceIndex, int workitems, int localsize, bool wait=false);
	int executeKernelParallel2D(string name, int deviceType, int deviceIndex, size_t *workitems, size_t *localsize, bool wait=false);
	int setKernelArgument(string name, cl_int inx, ComputeBuffer cb);
	int setKernelArgument(string name, cl_int inx, size_t size, void* data);

	int readComputeBuffer(int deviceType, int deviceIndex, ComputeBuffer cb, size_t size, void *data);
	int mapReadComputeBuffer(int deviceType, int deviceIndex, ComputeBuffer cb, size_t size, void *data);
	int writeToComputeBuffer(int, int, ComputeBuffer);

	std::string getCompilationOutput();
};