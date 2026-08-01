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

#include <iostream>
#include <fstream>
#include <sstream>
#include "FluentCL.h"

#ifdef _WIN32
#include <Shlwapi.h>
#endif

int  FluentCL::platformState   = 0;
bool FluentCL::infoInitialised = false;
std::vector<PlatformInfo> FluentCL::clPlatforms;

FluentCL::FluentCL(){
	// Get and store once per app instance platform and device infomation
	if(!infoInitialised){
		initInfo();	
		infoInitialised = true;
	}
}

void FluentCL::initInfo(){
	int rc = 0;

	cl_platform_id *platforms;
	cl_uint         numPlatforms;

	// Get Number of available platforms
	rc = clGetPlatformIDs(MAXIMUM_PLATFORMS, NULL, &numPlatforms);

	if(rc < 0){
		platformState = NO_OPENCL_PLATFORMS_AVAILABLE_STATE;
		return;
	}

	platforms = NULL;
	platforms = new  cl_platform_id[numPlatforms];

	// Get Platform ID's
	rc = clGetPlatformIDs(numPlatforms, platforms, NULL);

	if(rc < 0){
		platformState = FAILED_TO_GET_OPENCL_PLATFORMS_STATE;
		delete[] platforms;
		return;
	}

	// Get and store platform and device infomation
	for(unsigned int i=0; i < numPlatforms; i++){
		PlatformInfo platformInfo;

		int  err = 0;
		char name[100];
		char vendor[100];
		char version[100];
		char profile[100];
		char extensions[2000];

		err = clGetPlatformInfo( platforms[i], CL_PLATFORM_NAME,       sizeof(name),       &name, NULL);

		if(err < 0)
			platformInfo.name = "N/A";
		else
			platformInfo.name = name;

		err = clGetPlatformInfo( platforms[i], CL_PLATFORM_VENDOR,     sizeof(vendor),     &vendor, NULL);

		if(err < 0)
			platformInfo.vendor = "N/A";
		else
			platformInfo.vendor = vendor;

		err = clGetPlatformInfo( platforms[i], CL_PLATFORM_VERSION,    sizeof(version),    &version, NULL);

		if(err < 0)
			platformInfo.version = "N/A";
		else
			platformInfo.version = version;

		err = clGetPlatformInfo( platforms[i], CL_PLATFORM_PROFILE,    sizeof(profile),    &profile, NULL);

		if(err < 0)
			platformInfo.profile = "N/A";
		else
			platformInfo.profile = profile;

		err = clGetPlatformInfo( platforms[i], CL_PLATFORM_EXTENSIONS, sizeof(extensions), &extensions, NULL);
		
		if(err < 0)
			platformInfo.extensions = "N/A";
		else
			platformInfo.extensions = extensions;
		

		platformInfo.numCpuDevices = 0;
		platformInfo.numGpuDevices = 0;

		int _rc1 = clGetDeviceIDs(platforms[i], CL_DEVICE_TYPE_CPU, MAXIMUM_CPU_DEVICES, NULL, &platformInfo.numCpuDevices);
		int _rc2 = clGetDeviceIDs(platforms[i], CL_DEVICE_TYPE_GPU, MAXIMUM_GPU_DEVICES, NULL, &platformInfo.numGpuDevices);
		
		if(_rc1 < 0 && _rc2 < 0){
			platformInfo.platformState = FAILED_TO_FIND_DEVICES_STATE;
			clPlatforms.push_back(platformInfo);
			continue;
		}

		cl_device_id *cpu_devices = NULL;
		cl_device_id *gpu_devices = NULL; 

		if(platformInfo.numCpuDevices > 0)
			cpu_devices = (cl_device_id *)malloc(sizeof(cl_device_id ) * platformInfo.numCpuDevices);

		if(platformInfo.numGpuDevices > 0)
			gpu_devices = (cl_device_id *)malloc(sizeof(cl_device_id ) * platformInfo.numGpuDevices);
		
		if(platformInfo.numCpuDevices > 0)
			int err = clGetDeviceIDs(platforms[i], CL_DEVICE_TYPE_CPU, platformInfo.numCpuDevices, cpu_devices, NULL);

		if(platformInfo.numGpuDevices > 0)
			clGetDeviceIDs(platforms[i], CL_DEVICE_TYPE_GPU, platformInfo.numGpuDevices, gpu_devices, NULL);

		for(unsigned int j=0; j < platformInfo.numCpuDevices; j++){
			addPlatformDevice(cpu_devices[j], platformInfo.cpuDevices, "CPU");
		}

		for(unsigned int j=0; j < platformInfo.numGpuDevices; j++){
			addPlatformDevice(gpu_devices[j], platformInfo.gpuDevices, "GPU");
		}

		platformInfo.platformState = FOUND_DEVICES_STATE;
		clPlatforms.push_back(platformInfo);

		delete[] cpu_devices;
		delete[] gpu_devices;
	}

	platformState = OPENCL_PLATFORMS_AVAILABLE_STATE;
	
	delete[] platforms;
}

