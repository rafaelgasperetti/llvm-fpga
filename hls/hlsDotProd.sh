clear

echo "Starting compilation and instalation of HLS project..."

if [ ! -d "build" ]; then
	echo "Creating build directory..."
	mkdir build
fi

if [ ! -d "install" ]; then
	echo "Creating install directory..."
	mkdir install
fi

echo "Configuring the installation of HLS project..."
cd build
cmake -DCMAKE_INSTALL_PREFIX=../install .. -DCMAKE_BUILD_TYPE=Debug
echo "HLS project successfully configured."

echo "Compiling and installing HLS project..."
make && make install
if [ $? -eq 0 ]; then
	echo "HLS project sucessfully compilled."

	cd ..

	dir=`pwd`

	cd install
	
	if [ $? -eq 0 ]; then
		fileInput="tests/dotprod/dotprod.c"
		fileOutput="testsOutput/dotprod/dotprod.vhd"
		echo "Generating clang graph for algorithm from \"$dir/$fileInput\" to \"$dir/$fileOutput\"..."
		../../../llvminstall/bin/clang -cc1 -ast-view "../"$fileInput""
		echo "Testing HLS project with DotProd algorithm from \"$dir/$fileInput\" to \"$dir/$fileOutput\"..."
		./hls "../"$fileInput" ../"$fileOutput" 32"
		echo "Successfully tested DotProd algorithm."
	else
		echo "HLS project failed to install."
	fi
else
	echo "HLS project failed to compile."
fi
