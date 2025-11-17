#include "common.hpp"
#include "graham_scan.hpp"
#include "quickhull.hpp"
#include "marriage_before_conquest.hpp"
#include "util.hpp"
#include <benchmark/benchmark.h>
#include <sstream>
#include <vector>

typedef enum {
  Circle = 0,
  Parabola = 1,
  Square = 2,
} Shape;

std::vector<Point> read_points(Shape shape, int size) {
  std::stringstream s;
  s << "build/tests/";
  switch (shape) {
  case Circle:
    s << "circle/";
    break;
  case Parabola:
    s << "parabola/";
    break;
  case Square:
    s << "square/";
    break;
  }
  s << size;

  std::vector<Point> res;
  util::read_points_from_file(s.str(), res);
  return res;
}

template <typename T>
void bench(benchmark::State &state, ConvexHull<T> const& algo, Shape shape) {
  std::vector<Point> points = read_points(shape, state.range());

  for (auto _ : state)
    benchmark::DoNotOptimize(algo.compute(points));
}

BENCHMARK_CAPTURE(bench, grahamvec_circle, GrahamScan<std::vector<Point>>(), Circle)->RangeMultiplier(2)->Range(256, 524288);
BENCHMARK_CAPTURE(bench, grahamvec_square, GrahamScan<std::vector<Point>>(), Square)->RangeMultiplier(2)->Range(256, 524288);
BENCHMARK_CAPTURE(bench, grahamvec_parabola, GrahamScan<std::vector<Point>>(), Parabola)->RangeMultiplier(2)->Range(256, 524288);
BENCHMARK_CAPTURE(bench, grahamlist_circle, GrahamScan<std::list<Point>>(), Circle)->RangeMultiplier(2)->Range(256, 524288);
BENCHMARK_CAPTURE(bench, grahamlist_square, GrahamScan<std::list<Point>>(), Square)->RangeMultiplier(2)->Range(256, 524288);
BENCHMARK_CAPTURE(bench, grahamlist_parabola, GrahamScan<std::list<Point>>(), Parabola)->RangeMultiplier(2)->Range(256, 524288);
BENCHMARK_CAPTURE(bench, grahamdeque_circle, GrahamScan<std::deque<Point>>(), Circle)->RangeMultiplier(2)->Range(256, 524288);
BENCHMARK_CAPTURE(bench, grahamdeque_square, GrahamScan<std::deque<Point>>(), Square)->RangeMultiplier(2)->Range(256, 524288);
BENCHMARK_CAPTURE(bench, grahamdeque_parabola, GrahamScan<std::deque<Point>>(), Parabola)->RangeMultiplier(2)->Range(256, 524288);
BENCHMARK_CAPTURE(bench, quick_circle, QuickHullNS::QuickHull(), Circle)->RangeMultiplier(2)->Range(256, 524288);
BENCHMARK_CAPTURE(bench, quick_square, QuickHullNS::QuickHull(), Square)->RangeMultiplier(2)->Range(256, 524288);
BENCHMARK_CAPTURE(bench, quick_parabola, QuickHullNS::QuickHull(), Parabola)->RangeMultiplier(2)->Range(256, 524288);
BENCHMARK_CAPTURE(bench, marriage_circle, MarriageNS::MarriageBeforeConquest(), Circle)->RangeMultiplier(2)->Range(256, 524288);
BENCHMARK_CAPTURE(bench, marriage_square, MarriageNS::MarriageBeforeConquest(), Square)->RangeMultiplier(2)->Range(256, 524288);
BENCHMARK_CAPTURE(bench, marriage_parabola, MarriageNS::MarriageBeforeConquest(), Parabola)->RangeMultiplier(2)->Range(256, 524288);
BENCHMARK_CAPTURE(bench, marriagev2_circle, MarriageNS::MarriageBeforeConquestV2(), Circle)->RangeMultiplier(2)->Range(256, 524288);
BENCHMARK_CAPTURE(bench, marriagev2_square, MarriageNS::MarriageBeforeConquestV2(), Square)->RangeMultiplier(2)->Range(256, 524288);
BENCHMARK_CAPTURE(bench, marriagev2_parabola, MarriageNS::MarriageBeforeConquestV2(), Parabola)->RangeMultiplier(2)->Range(256, 524288);

BENCHMARK_MAIN();
