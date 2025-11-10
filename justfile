run: build
	./build/convex_hull

main2: build
    ./build/main2

build:
	mkdir -p build
	CMAKE_EXPORT_COMPILE_COMMANDS=true cmake -S . -B build -G Ninja
	cmake --build build
