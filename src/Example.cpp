#include "StreamOCL.hpp"


//This is an example program that will probe the computer for different devices
int main()
{
	OpenCL_Data data;
	system("clear");

	data.queryPlatforms();
	data.queryAllDevices();
}


