#import "../bench.typ" as bench


== QuickHull Algorithm

=== Implementation

In the following section, we will present the implementation of the QuickHull algorithm and the benchmarks we performed to evaluate its performance.

#figure(
  caption: [QuickHull working principle],
  kind: auto,
grid(
    columns: 2,
  image("../assets/Quickhull_example3.svg.png", width: 50%),
  image("../assets/Quickhull_example6.svg.png", width: 50%)
  )
)

Firstly, we will introduce some of the utility function created to support the core algorithm(see @lst:extremes).

#figure(
  caption: [Function to find extreme points with vertical alignment cases],
  kind: auto,
```cpp
std::pair<TPoint, TPoint> findExtremePointsCases(const Points &points) {
  Point leftPointYMin = points[0];
  Point leftPointYMax = points[0];
  Point rightPointYMin = points[0];
  Point rightPointYMax = points[0];

  for (const auto &p : points) {
    if (p.x < leftPointYMin.x) { // new leftmost point found
      leftPointYMin = p;
      leftPointYMax = p;
    } else if (p.x == leftPointYMin.x) { // same x-coordinate as current leftmost, check y-coordinates
      if (p.y < leftPointYMin.y) {
        leftPointYMin = p;
      }
      if (p.y > leftPointYMax.y) {
        leftPointYMax = p;
      }
    }

    if (p.x > rightPointYMax.x) { // new rightmost point found
      rightPointYMin = p;
      rightPointYMax = p;
    } else if (p.x == rightPointYMax.x) { // same x-coordinate as current rightmost, check y-coordinates
      if (p.y < rightPointYMin.y) {
        rightPointYMin = p;
      }
      if (p.y > rightPointYMax.y) {
        rightPointYMax = p;
      }
    }
  }
  return {{leftPointYMin, leftPointYMax}, {rightPointYMin, rightPointYMax}};
}
```
)<lst:extremes>

The function shown above finds the extreme points in a set of 2D points, specifically the leftmost and rightmost points, while handling cases where multiple points share the same x-coordinate but have different y-coordinates (i.e., vertical alignments). The full algorithm implementation is shown below @app:quickhull.

As we have mentioned before, the compute function is the core of all the ConvexHull classes. In this particular case, it initializes the QuickHull algorithm by finding the extreme points and partitioning the input points into upper and lower subsets. It then calls the recursive functions to compute the upper and lower hulls, finally combining them into a single convex hull.

A lot of the extra steps are made to cover edge cases where multiple points share the same x-coordinate, ensuring that the algorithm correctly identifies and processes these points without introducing duplicates in the final hull.

The recursive functions used to compute the upper and lower hulls are shown below @app:quickhull_recursive.


=== Complexity Analysis

Let's analyze the time complexity of the QuickHull algorithm:
- The algorithm starts by finding the extreme points, which takes $O(n)$ time.
- The partitioning of points into upper and lower subsets also takes $O(n)$ time.
- The recursive step involves finding the point with the maximum distance from the line segment, which takes O(n) time in the worst case.
- Then we recur two times on subsets of points $O(2 T(n/2))$.

Let's now set up the recurrence relation for the time complexity:

$ T(n) = T(k) + T(n - k) + O(n) $

Where $k$ is the number of points in one of the subsets. In the worst case, $k$ can be as large as $n-1$, leading to unbalanced partitions. This results in a time complexity of $O(n^2)$ in the worst case.

=== Benchmarks

In our benchmarks, we expect the QuickHull algorithm to perform efficiently on average, with a time complexity of $O(n log n)$ for random distributions of points. However, for some testcases such as a parabola distribution, we might observe performance degradation towards the worst-case scenario of $O(n^2)$.

// TODO: why is nlog n as average ?

The results of our benchmarks shown in the diagram below respects our expectations, with QuickHull performing well on circular and square distributions, while showing increased computation time on the parabola distribution as the number of points increases.

Following up the results for the QuickHull algorithm in linear scale:
// full page
#bench.lq.diagram(
  bench.plot_bench("quick", "circle"),
  bench.plot_bench("quick", "square"),
  bench.plot_bench("quick", "parabola"),
  xaxis: (label: "Number of elements"),
  legend: (position: top + left),
  width: 100%,
  height: 6cm,
)

And in log-log scale. //(see @fig:quickhull-bench-loglog).

#bench.lq.diagram(
  bench.plot_bench("quick", "circle"),
  bench.plot_bench("quick", "square"),
  bench.plot_bench("quick", "parabola"),
  xaxis: (label: "Number of elements"),
  xscale: "log",
  yscale: "log",
  legend: (position: top + left),
  width: 100%,
  height: 6cm,
)<fig:quickhull-bench-loglog>

=== Optimizations

In order to have a more fair comparison we decided to test the algorithm on also a compiler-optimized version with the -O3 flag enabled. The results are shown below.

#bench.lq.diagram(
  bench.plot_bench("quick", "circle_optimized"),
  bench.plot_bench("quick", "square_optimized"),
  bench.plot_bench("quick", "parabola_optimized"),
  xaxis: (label: "Number of elements"),
  xscale: "log",
  yscale: "log",
  legend: (position: top + left),
  width: 100%,
  height: 6cm,
)<fig:quickhull-bench-loglog>

As we could expect, the optimization provided by the compiler improved the performance by a 5-10x factor across all the different distributions. But the overall behavior of the algorithm remained the same.

In addition, we've also measured time taken for each function inside the implementation with a flame graph (see @fig:quickhull-flame).

#figure(
  caption: [Flame graph for QuickHull algorithm],
  kind: auto,
  image("../assets/quick_flame.png", width: 100%)
)<fig:quickhull-flame>

The results reported respect our expectations, as the majority is equally split between the `isLeft`, `findExtremePointsCases`, `std::push_back` functions.
