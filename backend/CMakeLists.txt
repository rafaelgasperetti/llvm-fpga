# TODO: Previous subdirectory structure broken - fix

cmake_minimum_required(VERSION 2.8.4)
project(llvm-fpga)

set(LLVM_PATH ~/llvm-fpga-home/llvmsource)
set(LLVM_PATH2 ~/llvm-fpga-home/llvmbuild)
set(BACKEND_PATH ~/llvm-fpga-home/llvm-fpga-source/backend)
link_directories(${LLVM_PATH2}/lib ~/llvm-fpga-home/llvm-fpga-source/backend)
include_directories(${LLVM_PATH}/include ${LLVM_PATH}/tools/clang/include ${LLVM_PATH2}/tools/clang/include ${LLVM_PATH2}/include ~/llvm-fpga-home/llvm-fpga-source/backend/include)

get_property(dirs DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY LINK_DIRECTORIES)
foreach(dir ${dirs})
  message(STATUS "dir='${dir}'")
endforeach()

add_definitions(
-D__STDC_LIMIT_MACROS
-D__STDC_CONSTANT_MACROS
)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-rtti -std=c++11")

set(SOURCES
	${BACKEND_PATH}/src/algorithms/componentsGen.cpp
	${BACKEND_PATH}/src/algorithms/vhdlGen.cpp
	${BACKEND_PATH}/src/components/cmp_add_reg_op_s.cpp
	${BACKEND_PATH}/src/components/cmp_add_op_s.cpp
	${BACKEND_PATH}/src/components/cmp_block_ram.cpp
	${BACKEND_PATH}/src/components/cmp_counter.cpp
	${BACKEND_PATH}/src/components/cmp_delay_op.cpp
	${BACKEND_PATH}/src/components/cmp_mult_op_s.cpp
	${BACKEND_PATH}/src/components/cmp_if_gt_op_s.cpp
	${BACKEND_PATH}/src/components/cmp_if_lt_op_s.cpp
	${BACKEND_PATH}/src/components/cmp_mux_m.cpp
	${BACKEND_PATH}/src/components/cmp_neg_op_s.cpp
	${BACKEND_PATH}/src/components/cmp_reg_mux_m_op.cpp
	${BACKEND_PATH}/src/components/cmp_reg_op.cpp
	${BACKEND_PATH}/src/components/cmp_sub_op_s.cpp
	${BACKEND_PATH}/src/core/design.cpp
  ${BACKEND_PATH}/src/core/signal.cpp
	${BACKEND_PATH}/src/core/component.cpp
	${BACKEND_PATH}/src/core/genericMap.cpp
	${BACKEND_PATH}/src/core/port.cpp)
add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(llvm-fpga
  clang
  clangFrontendTool
  clangCodeGen
  clangStaticAnalyzerFrontend
  clangStaticAnalyzerCheckers
  clangStaticAnalyzerCore
  clangARCMigrate
  clangRewrite
  clangRewriteFrontend
  clangFrontend
  clangSerialization
  clangDriver
  clangParse
  clangSema
  clangAnalysis
  clangAST
  clangBasic
  clangEdit
  clangLex
  clangTooling
)

target_link_libraries(llvm-fpga
  LLVMX86AsmParser # MC, MCParser, Support, X86Desc, X86Info
  LLVMX86Desc # MC, Support, X86AsmPrinter, X86Info
  LLVMX86AsmPrinter # MC, Support, X86Utils
  LLVMX86Info # MC, Support, Target
  LLVMX86Utils # Core, Support
  LLVMipo
  LLVMScalarOpts
  LLVMInstCombine
  LLVMTransformUtils
  #LLVMIpa
  LLVMAnalysis
  LLVMTarget
  LLVMOption # Support
  LLVMMCParser # MC, Support
  LLVMMC # Object, Support
  LLVMObject # BitReader, Core, Support
  LLVMBitReader # Core, Support
  LLVMCore # Support
  LLVMSupport
)

include(FindCurses)
target_link_libraries(llvm-fpga
    pthread
    z
    dl
    ${CURSES_LIBRARIES}
)

install(
	TARGETS
		${PROJECT_NAME}
	RUNTIME DESTINATION
		${BACKEND_PATH}/install
)
