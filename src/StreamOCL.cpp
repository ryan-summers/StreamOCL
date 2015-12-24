#include "StreamOCL.hpp"
//Default constructor for the OpenCL Class
OpenCL_Data::OpenCL_Data() 
{
	platformID = NULL;
	deviceID = NULL;
	context = NULL;
	commandQueue = NULL;
	program = NULL;
	kernel = NULL;

}	

//Default copy constructor for the OpenCL_Data class
OpenCL_Data::OpenCL_Data(OpenCL_Data &data)
{
	this->platformID = data.platformID;
	this->deviceID = data.deviceID;
	this->context = data.context;
	this->commandQueue = data.commandQueue;
	this->program = data.program;
	this->kernel = data.kernel;
}

//Default destructor for the OpenCL_Data class
OpenCL_Data::~OpenCL_Data()
{
	cl_int ret;
	//Force command queue to finish
	if (this->commandQueue)
	{	
		ret = clFlush(this->commandQueue);
		if (ret != CL_SUCCESS)
			cout << "Error. Could not flush command queue. CL Error: " << ret << endl;
		ret =clFinish(this->commandQueue);
			cout << "Error. Could not finish command queue. CL Error: " << ret << endl;
	}

	if (this->kernel)
	{
		ret = clReleaseKernel(this->kernel);
			if (ret != CL_SUCCESS)
				cout << "Error. Could not release kernel. CL Error: " << ret << endl;
	}

	if (this->program)
	{
		ret = clReleaseProgram(this->program);
		if (ret != CL_SUCCESS)
			cout << "Error. Could not release the program. CL Error: " << ret << endl;
	}

	if (this->commandQueue)
	{
		ret = clReleaseCommandQueue(this->commandQueue);
		if (ret != CL_SUCCESS)
			cout << "Error. Could not release the command queue. CL Error: " << ret << endl;

	}

	if (this->context)
	{
		ret = clReleaseContext(this->context);
		if (ret != CL_SUCCESS)
			cout << "Error. Failed to release the context. CL Error: " << ret << endl;
	}

	//Free all the memory buffers for OpenCL
	for (int i = 0; i < this->arguments.size(); i++)
	{
		if (this->arguments.at(i).buffer != NULL)
		{
			ret = clReleaseMemObject(this->arguments.at(i).buffer);
			if (ret != CL_SUCCESS)
				cout << "Error. Could not release memory object for argument " << i << ". CL Error: " << ret << endl;
		}
	}
}	


//Additional Functionality Functions

//Query available OpenCL Platforms
void OpenCL_Data::queryPlatforms()
{
	//OpenCL Parameters
	cl_int err;
	cl_uint ret;
	cl_platform_id platformIDs[20];

	//Buffer Information
	size_t buffer_used;
	char buffer[1024]; //allocate a kilobyte of information buffer to place parameter queries into

	//Get the Platform IDs
	err = clGetPlatformIDs(20, platformIDs, &ret);
	
	
	//Print out Platform info
	cout << "Found " << (int)ret << " platforms!" << endl << "-------------------" << endl;
	for (int i = 0; i < ret; i++)
	{
		cout << endl << endl << " Platform " << i << endl;
		cout << " -----------" << endl << endl;
		//query the information about the platform
		clGetPlatformInfo(platformIDs[i], CL_PLATFORM_NAME, sizeof(buffer), buffer, &buffer_used);
		cout << "Name: \t\t" << buffer << endl;
		clGetPlatformInfo(platformIDs[i], CL_PLATFORM_VENDOR, sizeof(buffer), buffer, &buffer_used);
		cout << "Vendor: \t" << buffer << endl;
		clGetPlatformInfo(platformIDs[i], CL_PLATFORM_VERSION, sizeof(buffer), buffer, &buffer_used);
		cout << "Version: \t" << buffer << endl;
		clGetPlatformInfo(platformIDs[i], CL_PLATFORM_EXTENSIONS, sizeof(buffer), buffer, &buffer_used);
		cout << "Extensions: \t" << buffer << endl;
		clGetPlatformInfo(platformIDs[i], CL_PLATFORM_PROFILE, sizeof(buffer), buffer, &buffer_used);
		cout << "Profile: \t" << buffer << endl;
		cout << endl << "---------------" << endl << endl;
	}
}	

