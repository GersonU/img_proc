cmake_minimum_required ( VERSION 2.8 )
project ( detect_objects )

option ( CUDA_USE_STATIC_CUDA_RUNTIME OFF )
find_package ( OpenCV REQUIRED )
find_package ( CUDA REQUIRED )

set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11" )
set ( CMAKE_CXX_STANDARD_REQUIRED ON )
set ( CUDA_NVCC_FLAGS ${CUDA_NVCC_FLAGS}; -O3 -gencode arch=compute_61,code=sm_61 )

include_directories ( "/usr/local/cuda-8.0/include" )
include_directories ( "/usr/include/glib-2.0" "/usr/include/libxml2" "/usr/lib/aarch64-linux-gnu/glib-2.0/include/" )

cuda_add_executable (
	do_test.exe
	do_test.cpp
	detect_objects.cpp
	in_range.cu
)

target_link_libraries ( do_test.exe ${OpenCV_LIBS} )