void FluentCL::addPlatformDevice(cl_device_id &devid, std::vector<DeviceInfo> &arr, string type){

	DeviceInfo devinfo;
	devinfo.id = devid;

	int err = 0;
	char dname[100];
	char dvendor[100];
	char dextensions[2000];
	cl_ulong globalMemorySize;
	cl_uint addressBits;
	cl_bool available;
	cl_bool compilerAvailable;

	err = clGetDeviceInfo(devinfo.id, CL_DEVICE_NAME, sizeof(dname), dname, NULL);

	if(err < 0)
		devinfo.name = "N/A";
	else
		devinfo.name = dname;

	err = clGetDeviceInfo(devinfo.id, CL_DEVICE_VENDOR, sizeof(dvendor), dvendor, NULL);

	if(err < 0)
		devinfo.vendor = "N/A";
	else
		devinfo.vendor = dvendor;

	err = clGetDeviceInfo(devinfo.id, CL_DEVICE_EXTENSIONS, sizeof(dextensions), dextensions, NULL);

	if(err < 0)
		devinfo.extensions = "N/A";
	else
		devinfo.extensions = dextensions;

	err = clGetDeviceInfo(devinfo.id, CL_DEVICE_GLOBAL_MEM_SIZE, sizeof(globalMemorySize), &globalMemorySize, NULL);

	if(err < 0)
		devinfo.globalMemorySize = 0;
	else
		devinfo.globalMemorySize = globalMemorySize;

	err = clGetDeviceInfo(devinfo.id, CL_DEVICE_ADDRESS_BITS, sizeof(addressBits), &addressBits, NULL);

	if(err < 0)
		devinfo.addressBits = 0;
	else
		devinfo.addressBits = addressBits;

	err = clGetDeviceInfo(devinfo.id, CL_DEVICE_AVAILABLE, sizeof(available), &available, NULL);

	if(err < 0)
		devinfo.available = false;
	else
		devinfo.available = available;

	err = clGetDeviceInfo(devinfo.id, CL_DEVICE_COMPILER_AVAILABLE, sizeof(compilerAvailable), &compilerAvailable, NULL);

	if(err < 0)
		devinfo.compilerAvailable = false;
	else
		devinfo.compilerAvailable = compilerAvailable;

	devinfo.type = type;

	arr.push_back(devinfo);	
}

std::string FluentCL::compilePlatformInfo(){
	std::stringstream ss;

	for(int i=0; i < clPlatforms.size(); i++){
		ss <<  clPlatforms[i].toString();
	}

	return ss.str();
}

int FluentCL::getPlatformState(){
	return platformState;
}

Context &FluentCL::getContext(){
	return context;
}

void FluentCL::clearContext(){

}

int FluentCL::createContext(ContextPolicy cp){
	bool contextReady = false;
	int rc = 0;

	if(platformState != OPENCL_PLATFORMS_AVAILABLE_STATE) return platformState;

	for(int i=0; i < clPlatforms.size(); i++){
		rc = context.newContext(cp, clPlatforms[i]);

		if(rc == SUCCESS_STATE){
			contextReady = true;
			return rc;
		}
	}

	return rc;
}

ComputeBuffer FluentCL::createReadWriteBuffer(void* data, size_t size){
	int err;
	cl_context con = context.getContext();
	cl_mem buff = clCreateBuffer(con, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, size, data, &err);

	if(err < 0){
		return ComputeBuffer();
	}else{
		return ComputeBuffer(buff, size);
	}
}
ComputeBuffer FluentCL::createReadWriteBuffer(size_t size){
	int err;
	cl_context con = context.getContext();
	cl_mem buff = clCreateBuffer(con, CL_MEM_READ_WRITE, size, NULL, &err);

	if(err < 0){
		return ComputeBuffer();
	}else{
		return ComputeBuffer(buff, size);
	}
}


