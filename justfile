run: build
	./build/convex_hull

vis:
    cd vis && uv run manim -p main.py

build:
	mkdir -p build
	CMAKE_EXPORT_COMPILE_COMMANDS=true cmake -S . -B build -G Ninja
	cmake --build build