//Query available OpenCL Devices on the platform set within the object
void OpenCL_Data::queryDevices() 
{
	cl_device_id deviceIDs[20];
	cl_uint ret, iterator;
	cl_int err;

	char buffer[1024]; //allocate 1KB of data for the buffer space so we can query the devices
	size_t buffer_used;
	
	//Get all the available devices on the platform
	err = clGetDeviceIDs(this->platformID, CL_DEVICE_TYPE_ALL, 20, deviceIDs, &ret);
	printf("\nFound %d devices. \n", ret);
	printf("----------------\n\n");
	
	if (err != CL_SUCCESS)
		printf("Error. Could not get device IDs. Error code: %d", err);
	else
	{
		for (iterator = 0; iterator < ret; iterator++)
		{
			printf("\nDevice %d: \n", iterator);
			cout << "---------" << endl << endl;
			clGetDeviceInfo(deviceIDs[iterator], CL_DEVICE_NAME, sizeof(buffer), buffer, &buffer_used);
			printf("Name: \t\t\t%.*s\n", buffer_used, buffer);
			clGetDeviceInfo(deviceIDs[iterator], CL_DEVICE_VENDOR, sizeof(buffer), buffer, &buffer_used);
			printf("Vendor: \t\t%.*s\n", buffer_used, buffer);
			clGetDeviceInfo(deviceIDs[iterator], CL_DEVICE_VENDOR_ID, sizeof(buffer), buffer, &buffer_used);
			printf("Vendor ID: \t\t%d\n", ((cl_uint *)buffer)[0]);
			clGetDeviceInfo(deviceIDs[iterator], CL_DEVICE_VERSION, sizeof(buffer), buffer, &buffer_used);
			printf("Device Version: \t%.*s\n", buffer_used, buffer);
			clGetDeviceInfo(deviceIDs[iterator], CL_DEVICE_AVAILABLE, sizeof(buffer), buffer, &buffer_used);
			printf("Device Available: \t%d\n", ((int *)buffer)[0]);
			clGetDeviceInfo(deviceIDs[iterator], CL_DEVICE_MAX_CLOCK_FREQUENCY, sizeof(buffer), buffer, &buffer_used);
			printf("Maximum Clock (MHz): \t%d\n", ((cl_uint *)buffer)[0]);
			clGetDeviceInfo(deviceIDs[iterator], CL_DEVICE_MAX_COMPUTE_UNITS, sizeof(buffer), buffer, &buffer_used);
			printf("Max Compute Units: \t%d\n", ((cl_uint *)buffer)[0]);
			clGetDeviceInfo(deviceIDs[iterator], CL_DEVICE_MAX_WORK_GROUP_SIZE, sizeof(buffer), buffer, &buffer_used);
			printf("Max Work Group Size: \t%d\n", ((size_t *)buffer)[0]);
			
			//Memory info
			cout << endl << "Memory Information: (All sizes in Bytes): " << endl;
			cout << "----------------------------------------" << endl;
			clGetDeviceInfo(deviceIDs[iterator], CL_DEVICE_GLOBAL_MEM_SIZE, sizeof(buffer), buffer, &buffer_used);
			printf("Global Size: \t\t%d\n", ((cl_ulong *)buffer)[0]);
			clGetDeviceInfo(deviceIDs[iterator], CL_DEVICE_GLOBAL_MEM_CACHE_SIZE, sizeof(buffer), buffer, &buffer_used);
			printf("Global Cache Size: \t%d\n", ((cl_ulong *)buffer)[0]);
			clGetDeviceInfo(deviceIDs[iterator], CL_DEVICE_MAX_MEM_ALLOC_SIZE, sizeof(buffer), buffer, &buffer_used);
			printf("Max Allocation: \t%d\n", ((unsigned long *)buffer)[0]);
			clGetDeviceInfo(deviceIDs[iterator], CL_DEVICE_MAX_PARAMETER_SIZE, sizeof(buffer), buffer, &buffer_used);
			printf("Maxi Param Size: \t%d\n", ((size_t *)buffer)[0]);
			clGetDeviceInfo(deviceIDs[iterator], CL_DEVICE_LOCAL_MEM_SIZE, sizeof(buffer), buffer, &buffer_used);
			printf("Local Size: \t\t%d\n", ((cl_ulong *)buffer)[0]);
			clGetDeviceInfo(deviceIDs[iterator], CL_DEVICE_LOCAL_MEM_TYPE, sizeof(buffer), buffer, &buffer_used);
			printf("Local Type: \t\t%d\n", ((cl_device_local_mem_type *)buffer)[0]);
			clGetDeviceInfo(deviceIDs[iterator], CL_DEVICE_MAX_CONSTANT_BUFFER_SIZE, sizeof(buffer), buffer, &buffer_used);
			printf("Max Const Buffer: \t%d\n\n", ((cl_ulong *) buffer)[0]);
		} 
	}

}	

