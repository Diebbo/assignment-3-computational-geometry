run: build
    ./build/convex_hull

vis:
    cd vis && uv run manim -p main.py

build:
    mkdir -p build
    CMAKE_EXPORT_COMPILE_COMMANDS=true cmake -S . -B build -G Ninja
    cmake --build build

algorithms := "grahamvec grahamlist grahamdeque quick marriage"
shapes := "circle parabola square"
bench generate_tests="true" algorithm=algorithms shape=shapes: build
    #!/bin/sh

    # Generate input files
    if [ "{{generate_tests}}" == "true" ]; then
        uv run --with numpy vis/generate_tests.py
    fi

    # Run benchmarks
    for algo in {{algorithm}}; do
        for shape in {{shape}}; do
            just _bench-individual $algo $shape
        done
    done

_bench-individual algorithm shape:
    @mkdir -p report/data
    ./build/bench --benchmark_filter="bench/{{algorithm}}_{{shape}}/.*" --benchmark_out_format="csv" --benchmark_out="report/data/{{algorithm}}_{{shape}}.csv"

report algorithm=algorithms shape=shapes: build
    #!/bin/sh

    # Generate input files if not ./build/tests directory exists
    if [ ./build/tests -d ]; then
        echo "Input files already exist, skipping generation."
    else
        uv run --with numpy vis/generate_tests.py
    fi

    just run

    xdg-open ./report/main.pdf&
    typst watch ./report/main.typ --root=. 