ComputeBuffer FluentCL::createReadOnlyBuffer(void* data,  size_t size){
	int err;
	cl_context con = context.getContext();
	cl_mem buff = clCreateBuffer(con, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, size, data, &err);

	if(err < 0){
		return ComputeBuffer();
	}else{
		return ComputeBuffer(buff, size);
	}
}

ComputeBuffer FluentCL::createWriteOnlyBuffer(size_t size){
	int err;
	cl_context con = context.getContext();
	cl_mem buff = clCreateBuffer(con, CL_MEM_WRITE_ONLY, size, NULL, &err);

	if(err < 0){
		return ComputeBuffer();
	}else{
		return ComputeBuffer(buff, size);
	}
}

ComputeBuffer FluentCL::createReadWriteLocalBuffer(void* data, size_t size){
	int err;
	cl_context con = context.getContext();
	cl_mem buff = clCreateBuffer(con, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, size, data, &err);

	if(err < 0){
		return ComputeBuffer();
	}else{
		return ComputeBuffer(buff, size);
	}
}

ComputeBuffer FluentCL::createReadOnlyLocalBuffer(void* data,  size_t size){
	int err;
	cl_context con = context.getContext();
	cl_mem buff = clCreateBuffer(con, CL_MEM_READ_ONLY | CL_MEM_USE_HOST_PTR, size, data, &err);

	if(err < 0){
		return ComputeBuffer();
	}else{
		return ComputeBuffer(buff, size);
	}
}

#ifdef _WIN32

wchar_t *FluentCL::_charToLPCWSTR(const char* arr){

	wchar_t *wString= new wchar_t[4096];
	MultiByteToWideChar(CP_ACP, 0,  arr, -1, wString, 4096);

	return wString;
}


bool FluentCL::_fileExists(const char* path){
	
	LPCWSTR _path = _charToLPCWSTR(path);

	bool rc = PathFileExists(_path) == 0? false : true;

	return rc;
}

#endif

int FluentCL::buildProgram(const char **arr, int count, const size_t *programsize, double clversion){

	int err;

	cl_program program = clCreateProgramWithSource(context.getContext(), count, arr, programsize, &err);

	if(err < 0) {
		return BUILD_PROGRAM_FAILED_TO_LOAD_SOURCE_STATE;
	}

	cl_device_id *_ids = new cl_device_id[context.getNumCpuDevices() + context.getNumGpuDevices()];

	int counter = 0;
	for(int i=0; i < context.getNumCpuDevices(); i++){
		_ids[counter] = context.getCpuDevice(i);
		counter++;
	}

	for(int i=0; i < context.getNumGpuDevices(); i++){
		_ids[counter] = context.getGpuDevice(i);
		counter++;
	}

	stringstream arg;
	arg << "-cl-std=CL" << (float)clversion << " -cl-mad-enable";

	err = clBuildProgram(program, context.getNumCpuDevices() + context.getNumGpuDevices(), _ids, arg.str().c_str(), NULL, NULL);

	delete[] _ids;

	if(err < 0) {
		compilationOutput = buildCompilationLog(program);
		return BUILD_PROGRAM_COMPILATION_ERROR_STATE;
	}

	cl_uint kernelNum;
	clCreateKernelsInProgram(program, 0, NULL, &kernelNum);

	if(kernelNum <= 0) return BUILD_PROGRAM_NO_KERNELS_FOUND_STATE;

	cl_kernel *kernels = new cl_kernel[kernelNum];
	
	err = clCreateKernelsInProgram(program, kernelNum, kernels, NULL);

	if(err < 0){
		delete[] kernels;
		return BUILD_PROGRAM_FAILED_TO_CREATE_KERNELS_STATE;
	}

	for(unsigned int i=0; i < kernelNum; i++){
		char buffer[1000];
		clGetKernelInfo(kernels[i], CL_KERNEL_FUNCTION_NAME, 1000, buffer, NULL);

		std::string   name(buffer); 
		ComputeKernel cm(name, kernels[i]);
		kernelMap.push_back(cm);
	}

	delete[] kernels;

	return SUCCESS_STATE;
}