//This function sets the platform ID
void OpenCL_Data::setPlatformID(int index)
{
	cl_int error;
	cl_uint ret;
	cl_platform_id platformIDs[20];

	//Get all the platforms
	error = clGetPlatformIDs(20, platformIDs, &ret);

	if (error != CL_SUCCESS) 
	{
		cout << "Error. Could not get platform IDs. CL Error code: " << error << endl;
	} 
	else
	{
		//Ensure index is within bounds
		if (ret > index)
		{
			//Set the Platform ID
			this->platformID = platformIDs[index];
		}
		else
		{
			cout << "Error! Attempting to specify non-existant platform." << endl;
		}
	}
}

//This function sets the device ID
void OpenCL_Data::setDeviceID(int index)
{
	cl_int error;
	cl_uint num_devices;
	cl_device_id deviceIDs[20];

	error = clGetDeviceIDs(this->platformID, CL_DEVICE_TYPE_ALL, 20, deviceIDs, &num_devices);

	if (error != CL_SUCCESS)
	{
		cout << "Error. Could not get device IDs. CL Error Code: " << error << endl;
		cout << "Have you set the platform yet?" << endl;
	}
	else
	{
		//Check bounds
		if (num_devices > index)
			this->deviceID = deviceIDs[index];
		else
		{
			cout << "Error! Attempting to specify non-existant device." << endl;
			cout << "The Index specified exceeds the number of available devices on the platform." << endl;
		}
	}
}

void OpenCL_Data::queryAllDevices()
{
	//Create a temporary variable to hold the platform ID
	cl_platform_id holder = this->platformID;
	
	cl_int error;
	cl_uint ret;
	cl_platform_id platformIDs[20];

	//Get all the platforms
	error = clGetPlatformIDs(20, platformIDs, &ret);

	for (int i = 0; i < (int)ret; i++)
	{
		//Set the platform ID
		this->platformID = platformIDs[i];
		cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
		cout << "Querying devices for platform " << i << "." << endl;
		cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
		this->queryDevices();
	}
}

