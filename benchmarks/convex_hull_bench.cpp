#include <benchmark/benchmark.h>
#include <iostream>

static void BM_ConvexHull(benchmark::State& state) {
    for (auto _ : state) {
        std::cout << "Generating " << state.range(0) << " random points..." << std::endl;
    }
}

BENCHMARK(BM_ConvexHull)->Range(100, 100000);
BENCHMARK_MAIN();
