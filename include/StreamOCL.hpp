/*
    StreamOCL - GNU GPL Copyright Notice
    Copyright (C) 2015  Ryan Summers

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along
    with this program; if not, write to the Free Software Foundation, Inc.,
    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/




//This is the class definition of OpenCL_Support.
//This class provides easy functionality and managing of OpenCL Parameters.

#include <iostream>
#include <CL/cl.h> //The main OpenCL Header file
#include <vector>
#include <stdio.h>
#include <stdint.h>
#include <cstring>


typedef enum {
	INPUT,
	OUTPUT,
	INOUT
} OpenCL_IO;

typedef enum {
	GLOBAL,
	LOCAL,
	PRIVATE
} OpenCL_MemType;

typedef struct {
	int argumentIndex;
	void *argument;
	size_t argumentSize;
	OpenCL_IO io;
	OpenCL_MemType memType;
	cl_mem buffer;
} OpenCL_Argument;

using std::string;
using std::cout;
using std::endl;
using std::vector;

class OpenCL_Data {
	public:
		OpenCL_Data(bool timeInfo = false); 					//Default Constructor
		OpenCL_Data(OpenCL_Data &data); //Copy Constructor
		~OpenCL_Data(); 				//Destructor

		//Additional Functionality
		void queryPlatforms();			//Query all available platforms
		void queryDevices(); 			//Query all Devices on specified platform
		void queryAllDevices();			//Query information on all available devices on all available platforms 
		void queryMemoryInfo(); 		//Query Memory Information on the object's deviceID
		void setPlatformID(int index);	//This will set the platform ID to the index specified
		
		//These functions will return true if they succeed
		bool setDeviceID(int index);	//This will set the device ID to the index specified
		bool setDeviceID(char *name);	//This will set the device ID to the device id with the specified name

		int initialize(); 				//This function creates the command queue and context for the specified device ID
		int start(size_t *globalWorkSize, size_t *localWorkSize, int dimensions, bool blocking = false);					//Start program execution on the device
										//This will also create all buffers on the device for use.
		int setProgram(string kernelFileName, string functionName, bool debug); //This function will set the openCL program from the file specified
		void setKernelArgument(int argIndex, void *argument, size_t argumentSize, OpenCL_IO io, OpenCL_MemType memType); //Set a Kernel Argument
		void setKernelArgument(OpenCL_Argument arg); 	//Set the Argument specified by Index
		void updateKernelArgument(int argIndex, void *argument, size_t argumentSize, OpenCL_IO io, OpenCL_MemType memType);	//Update the Argument specified by argIndex
		void updateKernelArgument(OpenCL_Argument arg);		//Update the Argument specified by Index
		void removeKernelArgument(int argIndex);			//remove the specified Argument
		OpenCL_Argument getKernelArgument(int argIndex); 	//Get the specified Argument
		int writeBuffers();			//Write all Kernel Argument Buffers to the device
		int readResults();				//Read results from the GPU into the specified IO buffer
		int initializeBuffers();		//Initialize the OpenCL Buffers specified by Arguments
		uint64_t getMaximumMemorySize();		//Get the maximum memory size of the openCL device	
	private:

		//OpenCL Specific Members
		cl_platform_id platformID; //ID of a platform
		cl_device_id deviceID; //ID of a device specific to the platform ID
		cl_context context; //The OpenCL Context for the device
		cl_command_queue commandQueue; //The command queue for the assocaited device
		cl_program program; //The program that the OpenCL Device will run
		cl_kernel kernel; //The kernel of the OpenCL Device currently being run
		size_t localMemorySize;
		bool printTimeInfo;
	
		//Members to expand upon class functionality
		vector<OpenCL_Argument> arguments; //Vector of Argument Parameters to store argument information
};