//Query available memory information within the specified Device
void OpenCL_Data::queryMemoryInfo()
{
	char buffer[1024];
	size_t buffer_used;

	//Display Global Memory information
	clGetDeviceInfo(this->deviceID, CL_DEVICE_GLOBAL_MEM_SIZE, sizeof(buffer), buffer, &buffer_used);
	printf("Global Memory Size (bytes): \t%d\n", ((cl_ulong *)buffer)[0]);
	
	//Display Global Memory Cache Info
	clGetDeviceInfo(this->deviceID, CL_DEVICE_GLOBAL_MEM_CACHE_SIZE, sizeof(buffer), buffer, &buffer_used);
	printf("Global Memory Cache (bytes): \t%d\n", ((cl_ulong *)buffer)[0]);
	clGetDeviceInfo(this->deviceID, CL_DEVICE_GLOBAL_MEM_CACHE_TYPE, sizeof(buffer), buffer, &buffer_used);
	printf("Global Memory Cache Type: \t%d\n", ((cl_device_mem_cache_type *)buffer)[0]);

	clGetDeviceInfo(this->deviceID, CL_DEVICE_MAX_MEM_ALLOC_SIZE, sizeof(buffer), buffer, &buffer_used);
	printf("Maximum Memory Alloc (bytes): \t%d\n", ((cl_ulong *)buffer)[0]);
		

	//Display Local Memory information
	clGetDeviceInfo(this->deviceID, CL_DEVICE_LOCAL_MEM_SIZE, sizeof(buffer), buffer, &buffer_used);
	printf("Maximum Local Memory Size (bytes): \t%d\n", ((cl_ulong *)buffer)[0]);
	clGetDeviceInfo(this->deviceID, CL_DEVICE_LOCAL_MEM_TYPE, sizeof(buffer), buffer, &buffer_used);
	printf("Local Memory Type: \t%.*s\n", ((cl_device_local_mem_type *)buffer)[0]);

}

//Create the command queue and context for the associated device
void OpenCL_Data::initialize()
{

	cl_int ret;
	this->context = clCreateContext(NULL, 1, &(this->deviceID), NULL, NULL, &ret);
	if (ret != CL_SUCCESS)
	{
		cout << "Error. Failed to establish a context. CL Error: " << ret << endl;
	}
	else
	{
		this->commandQueue = clCreateCommandQueue(this->context, this->deviceID, 0, &ret);
		if (ret != CL_SUCCESS)
			cout << "Error. Failed to create the command queue. CL Error: " << ret << endl;
		else
		{
			//Else, initializeBuffers
			this->initializeBuffers();	
		}
	}
	
}

//Configure the device program from the specified kernelFile and the functionName
void OpenCL_Data::setProgram(string kernelFileName, string functionName)
{
	size_t srcKernelLength = 0; 
	FILE *kernelFile = NULL;
	char *kernelString = NULL;
	cl_int err_code = 0;

	//first we open the kernel file
	kernelFile = fopen(kernelFileName.c_str(), "r");
	if (kernelFile == NULL)
		cout << "Failed to open the Kernel File. " << endl;
	else
	{
		
		fseek(kernelFile, 0L, SEEK_END);
		srcKernelLength = ftell(kernelFile);
		fseek(kernelFile, 0L, SEEK_SET);
		kernelString = (char *)calloc(srcKernelLength, 1);
		
	
		//now, read the kernel file into the memory
		size_t kernelLength = fread(kernelString, 1, srcKernelLength, kernelFile);
	
		//close the kernel file
		fclose(kernelFile);
	
		//create the program with the source code in the kernel file
		const char * str = kernelFileName.c_str();
		this->program = clCreateProgramWithSource(this->context, 1, (const char **)&str, &srcKernelLength, &err_code);
		if (err_code != CL_SUCCESS) {
			free(kernelString);
			cout << "Error. Failed to create program from source. CL Error: " << err_code << endl;
		}
		else
		{
			//build the program from the source
			err_code = clBuildProgram(this->program, 1, &(this->deviceID), NULL, NULL, NULL);
			if (err_code != CL_SUCCESS) {
				cout << "Error. Could not build program. CL Error: " << err_code << endl;
				size_t length;
				char *buffer;
				clGetProgramBuildInfo(this->program, this->deviceID, CL_PROGRAM_BUILD_LOG, 0, NULL, &length);
				buffer = (char *)malloc(length);
				cout << endl << "Build Log Information" << endl;
				clGetProgramBuildInfo(this->program, this->deviceID, CL_PROGRAM_BUILD_LOG, length, buffer, NULL);
				cout << buffer;
				free(buffer);
				free(kernelString);
			}
			else 
			{
				//create our kernel
				this->kernel = clCreateKernel(this->program, functionName.c_str(), &err_code);
				if (err_code != CL_SUCCESS) {
					cout << "Error. Failed to create kernel. CL Error: " << err_code << endl;
				}				
				//free allocated memory
				free(kernelString);
			}
		}
	}
}

