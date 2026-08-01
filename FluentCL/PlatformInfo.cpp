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

#include <sstream>
#include "PlatformInfo.h"

std::string DeviceInfo::toString(){
	std::string avail     = this->available ? "Yes": "No";
	std::string compAvail = this->compilerAvailable ? "Yes": "No";
	
	std::stringstream ss;
	ss  <<"---------------"<<"OpenCl Device Infomation (id=" << this->id << ").\n"
		<<"\t" <<"Device Name:  " << this->name << std::endl
		<<"\t" <<"Device Vendor:  " << this->vendor << std::endl
		<<"\t" <<"Device Address Bits: [ " << this->addressBits << " ]\n"
		<<"\t" <<"Device Global Mem Size: [ " << this->globalMemorySize << " ]\n"
		<<"\t" <<"Device Available: " << avail << "\n"
		<<"\t" <<"Device Compiler Available: " << compAvail << "\n"
		<<"\t" <<"Device Type: " <<  this->type << "\n\n";

	return ss.str();
}

std::string PlatformInfo::toString(){
	std::stringstream ss;

	ss  <<"----------------------------------------------\n"
	    <<"--------OpenCl Platform Information-----------\n"
		<<"----------------------------------------------\n\n"
	
        << "\t" << "Platform Name: "    << this->name    <<"\n"
		<< "\t" << "Platform Vendor: "  << this->vendor  <<"\n"
		<< "\t" << "Platform Version: " << this->version <<"\n"
		<< "\t" << "Platform Profile: " << this->profile <<"\n"
		<< "\t" << "Platform Extensions: " << this->extensions <<"\n\n";

	for(unsigned int i=0; i < this->numCpuDevices; i++) ss << this->cpuDevices[i].toString();
	for(unsigned int i=0; i < this->numGpuDevices; i++) ss << this->gpuDevices[i].toString();

	return ss.str();
}

