#This is a makefile for compiling the OpenCL Support library from source.

# This library is compiled with the OpenCL Library as well. Please ensure that you have the required
# OpenCL Library installed on your computer. Simply modify the below path to specify where the OpenCL Library is installed on your computer

#Specify the location of the $(OPENCL_LIB_NAME).so file
OPENCL_LIB_PATH=/opt/AMDAPPSDK-3.0/lib/x86_64

#Specify the location of OpenCL's Include file
OPENCL_INCLUDE_PATH=/opt/AMDAPPSDK-3.0/include

#Specify the OPENCL Library Name
OPENCL_LIB_NAME=OpenCL



#Other Variables
BUILD_DIR=build/
SRC_DIR=src/
INC_DIR=include/
CFLAGS= -L$(OPENCL_LIB_PATH) -Wformat=0 -I$(OPENCL_INCLUDE_PATH) -I$(INC_DIR)
CC=g++
LCC=ar
LCFLAGS=rvs

default: OpenCL_Support.o
	$(LCC) $(LCFLAGS) $(BUILD_DIR)libOpenCL_Support.a $(BUILD_DIR)$^

OpenCL_Support.o: 
	$(CC) $(CFLAGS) -c $(SRC_DIR)OpenCL_Support.cpp -o $(BUILD_DIR)$@ -l$(OPENCL_LIB_NAME)

$(shell mkdir -p $(BUILD_DIR))

