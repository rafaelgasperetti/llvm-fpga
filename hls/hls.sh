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
cmake -DCMAKE_INSTALL_PREFIX=../install ..
echo "HLS project successfully configured."

echo "Compiling and installing HLS project..."
make && make install
if [ $? -eq 0 ]; then
	echo "HLS project sucessfully compilled."

	cd ..

	dir=`pwd`

	cd install
	
	if [ $? -eq 0 ]; then
		folder="tests/Old/Autocorrelation.c"
		echo "Testing HLS project with Autocorrelation algorithm at \"$dir/$folder\"..."
		./hls "../"$folder""
		echo "Successfully tested Autocorrelation algorithm."

		folder="tests/Old/BitCount.c"
		echo "Testing HLS project with BitCount algorithm at \"$dir/$folder\"..."
		./hls "../"$folder""
		echo "Successfully tested BitCount algorithm."

		folder="tests/Old/BubbleSort.c"
		echo "Testing HLS project with BubbleSort algorithm at \"$dir/$folder\"..."
		./hls "../"$folder""
		echo "Successfully tested BubleSort algorithm."

		folder="tests/Old/Dotprod.c"
		echo "Testing HLS project with Dotprod algorithm at \"$dir/$folder\"..."
		./hls "../"$folder""
		echo "Successfully tested Dotprod algorithm."

		folder="tests/Old/Fibonacci.c"
		echo "Testing HLS project with Fibonacci algorithm at \"$dir/$folder\"..."
		./hls "../"$folder""
		echo "Successfully tested Fibonacci algorithm."

		folder="tests/Old/Max.c"
		echo "Testing HLS project with Max algorithm at \"$dir/$folder\"..."
		./hls "../"$folder""
		echo "Successfully tested Max algorithm."

		folder="tests/Old/VectorSum.c"
		echo "Testing HLS project with VectorSum algorithm at \"$dir/$folder\"..."
		./hls "../"$folder""
		echo "Successfully tested VectorSum algorithm."
	else
		echo "HLS project failed to install."
	fi
else
	echo "HLS project failed to compile."
fi
