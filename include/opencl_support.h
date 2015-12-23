
//This header is created with the intent that these support functions will help
//To query OpenCL data and parameters as well as set up OpenCL devices and input parameters to avoid the lengthy processing within the library itself

#include <CL/cl.h>
#include <stdio.h>

typedef struct OPENCL_DATA {
	cl_context context;
	cl_command_queue commandQueue;
	cl_platform_id deviceID[5];
	cl_program program;
	cl_kernel kernel;
} OpenCL_Data;

void opencl_configureDevices(OpenCL_Data *device);
void opencl_queryPlatforms();
void opencl_queryDevices(cl_platform_id platform);
