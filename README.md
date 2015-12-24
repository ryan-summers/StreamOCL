## StreamOCL
This library provides functions written in C++ for supporting OpenCL. 
The purpose of this library is to provide simple functions to query OpenCL devices and display information about them. 
Additionally, this library provides functions to establish openCL device contexts, associate kernel files with them, write inputs, and memory management of OpenCL parameters.

### Library Contents:
 - `OpenCL_Data` Class: The `OpenCL_Data` Class contains all information associated with an OpenCL Program. An object of the `OpenCL_Data` Class contains a platform ID, a device ID, a kernel program, and buffers.

### Library Use:

1. To use the library, simply instantiate an object of the class. 

2. Next, associate a platform ID, and then a device ID with the class using `setPlatformID()` and `setDeviceID()` respectively. 

3. Next, associate and compile your openCL kernel file by supplying the file and function name to the `setProgram()` function.

4. After that, add all kernel arguments using the `SetKernelArgument()` function.

5. To write data to the OpenCL device and establish all required OpenCL Structures, call the `initialize()` function.

6. After `initialize()`, call `writeData()` to enqueue a write of all buffers to the OpenCL device.

7. Finally, the program is ready to begin. Call the `start()` function to initialize processing.

8. To read results from the device, first wait for the kernel execution to finish. When it has, call the `readResults()` function. This will read all parameters specified as OUTPUT or INOUTs to their respective buffers for use in your program.

### _Alternate Use:_

The library also provides a number of functions to easily query platform and device information on any computer. To view all platforms in a human-readable format, call the `queryPlatforms()` function. To query devices of a specific platform, set the platform using `setPlatformID()` and then call `queryDevices()` to view all devices available on the specified platform. Alternatively, you can call the function `queryAllDevices()` to view all available devices on every available platform.

The library manages all OpenCL memory associated with a device and will always release openCL variables when they leave scope.

If you have further questions about the library, Please read the wiki available on github.

If you have any issues with the library or would like to suggest additional functionality, please email the author (ryan.summers@wsu.edu).
