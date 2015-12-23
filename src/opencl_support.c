#include "CL/cl.h"
#include "opencl_support.h"

void openCL_configureDevices(OpenCL_Data *device)
{
	cl_int error_code;
	cl_int ret;
	
	if (error_code != CL_SUCCESS)
		printf("Error. Could not get platform IDs.");
	else {
		//for each platform, detect the number of devices available
		
	}
}

//This will detect all platforms and display information about them
void opencl_queryPlatforms()
{
	//Query all available platforms
	cl_int err;
	cl_uint ret, iterator;
	cl_platform_id platformIDs[20];
	

	size_t buffer_used;
	char buffer[1024]; //allocate a kilobyte of information buffer to place parameter queries into

	err = clGetPlatformIDs(20, platformIDs, &ret);
	printf("Found %d platforms. ", ret);

	for (iterator = 0; iterator < ret; iterator++)
	{
		printf("\n\nPlatform %d:\n\n", iterator);
		//query the information about the platform
		clGetPlatformInfo(platformIDs[iterator], CL_PLATFORM_NAME, sizeof(buffer), buffer, &buffer_used);
		printf("Name: \t%.*s\n", buffer_used, buffer);
		clGetPlatformInfo(platformIDs[iterator], CL_PLATFORM_VENDOR, sizeof(buffer), buffer, &buffer_used);
		printf("Vendor: \t%.*s\n", buffer_used, buffer);
		clGetPlatformInfo(platformIDs[iterator], CL_PLATFORM_VERSION, sizeof(buffer), buffer, &buffer_used);
		printf("Version: \t%.*s\n", buffer_used, buffer);
		clGetPlatformInfo(platformIDs[iterator], CL_PLATFORM_EXTENSIONS, sizeof(buffer), buffer, &buffer_used);
		printf("Extensions: \t%.*s\n", buffer_used, buffer);
		clGetPlatformInfo(platformIDs[iterator], CL_PLATFORM_PROFILE, sizeof(buffer), buffer, &buffer_used);
		printf("Profile: \t%.*s\n", buffer_used, buffer);
		printf("\n\n---------------");
		
		//Query all available devices on the specified platform	
		opencl_queryDevices(platformIDs[iterator]);
	}

}

//This will detect any devices on a platform and display information about the devices
void opencl_queryDevices(cl_platform_id platform)
{
	cl_device_id deviceIDs[20];
	cl_uint ret, iterator;
	cl_int err;

	char buffer[1024]; //allocate 1KB of data for the buffer space so we can query the devices
	size_t buffer_used;
	//Get all the available devices on the platform
	err = clGetDeviceIDs(platform, CL_DEVICE_TYPE_ALL, 20, deviceIDs, &ret);
	printf("\nFound %d devices. \n", ret);
	printf("---------------\n\n");
	
if (err != CL_SUCCESS)
		printf("Error. Could not get device IDs. Error code: %d", err);
	else
	{
		for (iterator = 0; iterator < ret; iterator++)
		{
			printf("\nDevice %d: \n", iterator);
			clGetDeviceInfo(deviceIDs[iterator], CL_DEVICE_NAME, sizeof(buffer), buffer, &buffer_used);
			printf("Name: \t%.*s\n", buffer_used, buffer);
			clGetDeviceInfo(deviceIDs[iterator], CL_DEVICE_VENDOR, sizeof(buffer), buffer, &buffer_used);
			printf("Vendor: \t%.*s\n", buffer_used, buffer);
			clGetDeviceInfo(deviceIDs[iterator], CL_DEVICE_VENDOR_ID, sizeof(buffer), buffer, &buffer_used);
			printf("Vendor ID: \t%d\n", (cl_uint *)buffer[0]);
			clGetDeviceInfo(deviceIDs[iterator], CL_DEVICE_VERSION, sizeof(buffer), buffer, &buffer_used);
			printf("Device Version: \t%.*s\n", buffer_used, buffer);
			clGetDeviceInfo(deviceIDs[iterator], CL_DEVICE_AVAILABLE, sizeof(buffer), buffer, &buffer_used);
			printf("Device Available: \t%d\n", (int *)buffer[0]);
			clGetDeviceInfo(deviceIDs[iterator], CL_DEVICE_EXTENSIONS, sizeof(buffer), buffer, &buffer_used);
		//	printf("Extensions: \t%.*s\n", buffer_used, buffer);
		//	clGetDeviceInfo(deviceIDs[iterator], CL_DEVICE_GLOBAL_MEM_CACHE_SIZE, sizeof(buffer), buffer, &buffer_used);
			printf("Global Memory Cache (bytes): \t%d\n", (cl_ulong *)buffer[0]);
			clGetDeviceInfo(deviceIDs[iterator], CL_DEVICE_GLOBAL_MEM_SIZE, sizeof(buffer), buffer, &buffer_used);
			printf("Global Memory Size (bytes): \t%d\n", (cl_ulong *)buffer[0]);
			clGetDeviceInfo(deviceIDs[iterator], CL_DEVICE_MAX_MEM_ALLOC_SIZE, sizeof(buffer), buffer, &buffer_used);
			printf("Maximum Memory Alloc (bytes): \t%d\n", (cl_ulong *)buffer[0]);
			clGetDeviceInfo(deviceIDs[iterator], CL_DEVICE_MAX_CLOCK_FREQUENCY, sizeof(buffer), buffer, &buffer_used);
			printf("Maximum Clock (MHz): \t%d\n", (cl_uint *)buffer[0]);
			clGetDeviceInfo(deviceIDs[iterator], CL_DEVICE_MAX_COMPUTE_UNITS, sizeof(buffer), buffer, &buffer_used);
			printf("Max Compute Units: \t%d\n", (cl_uint *)buffer[0]);
			clGetDeviceInfo(deviceIDs[iterator], CL_DEVICE_MAX_PARAMETER_SIZE, sizeof(buffer), buffer, &buffer_used);
			printf("Maximum Parameter Size (bytes): \t%d\n", (size_t *)buffer[0]);
			clGetDeviceInfo(deviceIDs[iterator], CL_DEVICE_LOCAL_MEM_SIZE, sizeof(buffer), buffer, &buffer_used);
			printf("Maximum Local Memory Size (bytes): \t%d\n", (cl_ulong *)buffer[0]);
			clGetDeviceInfo(deviceIDs[iterator], CL_DEVICE_LOCAL_MEM_TYPE, sizeof(buffer), buffer, &buffer_used);
			printf("Local Memory Tepe: \t%.*s\n", (cl_device_local_mem_type *)buffer[0]);
			clGetDeviceInfo(deviceIDs[iterator], CL_DEVICE_MAX_WORK_GROUP_SIZE, sizeof(buffer), buffer, &buffer_used);
			printf("Maximum Work Group Size: \t%d\n", (size_t *)buffer[0]);
			clGetDeviceInfo(deviceIDs[iterator], CL_DEVICE_MAX_CONSTANT_BUFFER_SIZE, sizeof(buffer), buffer, &buffer_used);
			printf("Maximum Constant Buffer Size (bytes): \t%d\n", (cl_ulong *) buffer[0]);
	
			printf("\n~~~\n");
		} 
	}
}


int main()
{
	opencl_queryPlatforms();
}