//Set a kernel argument based upon parameters
void OpenCL_Data::setKernelArgument(int argIndex, void *argument, size_t argumentSize, OpenCL_IO io, OpenCL_MemType memType)
{
	//Add the kernel argument to the vector. Construct an object
	OpenCL_Argument arg = {0};
	arg.argumentIndex = argIndex;
	arg.argument = argument;
	arg.argumentSize = argumentSize;
	arg.io = io;
	arg.memType = memType;
	arg.buffer = NULL;

	int flag = 0;
	//ensure the argument index does not yet exist 
	for (int i = 0; i < this->arguments.size(); i++)
	{
		if (this->arguments.at(i).argumentIndex == argIndex)
		{
			cout << "Error. Argument already specified. Please use updateKernelArgument(). " << endl;
			flag = 1;
		}
	}

	if (!flag)
		this->arguments.push_back(arg);
}

//This will set a kernel argument with the specified argument parameter if it does not already exist
void OpenCL_Data::setKernelArgument(OpenCL_Argument argument)
{
	int flag = 0;

	//ensure the argument index does not yet exist 
	for (int i = 0; i < this->arguments.size(); i++)
	{
		if (this->arguments.at(i).argumentIndex == argument.argumentIndex)
		{
			cout << "Error. Argument already specified. Please use updateKernelArgument(). " << endl;
			flag = 1;
		}
	}

	if (!flag)
		this->arguments.push_back(argument);
}

//This function will update the argument specified by argIndex with the provided parameters
void OpenCL_Data::updateKernelArgument(int argIndex, void *argument, size_t argumentSize, OpenCL_IO io, OpenCL_MemType memType)
{
	int flag = 0;

	//Search for the argument in the vector
	for (int i = 0; i < this->arguments.size(); i++)
	{
		if (this->arguments.at(i).argumentIndex == argIndex)
		{
			flag = 1;
			this->arguments.at(i).argument = argument;
			this->arguments.at(i).argumentSize = argumentSize;
			this->arguments.at(i).io = io;
			this->arguments.at(i).memType = memType;
		}
	}

	if (!flag)
		cout << "Error. Cannot update argument. Argument is not specified." << endl;
}

//This function will update the argument specified by Index with the argument parameter
void OpenCL_Data::updateKernelArgument(OpenCL_Argument argument)
{
	int flag = 0;

	//Search for the argument in the vector
	for (int i = 0; i < this->arguments.size(); i++)
	{
		if (this->arguments.at(i).argumentIndex == argument.argumentIndex)
		{
			flag = 1;
			this->arguments.at(i) = argument;
		}
	}

	if (!flag)
		cout << "Error. cannot update argument. Argumetn not specified." << endl;
}

//This function removes a kernel argument from the vector of arguments
void OpenCL_Data::removeKernelArgument(int argIndex)
{

	cl_int ret;
	int flag = 0;
	for (int i = 0; i < this->arguments.size(); i++)
	{
		if (this->arguments.at(i).argumentIndex == argIndex) 
		{
			//Remove the Argument from the vector
			//First, release the buffer object if it exists
			if (this->arguments.at(i).buffer != NULL)
			{
				ret = clReleaseMemObject(this->arguments.at(i).buffer);
				if (ret != CL_SUCCESS)
					cout << "Error. Failed to release memory object for argument " << argIndex <<". CL Error: " << ret << endl;
			}
			this->arguments.erase(this->arguments.begin()+i);
			flag = 1;
		}
	}

	if (!flag)
		cout << "Error. Specified argument does not exist. " << endl;
}

//This function will return an argument specified by the index
OpenCL_Argument OpenCL_Data::getKernelArgument(int argIndex)
{
	OpenCL_Argument ret ={0};
	for (int i = 0; i < this->arguments.size(); i++)
	{
		if (this->arguments.at(i).argumentIndex == argIndex)
			ret = this->arguments.at(i);
	}

	return ret;
}

