cmake_minimum_required(VERSION 3.2.2)
project(hls)

set(HLS_ROOT_PATH ${CMAKE_CURRENT_SOURCE_DIR})
set(LLVM_FPGA_HOME_SOURCE_DIR ~/llvm-fpga-home)
set(TARGET_ARCH X86)

add_subdirectory(src)
