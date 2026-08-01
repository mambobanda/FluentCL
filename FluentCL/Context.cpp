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

#include "Constants.h"
#include "PlatformInfo.h"
#include "Context.h"

Context::Context(){
	_context = NULL;
	ready = false;
}

int Context::newContext(ContextPolicy policy, PlatformInfo &info){

	bool contextReady = true;

	int cpuCount = 0;
	int gpuCount = 0;
	int cpuCounter = 0;
	int gpuCounter = 0;

	char *clVersion = NULL;
	clVersion = strtok((char*)info.version.c_str(), " ");
	clVersion = strtok(NULL, " ");
	double f_clVersion = atof(clVersion);

	if(policy.useMinClVersion)
		if(f_clVersion < policy.minClVersion) return POLICY_USE_MIN_CL_VERSION_FAILED_STATE;


	if(info.platformState != FOUND_DEVICES_STATE) return info.platformState;
	
	if(!policy.includeCpuDevices && !policy.includeGpuDevices) return POLICY_INCLUDE_DEVICES_FAILED_STATE;

	if(policy.includeCpuDevices){
		if(info.numCpuDevices == 0) return POLICY_INCLUDE_CPU_DEVICE_FAILED_STATE;
		else if(policy.minCpuDeviceCount == 0) return POLICY_MIN_CPU_FAILED_STATE;
		else if(policy.minCpuDeviceCount > info.numCpuDevices) return POLICY_MIN_CPU_FAILED_STATE;
		else if(policy.minCpuDeviceCount > policy.maxCpuDeviceCount) return POLICY_MAX_CPU_FAILED_STATE;
		
		if(policy.maxCpuDeviceCount > info.numCpuDevices) cpuCount = info.numCpuDevices;
		else cpuCount = policy.maxCpuDeviceCount;
			
	}

	if(policy.includeGpuDevices){
		if(info.numGpuDevices == 0) return POLICY_INCLUDE_GPU_DEVICE_FAILED_STATE;
		else if(policy.minGpuDeviceCount == 0) return POLICY_MIN_GPU_FAILED_STATE;
		else if(policy.minGpuDeviceCount > info.numGpuDevices) return POLICY_MIN_GPU_FAILED_STATE;
		else if(policy.minGpuDeviceCount > policy.maxGpuDeviceCount) return POLICY_MAX_GPU_FAILED_STATE;
		
		if(policy.maxGpuDeviceCount > info.numGpuDevices) gpuCount = info.numGpuDevices;
		else gpuCount = policy.maxGpuDeviceCount;
			
	}

	if(policy.includeCpuDevices){
		for(unsigned int i=0; i < info.numCpuDevices; i++){

			if(policy.useMinGlobalMemorySize)
				if(policy.minGlobalMemorySize > info.cpuDevices[i].globalMemorySize) 
					continue;

			if(policy.useMaxGlobalMemorySize)
				if(policy.maxGlobalMemorySize < info.cpuDevices[i].globalMemorySize) 
					continue;

			if(policy.useMinAddressBits)
				if(policy.minAddressBits > info.cpuDevices[i].addressBits)
					continue;

			if(policy.useMaxAddressBits)
				if(policy.maxAddressBits < info.cpuDevices[i].addressBits)
					continue;

			_cpu.push_back(info.cpuDevices[i]);

			cpuCounter++;
			if(cpuCounter == cpuCount) break;
		}
	}

	if(policy.includeGpuDevices){
		for(unsigned int i=0; i < info.numGpuDevices; i++){

			if(policy.useMinGlobalMemorySize)
				if(policy.minGlobalMemorySize > info.gpuDevices[i].globalMemorySize) 
					continue;

			if(policy.useMaxGlobalMemorySize)
				if(policy.maxGlobalMemorySize < info.gpuDevices[i].globalMemorySize) 
					continue;

			if(policy.useMinAddressBits)
				if(policy.minAddressBits > info.gpuDevices[i].addressBits)
					continue;

			if(policy.useMaxAddressBits)
				if(policy.maxAddressBits < info.gpuDevices[i].addressBits)
					continue;

			_gpu.push_back(info.gpuDevices[i]);

			gpuCounter++;
			if(gpuCounter == gpuCount) break;
		}
	}

	if((gpuCount + cpuCount) == (_cpu.size() + _gpu.size())){
		platformName     = info.name;
		platformVendor   = info.vendor; 
		platformVersion  = info.version;

		cl_device_id *_ids = new cl_device_id[_cpu.size() + _gpu.size()]; 

		int counter = 0;
		for(int i=0; i < _cpu.size(); i++){
			_ids[counter] = _cpu[i].id;
			counter++;
		}

		for(int i=0; i < _gpu.size(); i++){
			_ids[counter] = _gpu[i].id;
			counter++;
		}

		cl_int err;

		_context = clCreateContext(NULL, counter, _ids , NULL, NULL, &err);

		delete[] _ids; 

		if(err < 0){
			_context = NULL;
			_cpu.clear();
			_gpu.clear();
			return FAILED_TO_CREATE_CONTEXT_STATE;
		}else{

			for(int d=0; d < _cpu.size(); d++){
				cl_int err;
				cl_command_queue queue = NULL;
				queue = clCreateCommandQueue(_context, _cpu[d].id, 0, &err);

				if(err < 0) {
					queue = NULL;
					cpuCommandQueues.push_back(queue);
				}else{
					cpuCommandQueues.push_back(queue);
				}
			}

			for(int d=0; d < _gpu.size(); d++){
				cl_int err;
				cl_command_queue queue = NULL;
				queue = clCreateCommandQueue(_context, _gpu[d].id, 0, &err);

				if(err < 0) {
					queue = NULL;
					gpuCommandQueues.push_back(queue);
				}else{
					gpuCommandQueues.push_back(queue);
				}
			}
		}

		ready = true;
		return SUCCESS_STATE;
	}else{
		_cpu.clear();
		_gpu.clear();
		return FAILED_TO_CREATE_CONTEXT_POLICY_FAILED_STATE;
	}
}

cl_command_queue Context::getCpuCommandQueue(int inx){
	return cpuCommandQueues.at(inx);
}

cl_command_queue Context::getGpuCommandQueue(int inx){
	return gpuCommandQueues.at(inx);
}

int Context::getNumDevices(){
	return numCpu + numGpu;
}

int Context::getNumCpuDevices(){
	return (int)_cpu.size();
}

int Context::getNumGpuDevices(){
	return (int)_gpu.size();
}

std::string Context::getCpuDeviceInfo(int inx){
	return _cpu[inx].toString();
}

std::string Context::getGpuDeviceInfo(int inx){
	return _gpu[inx].toString();
}

std::string Context::getPlatformName(){
	return platformName;
}

std::string Context::getPlatformVendor(){
	return platformVendor;
}

std::string Context::getPlatformVersion(){
	return platformVersion;
}

cl_device_id Context::getCpuDevice(int inx){
	return _cpu[inx].id;
}

cl_device_id Context::getGpuDevice(int inx){
	return _gpu[inx].id;
}

cl_context Context::getContext(){
	return _context;
}