int FluentCL::loadKernelsFromSource(std::string path, double clversion){

#ifdef _WIN32

	if(!_fileExists(path.c_str())) return FILE_DOES_NOT_EXIST_LOAD_KERNEL_STATE;

	ifstream file;
	file.open(path);

	stringstream ss;
	
	while(!file.eof()){
		char buff[5000];
		file.getline(buff, 1000);

		ss <<  buff << endl;
	}

	file.close();

	const size_t programsize[1] = {strlen(ss.str().c_str())};
	const char **source = new const char*[1];
	source[0] = _strdup(ss.str().c_str());
	
	int rc = buildProgram(source, 1, programsize, clversion);

	free((void*)source[0]);
	delete[] source;

	return rc;
#endif

	return UNKNOWN_OPERATING_SYSTEM_STATE;
}


int FluentCL::loadKernelsFromSource(vector<std::string> paths, double clversion){
#ifdef _WIN32

	vector<std::string> snippet;

	for(int i=0; i < paths.size(); i++){
		if(!_fileExists(paths[i].c_str())) return FILE_DOES_NOT_EXIST_LOAD_KERNEL_STATE;

		stringstream ss;
		ifstream file;
		file.open(paths[i]);
	
		while(!file.eof()){
			char buff[5000];
			file.getline(buff, 1000);

			ss <<  buff << endl;
		}

		snippet.push_back(ss.str());

		file.close();
	}

	size_t snippetCount = snippet.size();
	const char **sourceBuffer = new const char*[snippetCount];
	size_t *programsize = new size_t[snippetCount];

	for(int i=0; i < snippet.size(); i++){
		sourceBuffer[i] = _strdup(snippet[i].c_str());
		programsize[i]  = strlen(snippet[i].c_str());
	}

	int rc = buildProgram(sourceBuffer,  (int)snippetCount, programsize, clversion);
	
	for(int i=0; i < snippet.size(); i++) free((void*)sourceBuffer[i]);

	delete[] sourceBuffer;
	delete[] programsize;

	return rc;
#endif

	return UNKNOWN_OPERATING_SYSTEM_STATE;
}

int loadKernelsFromSource(char **, double){
	return SUCCESS_STATE;
}

std::string FluentCL::buildCompilationLog(cl_program program){

	stringstream log;

	for(int i=0; i < context.getNumCpuDevices(); i++){
		log << context.getCpuDeviceInfo(i);
		log << endl 
			<< "//*****************************************************\n"
			<< "//Compilation Device Compilation Output\n"
			<< "//*****************************************************\n";

		size_t logSize;
		clGetProgramBuildInfo(program, context.getCpuDevice(i), CL_PROGRAM_BUILD_LOG, 0, NULL, &logSize);
		char *buffer = (char*)calloc(logSize+1, sizeof(char));
		clGetProgramBuildInfo(program, context.getCpuDevice(i), CL_PROGRAM_BUILD_LOG, logSize+1, buffer, NULL);
		
		log << buffer;
		log << "\n\n";

		free(buffer);
	}

	for(int i=0; i < context.getNumGpuDevices(); i++){
		log << context.getGpuDeviceInfo(i);
		log << endl 
			<< "//*****************************************************\n"
			<< "//Compilation Device Compilation Output\n"
			<< "//*****************************************************\n";

		size_t logSize;
		clGetProgramBuildInfo(program, context.getGpuDevice(i), CL_PROGRAM_BUILD_LOG, 0, NULL, &logSize);
		char *buffer = (char*)calloc(logSize+1, sizeof(char));
		clGetProgramBuildInfo(program, context.getGpuDevice(i), CL_PROGRAM_BUILD_LOG, logSize+1, buffer, NULL);

		log << buffer;
		log << "\n\n";

		free(buffer);
	}

	return log.str();
}

int FluentCL::loadKernelsFromSource(const char** source, int size, double clversion){
	
	size_t *programsize = new size_t[size];

	for(int i=0; i < size; i++) programsize[i] = strlen(source[i]);
	
	int rc = buildProgram(source,  size, programsize, clversion);

	delete[] programsize;

	return SUCCESS_STATE;
}

std::string FluentCL::getCompilationOutput(){
	return compilationOutput;
}

