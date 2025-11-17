run: build
    ./build/convex_hull

vis:
    cd vis && uv run manim -p main.py

build:
    mkdir -p build
    CMAKE_EXPORT_COMPILE_COMMANDS=true cmake -S . -B build -G Ninja
    cmake --build build

algorithms := "grahamvec grahamlist grahamdeque quick marriage marriagev2"
shapes := "circle parabola square"
bench only_opt="false" generate_tests="true" algorithm=algorithms shape=shapes: build
    #!/bin/sh

    # Generate input files
    if [ "{{generate_tests}}" == "true" ]; then
        uv run --with numpy vis/generate_tests.py
    fi

    # Run benchmarks
    for algo in {{algorithm}}; do
        for shape in {{shape}}; do
            if [ "{{only_opt}}" == "true" ]; then
                just _bench-individual-opt $algo $shape
                continue
            fi
            just _bench-individual $algo $shape
            just _bench-individual-opt $algo $shape
        done
    done

_bench-individual-opt algorithm shape:
    @mkdir -p report/data
    ./build/bench_opt --benchmark_filter="bench/{{algorithm}}_{{shape}}/.*" --benchmark_out_format="csv" --benchmark_out="report/data/{{algorithm}}_{{shape}}_optimized.csv"

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
flame: build
    perf record --call-graph dwarf ./build/convex_hull_opt
    perf script | inferno-collapse-perf > stacks.folded
    cat stacks.folded | inferno-flamegraph > ./report/assets/flamegraph.svg
    rm stacks.folded perf.data

