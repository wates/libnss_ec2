cmake_minimum_required(VERSION 2.8)

project(libnss_aws)


if(CMAKE_COMPILER_IS_GNUCC)
    set(CMAKE_C_FLAGS "-fPIC -fno-strict-aliasing -O2")# --input-charset=utf8 --exec-charset=utf8
    set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS}  -std=c++11 ${CMAKE_CXX_FLAGS}")
    add_definitions(-DGCC)
elseif(MSVC)
    set(CMAKE_C_FLAGS "/W4 /wd4819 /wd4100")
    set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} /EHsc")
    add_definitions(-DWINDOWS)
    add_definitions(-D_WINDOWS)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
else()
endif()

add_subdirectory(aws-sdk-cpp)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp/aws-cpp-sdk-core/include
					${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp/aws-cpp-sdk-ec2/include
                   )


add_executable(list list.cpp instances.cc)
target_link_libraries(list aws-cpp-sdk-core aws-cpp-sdk-ec2)

add_library(nss_ec2 SHARED libmain.c instances.cc)
target_link_libraries(nss_ec2 aws-cpp-sdk-core aws-cpp-sdk-ec2)