int FluentCL::executeKernel(string name, int deviceType, int deviceIndex){

	for(int i=0; i < kernelMap.size();  i++){
		string m = kernelMap[i].getName();
		if(kernelMap[i].getName().compare(name) == 0){
			int err;
			if(deviceType == OPENCL_CPU_DEVICE){ 
				err = clEnqueueTask(context.getCpuCommandQueue(deviceIndex), kernelMap[i].getKernel(), 0, NULL, NULL);
			}else if(deviceType == OPENCL_GPU_DEVICE){
				err = clEnqueueTask(context.getGpuCommandQueue(deviceIndex), kernelMap[i].getKernel(), 0, NULL, NULL);
			}else{
				return UNKNOWN_OPENCL_DEVICE_TYPE_STATE;
			}

			if(err < 0){
				return UNABLE_TO_EXECUTE_KERNEL_STATE;
			}

			return SUCCESS_STATE;
		}
	}

	return UNKNOWN_OPENCL_KERNEL_STATE;
}

int FluentCL::executeKernelParallel1D(string name, int deviceType, int deviceIndex, int workitems, int localsize, bool wait){
	size_t witems = workitems;	
	size_t litems = localsize;
	cl_event _event;

	for(int i=0; i < kernelMap.size();  i++){
		string m = kernelMap[i].getName();
		if(kernelMap[i].getName().compare(name) == 0){
			int err;
			if(deviceType == OPENCL_CPU_DEVICE){ 
				if(localsize == 0){
					err = clEnqueueNDRangeKernel(context.getCpuCommandQueue(deviceIndex), kernelMap[i].getKernel(), 1, NULL, &witems, NULL, 0, NULL,  &_event);
					
					if(err >= 0){
						if(wait){
							clFlush(context.getCpuCommandQueue(deviceIndex));
							waitAndReleaseDevice(_event);
						}
					}
				}else{
					err = clEnqueueNDRangeKernel(context.getCpuCommandQueue(deviceIndex), kernelMap[i].getKernel(), 1, NULL, &witems, &litems, 0, NULL,  &_event);
				
					if(err >= 0){
						if(wait){
							clFlush(context.getCpuCommandQueue(deviceIndex));
							waitAndReleaseDevice(_event);
						}
					}
				}

			}else if(deviceType == OPENCL_GPU_DEVICE){
				if(localsize == 0){
					err = clEnqueueNDRangeKernel(context.getGpuCommandQueue(deviceIndex), kernelMap[i].getKernel(), 1, NULL, &witems, NULL, 0, NULL,  &_event);
				
					if(err >= 0){
						if(wait){
							clFlush(context.getGpuCommandQueue(deviceIndex));
							waitAndReleaseDevice(_event);
						}
					}
				}else{
					err = clEnqueueNDRangeKernel(context.getGpuCommandQueue(deviceIndex), kernelMap[i].getKernel(), 1, NULL, &witems, &litems, 0, NULL,  &_event);

					if(err >= 0){
						if(wait){
							clFlush(context.getGpuCommandQueue(deviceIndex));
							waitAndReleaseDevice(_event);
						}
					}
				}
			}else{
				return UNKNOWN_OPENCL_DEVICE_TYPE_STATE;
			}

			if(err < 0){
				return UNABLE_TO_EXECUTE_KERNEL_STATE;
			}

			return SUCCESS_STATE;
		}
	}

	return UNKNOWN_OPENCL_KERNEL_STATE;
}

int FluentCL::executeKernelParallel2D(string name, int deviceType, int deviceIndex, size_t *workitems, size_t *localsize, bool wait){
	cl_event _event;

	for(int i=0; i < kernelMap.size();  i++){
		string m = kernelMap[i].getName();
		if(kernelMap[i].getName().compare(name) == 0){
			int err;
			if(deviceType == OPENCL_CPU_DEVICE){ 
					err = clEnqueueNDRangeKernel(context.getCpuCommandQueue(deviceIndex), kernelMap[i].getKernel(), 2, NULL, workitems, localsize, 0, NULL, &_event);

					if(err >= 0){
						if(wait){
							clFlush(context.getCpuCommandQueue(deviceIndex));
							waitAndReleaseDevice(_event);
						}
					}

			}else if(deviceType == OPENCL_GPU_DEVICE){
					err = clEnqueueNDRangeKernel(context.getGpuCommandQueue(deviceIndex), kernelMap[i].getKernel(), 2, NULL, workitems, localsize, 0, NULL, &_event);

					if(err >= 0){
						if(wait){
							clFlush(context.getGpuCommandQueue(deviceIndex));
							waitAndReleaseDevice(_event);
						}
					}
					
			}else{
				return UNKNOWN_OPENCL_DEVICE_TYPE_STATE;
			}

			if(err < 0){
				cout << "Error: " << err << endl;
				return UNABLE_TO_EXECUTE_KERNEL_STATE;
			}

			return SUCCESS_STATE;
		}
	}

	return UNKNOWN_OPENCL_KERNEL_STATE;
}

