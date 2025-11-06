build:
	# create a build directory if it doesn't exist
	mkdir -p build
	# cmake configuration
	cmake -S . -B build
	# build the project
	cmake --build build

run: build
	# run the built executable
	./build/convex_hull
