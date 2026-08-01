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

#include "PlatformState.h"

//#define POLICY_MIN_CL_VERSION_FAILED_STATE			0x20015

std::string StateInfo::info(int code){
	switch (code)
	{
	case 0x10001:
		return std::string("No OpenCL platforms available on this device.");
	case 0x10002:
		return std::string("OpenCL platforms are available on this device.");
	case 0x10003:
		return std::string("OpenCL platforms detected, but failed to retrieve them.");
	case 0x10004:
		return std::string("OpenCL platforms detected, but no OpenCL capable CPU or GPU devices detected.");
	case 0x10005:
		return std::string("OpenCL devices found on device.");
	case 0x10006:
		return std::string("Failed to create context, no platform found fit the policy.");
	case 0x10007:
		return std::string("Failed to create context, OpenCl error.");
	case 0x10008:
		return std::string("Unkown Operating System.");
	case 0x10009:
		return std::string("Unable to load kernel, file does not exist.");
	case 0x10010:
		return std::string("Build program failed to load source.");
	case 0x10011:
		return std::string("Build program failed to compile source.");
	case 0x10012:
		return std::string("Build program failed to find kernels.");
	case 0x10013:
		return std::string("Build program failed to create kernels.");
	case 0x10014:
		return std::string("Failed to run OpenCL kernel.");
	case 0x10015:
		return std::string("Unkown OpenCl device type used.");
	case 0x10016:
		return std::string("Unkown Kernel name used.");
	case 0x10017:
		return std::string("Unable to set kernel argument.");
	case 0x10018:
		return std::string("Unable to load data to from compute device buffer.");
	case 0x20000:
		return std::string("Failed policy: failed to find devices.");
	case 0x20001:
		return std::string("Failed policy: Couldn't find CPU device.");
	case 0x20002:
		return std::string("Failed policy: Couldn't find GPU device.");
	case 0x20003:
		return std::string("Failed policy: Couldn't find devices.");
	case 0x20004:
		return std::string("Failed policy: No OpenCL compiler found on device.");
	case 0x20005:
		return std::string("Failed policy: Device doesn't have minimum OpenCL.");
	case 0x20007:
		return std::string("Failed policy: Device doesn't have minimum global memory size requirement.");
	case 0x20008:
		return std::string("Failed policy: Device doesn't have maximum global memory size requirement.");
	case 0x20009:
		return std::string("Failed policy: Device doesn't have minimum address bits requirement.");
	case 0x20010:
		return std::string("Failed policy: Device doesn't have maximum address bits requirement.");
	case 0x20011:
		return std::string("Failed policy: Couldn't meet minimum CPU count requirement.");
	case 0x20012:
		return std::string("Failed policy: Couldn't meet maximum CPU count requirement.");
	case 0x20014:
		return std::string("Failed policy: Couldn't meet minimum GPU count requirement.");
	case 0x20015:
		return std::string("Failed policy: Device doesn't have minimum OpenCL version.");
	default:
		return "<< NO STATE >>";
		break;
	}
}