int FluentCL::setKernelArgument(string name, cl_int inx, ComputeBuffer cb){
	
	for(int i=0; i < kernelMap.size();  i++){

		if(kernelMap[i].getName().compare(name) == 0){
			cl_mem mem  = cb.getBuffer();
			size_t size = cb.getSize();

			int err = clSetKernelArg( kernelMap[i].getKernel(), inx, size, &mem);

			if(err < 0){
				return UNABLE_TO_SET_KERNEL_ARGUMENT_STATE;
			}

			return SUCCESS_STATE;
		}
	}

	return UNKNOWN_OPENCL_KERNEL_STATE;
}

int FluentCL::setKernelArgument(string name, cl_int inx, size_t size, void* data){
	
	for(int i=0; i < kernelMap.size();  i++){

		if(kernelMap[i].getName().compare(name) == 0){

			int err = clSetKernelArg( kernelMap[i].getKernel(), inx, size, data );

			if(err < 0){
				return UNABLE_TO_SET_KERNEL_ARGUMENT_STATE;
			}

			return SUCCESS_STATE;
		}
	}

	return UNKNOWN_OPENCL_KERNEL_STATE;
}

int FluentCL::readComputeBuffer(int deviceType, int deviceIndex, ComputeBuffer cb, size_t size, void *data){
	int err;

	cl_mem mem = cb.getBuffer();
	if(deviceType == OPENCL_CPU_DEVICE){ 
		err = clEnqueueReadBuffer(context.getCpuCommandQueue(deviceIndex), mem, CL_TRUE, 0, size, data, 0, NULL, NULL);
	}else if(deviceType == OPENCL_GPU_DEVICE){
		err = clEnqueueReadBuffer(context.getGpuCommandQueue(deviceIndex), mem, CL_TRUE, 0, size, data, 0, NULL, NULL);
	}else{
		return UNKNOWN_OPENCL_DEVICE_TYPE_STATE;
	}

	if(err < 0){
		return UNABLE_TO_READ_COMPUTE_BUFFER_STATE;
	}

	return SUCCESS_STATE;
}

int FluentCL::mapReadComputeBuffer(int deviceType, int deviceIndex, ComputeBuffer cb, size_t size, void *data){
	int err;

	cl_mem mem = cb.getBuffer();
	if(deviceType == OPENCL_CPU_DEVICE){ 

		void *mapped_memory = clEnqueueMapBuffer(context.getCpuCommandQueue(deviceIndex), mem, CL_TRUE, CL_MAP_READ, 0, size, 0, NULL, NULL, &err);
		memcpy(data, mapped_memory, size);
		err = clEnqueueUnmapMemObject(context.getCpuCommandQueue(deviceIndex), mem, mapped_memory, 0, NULL, NULL);
	
	}else if(deviceType == OPENCL_GPU_DEVICE){

		void *mapped_memory = clEnqueueMapBuffer(context.getGpuCommandQueue(deviceIndex), mem, CL_TRUE, CL_MAP_READ, 0, size, 0, NULL, NULL, &err);
		memcpy(data, mapped_memory, size);
		err = clEnqueueUnmapMemObject(context.getGpuCommandQueue(deviceIndex), mem, mapped_memory, 0, NULL, NULL);
	}else{
		return UNKNOWN_OPENCL_DEVICE_TYPE_STATE;
	}

	if(err < 0){
		return UNABLE_TO_READ_COMPUTE_BUFFER_STATE;
	}

	return SUCCESS_STATE;
}

int FluentCL::writeToComputeBuffer(int, int, ComputeBuffer){
	return SUCCESS_STATE;
}

void FluentCL::waitAndReleaseDevice(cl_event _event){
    cl_int eventStatus = CL_QUEUED;
    while(eventStatus != CL_COMPLETE) {
        clGetEventInfo(_event, CL_EVENT_COMMAND_EXECUTION_STATUS, sizeof(cl_int), &eventStatus, NULL);
    }
}