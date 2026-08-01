#include <iostream>
#include "PlatformState.h"
#include "FluentCL.h"

using namespace std;

typedef struct st_Simple{
	cl_int a;
	cl_float b;
}Simple;

void main(){
	int rc = 0;

	FluentCL flcl;
	
	ContextPolicy cpolicy;
	cpolicy.includeCpuDevices = true;
	cpolicy.includeGpuDevices = true;
	cpolicy.useMinClVersion   = true;
	cpolicy.minCpuDeviceCount = 1;
	cpolicy.maxCpuDeviceCount = 1;
	cpolicy.minGpuDeviceCount = 1;
	cpolicy.maxGpuDeviceCount = 1;
	cpolicy.minClVersion      = 1.19f;

	rc =  flcl.createContext(cpolicy);

	if(rc != SUCCESS_STATE){
		cout << StateInfo::info(rc) << endl;
		exit(1);
	}
	
	vector<string> paths;
	paths.push_back("C:\\opencl\\kernels\\mat_vec.cl");
	paths.push_back("C:\\opencl\\kernels\\sph_kernels.cl");

	cout << "Building OpenCL source..." << endl; 
	rc = flcl.loadKernelsFromSource("C:\\opencl\\kernels\\array_mult.cl", OPEN_CL_VER_1_2);

	if(rc != SUCCESS_STATE){
		if( rc == BUILD_PROGRAM_COMPILATION_ERROR_STATE) cout << flcl.getCompilationOutput();
		else cout << StateInfo::info(rc) << endl;
		exit(1);
	}

	Simple sim;
	sim.a = 10;
	sim.b = 20.f;

	int numbers[2] = {2,2};
	ComputeBuffer c_numbers = flcl.createReadOnlyBuffer(numbers, sizeof(int)*2);
	if(c_numbers.isNull()) cout <<  "1: Failed to create numbers buffers." << endl;

	int results[2] = {0,0};
	ComputeBuffer c_results = flcl.createWriteOnlyBuffer(sizeof(int)*2);
	if(c_results.isNull()) cout << "2: Failed to create results buffers." << StateInfo::info(rc)<< endl;

	rc = flcl.setKernelArgument("array_mult", 0, sizeof(Simple), &sim);
	if(rc != SUCCESS_STATE) cout << "3: " << StateInfo::info(rc) << endl;

	rc = flcl.setKernelArgument("array_mult", 1, c_numbers);
	if(rc != SUCCESS_STATE) cout << "4: " << StateInfo::info(rc) << endl;

	rc = flcl.setKernelArgument("array_mult", 2, c_results);
	if(rc != SUCCESS_STATE) cout << "5: " << StateInfo::info(rc) << endl;

	flcl.executeKernel("array_mult", OPENCL_GPU_DEVICE, 0);
	if(rc != SUCCESS_STATE) cout << "6: " << StateInfo::info(rc) << endl;
	
	flcl.readComputeBuffer(OPENCL_GPU_DEVICE, 0, c_results, sizeof(int)*2, results);
	for(int i=0;  i < 2; i++) cout << "Result: " << results[i] << endl;
 
	//for(int i=0;  i < 51; i++) cout << "Result: " << results[i] << endl;
	//flcl.executeKernelParallel1D("array_mult", OPENCL_GPU_DEVICE, 0, ITEM_COUNT);
	//if(rc != SUCCESS_STATE) cout << "4: " << StateInfo::info(rc) << endl;

	//int ITEM_COUNT = 10000000;
	////int numbers[10]     = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
	//int *numbers = new int[ITEM_COUNT];
	//for(int i=0; i < ITEM_COUNT; i++) numbers[i] = i;
	//ComputeBuffer c_numbers = flcl.createReadOnlyBuffer(numbers, sizeof(int)*ITEM_COUNT);
	//if(c_numbers.isNull()) cout <<  "1: Failed to create numbers buffers." << endl;

	////int coeficients[10] = { 2, 2, 2, 2, 2, 2, 2, 2, 2, 2 };
	//int *coeficients = new int[ITEM_COUNT];
	//for(int i=0; i < ITEM_COUNT; i++) coeficients[i] = 2;
	//ComputeBuffer c_coeficients = flcl.createReadOnlyBuffer(coeficients, sizeof(int)*ITEM_COUNT);
	//if(c_coeficients.isNull()) cout << "2: Failed to create coeficient buffers." << StateInfo::info(rc)<< endl;

	////int results[10] = { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };
	//int *results = new int[ITEM_COUNT];
	//for(int i=0; i < ITEM_COUNT; i++) results[i] = 0;
	//ComputeBuffer c_results = flcl.createWriteOnlyBuffer(sizeof(int)*ITEM_COUNT);
	//if(c_results.isNull()) cout << "3: Failed to create results buffers." << StateInfo::info(rc)<< endl;

	//rc = flcl.setKernelArgument("array_mult", 0, c_numbers);
	//if(rc != SUCCESS_STATE) cout << "1: " << StateInfo::info(rc)<< endl;

	//rc = flcl.setKernelArgument("array_mult", 1, c_coeficients);
	//if(rc != SUCCESS_STATE) cout << "2: " << StateInfo::info(rc) << endl;
	//
	//rc = flcl.setKernelArgument("array_mult", 2, c_results);
	//if(rc != SUCCESS_STATE) cout << "3: " << StateInfo::info(rc) << endl;

	//flcl.executeKernelParallel1D("array_mult", OPENCL_GPU_DEVICE, 0, ITEM_COUNT);
	//if(rc != SUCCESS_STATE) cout << "4: " << StateInfo::info(rc) << endl;
	//
	//flcl.readComputeBuffer(OPENCL_GPU_DEVICE, 0, c_results, sizeof(int)*ITEM_COUNT, results);
 //  
	//for(int i=0;  i < 51; i++) cout << "Result: " << results[i] << endl;
}