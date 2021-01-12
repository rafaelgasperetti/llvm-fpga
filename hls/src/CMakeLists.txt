cmake_minimum_required(VERSION 3.2.2)
project(hls)

set(LLVM_SOURCE_PATH ${LLVM_FPGA_HOME_SOURCE_DIR}/llvmsource)
set(LLVM_BUILD_PATH ${LLVM_FPGA_HOME_SOURCE_DIR}/llvmbuild)
set(LLVM_INSTALL_PATH ${LLVM_FPGA_HOME_SOURCE_DIR}/llvminstall)
set(BACKEND_PATH ${CMAKE_CURRENT_SOURCE_DIR})
set(${CURSES_LIBRARIES} ${LLVM_FPGA_HOME_SOURCE_DIR}/llvmsource/opt/lib/include)

link_directories(
	${LLVM_SOURCE_PATH}/tools/clang/include
	${LLVM_SOURCE_PATH}/include
	${LLVM_SOURCE_PATH}/lib
	#${LLVM_BUILD_PATH}/tools/clang/include
	#${LLVM_BUILD_PATH}/include
	${LLVM_BUILD_PATH}/lib
	${HLS_ROOT_PATH}/include
	${HLS_ROOT_PATH}/src
	#${BACKEND_PATH}
)

include_directories(
	${LLVM_SOURCE_PATH}/tools/clang/include
	#${LLVM_BUILD_PATH}/tools/clang/include
	${HLS_ROOT_PATH}/include
	${HLS_ROOT_PATH}/src
	#${CURSES_LIBRARIES}
)

get_property(dirs DIRECTORY ${HLS_ROOT_PATH} PROPERTY LINK_DIRECTORIES)
foreach(dir ${dirs})
  message(STATUS "dir='${dir}'")
endforeach()

add_definitions(
	-D__STDC_LIMIT_MACROS
	-D__STDC_CONSTANT_MACROS
	#-DFPERMISSIVE=1
)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-rtti -std=c++11")

set(SOURCES	
	${BACKEND_PATH}/Components/cmp_add_op.cpp
	${BACKEND_PATH}/Components/cmp_add_op_fl.cpp
	${BACKEND_PATH}/Components/cmp_add_reg_op_s.cpp
	${BACKEND_PATH}/Components/cmp_sub_op_s.cpp
	${BACKEND_PATH}/Components/cmp_add_op_s.cpp
	${BACKEND_PATH}/Components/cmp_block_ram.cpp
	${BACKEND_PATH}/Components/cmp_const_op.cpp
	${BACKEND_PATH}/Components/cmp_counter.cpp
	${BACKEND_PATH}/Components/cmp_delay_op.cpp
	${BACKEND_PATH}/Components/cmp_mult_op_s.cpp
	${BACKEND_PATH}/Components/cmp_if_gt_op_s.cpp
	${BACKEND_PATH}/Components/cmp_if_lt_op_s.cpp
	${BACKEND_PATH}/Components/cmp_mux_m.cpp
	${BACKEND_PATH}/Components/cmp_neg_op_s.cpp
	${BACKEND_PATH}/Components/cmp_reg_mux_m_op.cpp
	${BACKEND_PATH}/Components/cmp_reg_op.cpp
	${BACKEND_PATH}/Components/cmp_sub_op_s.cpp
	${BACKEND_PATH}/Components/cmp_add_op_s.cpp
	${BACKEND_PATH}/Components/cmp_div_op_s.cpp
	
	${BACKEND_PATH}/FileManager/FileBaseManager.cpp
	${BACKEND_PATH}/FileManager/FileReader.cpp
	${BACKEND_PATH}/FileManager/FileWriter.cpp

	${BACKEND_PATH}/Core/Design.cpp
	${BACKEND_PATH}/Core/Signal.cpp
	${BACKEND_PATH}/Core/GenericMap.cpp
	${BACKEND_PATH}/Core/Port.cpp
	${BACKEND_PATH}/Core/Component.cpp
	${BACKEND_PATH}/Core/IOComponent.cpp
	${BACKEND_PATH}/Compiler/Operation.cpp
	${BACKEND_PATH}/Compiler/Variable.cpp
	${BACKEND_PATH}/Compiler/Context.cpp
	${BACKEND_PATH}/Compiler/TimingControl.cpp
	${BACKEND_PATH}/Compiler/NodeInfo.cpp
	${BACKEND_PATH}/Compiler/NodeVisitor.cpp
	${BACKEND_PATH}/Compiler/DependencyLevel.cpp
	${BACKEND_PATH}/HLS.cpp
)

add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(hls
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

target_link_libraries(hls
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
target_link_libraries(hls
    pthread
    z
    dl
    ${CURSES_LIBRARIES}
)

install(
	TARGETS
		${PROJECT_NAME}
	RUNTIME DESTINATION
		${HLS_ROOT_PATH}/install
)
