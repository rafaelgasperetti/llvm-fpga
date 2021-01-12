# Set the minimum cmake version required
cmake_minimum_required(VERSION 3.2.2)

set(SOURCES
	TesteSCC.cpp
)

set(CMAKE_CXX_FLAGS "-std=c++14")

add_executable(TesteSCC TesteSCC.cpp)

install(
	TARGETS
		TesteSCC
	RUNTIME DESTINATION
		teste-lemon
)