//This function will write all data from arguments into the cl_mem objects
void OpenCL_Data::writeBuffers()
{
	cl_int ret;

	//Copy all the data specified by the arguments into the respective openCL Buffers
	for (int i = 0; i < this->arguments.size(); i++)
	{
		ret = clEnqueueWriteBuffer(this->commandQueue, this->arguments.at(i).buffer, CL_TRUE, 0, this->arguments.at(i).argumentSize, this->arguments.at(i).argument, 0, NULL, NULL);
		if (ret != CL_SUCCESS)
			cout << "Error. Failed to enqueue write of argument " << i <<". CL Error: " << ret << endl;

	}

}

//This will read results from the device into any buffer objects that have OUTPUT or INOUT status
void OpenCL_Data::readResults()
{
	cl_int ret;
	for (int i = 0; i < this->arguments.size(); i++)
	{
		if (this->arguments.at(i).io == OUTPUT || this->arguments.at(i).io == INOUT)
		{
			ret = clEnqueueReadBuffer(this->commandQueue, this->arguments.at(i).buffer, CL_TRUE, 0, this->arguments.at(i).argumentSize, this->arguments.at(i).argument, 0, NULL, NULL);
		}
	}
}


//This function initializes all buffers and sets kernel arguments. This should not be called by the user and will be called by the initialize() function
void OpenCL_Data::initializeBuffers()
{
	cl_int ret;
	cl_mem buffer;
	int flag = 0;
	//Loop through the Arguments vector and instantiate each buffer
	for (int i = 0; i < this->arguments.size(); i++)
	{
		//Create the buffer
		switch (arguments.at(i).io)
		{
			case INPUT:
		buffer = clCreateBuffer(this->context, CL_MEM_READ_ONLY, this->arguments.at(i).argumentSize, this->arguments.at(i).argument, &ret);
				break;
			case OUTPUT:
		buffer = clCreateBuffer(this->context, CL_MEM_WRITE_ONLY, this->arguments.at(i).argumentSize, this->arguments.at(i).argument, &ret);
				break;
			case INOUT:
		buffer = clCreateBuffer(this->context, CL_MEM_READ_WRITE, this->arguments.at(i).argumentSize, this->arguments.at(i).argument, &ret);
				break;
		}

		if (ret != CL_SUCCESS)
		{
			cout << "Failed to create buffer for argument " << i <<". CL Error: " << ret << endl;
			flag += 1;
		}
		else
		{
			this->arguments.at(i).buffer = buffer;
			if (this->arguments.at(i).memType == GLOBAL)
			{
				ret = clSetKernelArg(this->kernel, (cl_uint)this->arguments.at(i).argumentIndex, this->arguments.at(i).argumentSize, this->arguments.at(i).argument);
				if (ret != CL_SUCCESS)
				{
					cout << "Error. Failed to set kernel argument " << i <<". CL Error: " << ret << endl;
					flag += 1;
				}
			}
			else
			{
				ret = clSetKernelArg(this->kernel, (cl_uint)this->arguments.at(i).argumentIndex, this->arguments.at(i).argumentSize, NULL);
				if (ret != CL_SUCCESS)
				{
					cout << "Error. Failed to set kernel argument " << i <<". CL Error: " << ret << endl;
					flag += 1;
				}
			
			}
		}

		//Set the Kernel Argument	
	}
	
	if (flag)
		cout << "Failed to set " << flag << " arguments." << endl;

}

//This function will enqueue a kernel to execute across the device with the specified ranges
void OpenCL_Data::start(int globalWorkSize, int localWorkSize)
{
	cl_int ret;

	ret = clEnqueueNDRangeKernel(this->commandQueue, this->kernel, 1, NULL, (size_t *)&globalWorkSize, (size_t *)&localWorkSize, 0, NULL, NULL);
	if (ret != CL_SUCCESS)
		cout << "Error. Could not begin program execution. CL Error: " << ret << endl;
}
