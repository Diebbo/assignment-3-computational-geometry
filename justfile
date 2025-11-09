run: build
	./build/convex_hull

build:
	mkdir -p build
	CMAKE_EXPORT_COMPILE_COMMANDS=true cmake -S . -B build -G Ninja
	cmake --build build
