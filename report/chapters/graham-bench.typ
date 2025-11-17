#import "../bench.typ"

=== Benchmark results

We expected the Graham's Scan algorithm to perform with a time complexity of $O(n log n)$.
From the benchmark though we can observe that the parabola case
takes significantly less time than the other cases.
This is because the parabola, being continuous,
makes the calculation of the lower hull extremely simple,
since no "backtracking" is necessary.

#bench.algorithm_plot("grahamvec")

=== Graham's Scan in a single pass <chap:graham_deque>

We also experimented with removing the final step of the algorithm,
where the two halves are merged together to form the final hull.
To this end, we developed a variation of the algorithm that works with
containers that support both insertion from the back and insertion from the front.

The main idea was to only loop over the points once,
inserting them both at the front and at the back of the container;
afterwards, we check both ends of the container for the turning direction they have.

The code (in this example, using the `std::deque` container) can be found in @app:graham_deque.

From the benchmark below we can see that this change doesn't affect the running time.
We hypothesized that this was due to differences in the cache behaviour,
either between the two underlying data structures,
or because we insert separately at the beginning and at the end.
We tried running `perf stat` on our code,
and found that that was not what was happening;
instead, the cache misses increased by just 3%.

We tried collecting a FlameGraph#footnote[https://www.brendangregg.com/flamegraphs.html] to investigate,
and found that to be because I

#figure(image("../assets/graham_vec_flame.png"))

#bench.algocmp_plot("grahamvec", "grahamdeque", label1: "Normal version", label2: "Single-pass version")

=== Optimizations

In order to have a more fair comparison we decided to also benchmark the code
compiled with optimizations turned on. The results are shown below.

#bench.lq.diagram(
  bench.plot_bench("grahamvec", "square_optimized"),
  bench.plot_bench("grahamvec", "square"),
  xlabel: "Number of elements",
  ylabel: "Running time (ms)",
  xscale: bench.log2,
  yscale: "log",
  legend: (position: top + left),
  width: 100%,
  height: 6cm,
)<fig:quickhull-bench-loglog>
