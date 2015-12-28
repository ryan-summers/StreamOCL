#!/bin/bash

# This script will install the library onto a linux machine by first calling the makefile and then moving header and library into the default include and lib paths

make

sudo cp build/libStreamOCL.a /usr/lib/libStreamOCL.a
sudo cp include/StreamOCL.hpp /usr/include/StreamOCL.hpp
