#import "../bench.typ" as bench


== Marriage Before Conquest Algorithm

=== Implementation

In the following section, we will present the implementation of the Marriage Before Conquest algorithm and the benchmarks we performed to evaluate its performance.

#figure(
  caption: [Marriage Before Conquest working principle],
  kind: auto,
grid(
    columns: 2,
  image("../assets/mbc-idea.png", width: 100%),
  )
)

Firstly, we will introduce some of the utility function created to support the core algorithm(see).

#figure(
  caption: [Function to find the relative position of a point with respect to a line segment],
  kind: auto,
```cpp
bool isLeft(const Point &p1, const Point &p2, const Point &p3) {
  const double dx_32 = p3.x - p2.x;
  const double dy_12 = p1.y - p2.y;
  const double dy_32 = p3.y - p2.y;
  const double dx_12 = p1.x - p2.x;

  return (dx_32 * dy_12) > (dy_32 * dx_12);
}
```
)

The function above checks if a point p3 is to the left of the line segment formed by points p1 and p2. This is useful for determining the relative position of points during the construction of the convex hull.

#figure(
  caption: [Function to find extreme horizontal points with upper or lower hull cases],
  kind: auto,
```cpp
Line findExtremePoints(const Points &points, bool upper) {
  // Find leftmost and rightmost points with highest y in case of ties
  Point minPoint = points[0];
  Point maxPoint = points[0];
  if (upper) {
    for (const auto &p : points) {
      if (p.x < minPoint.x || (p.x == minPoint.x && p.y > minPoint.y)) {
        minPoint = p;
      }
      if (p.x > maxPoint.x || (p.x == maxPoint.x && p.y > maxPoint.y)) {
        maxPoint = p;
      }
    }
    return Line{minPoint, maxPoint};
  } else {
    for (const auto &p : points) {
      if (p.x < minPoint.x || (p.x == minPoint.x && p.y < minPoint.y)) {
        minPoint = p;
      }
      if (p.x > maxPoint.x || (p.x == maxPoint.x && p.y < maxPoint.y)) {
        maxPoint = p;
      }
    }
    return Line{maxPoint, minPoint};
  }
}
```
)

The function shown above finds the extreme points in a set of 2D points, specifically the leftmost and rightmost points, while handling cases where multiple points share the same x-coordinate but have different y-coordinates, based on whether we are considering the upper or lower hull.

The algorithm implementation is shown below @app:mbc.

The compute function implements the core logic of the Marriage Before Conquest algorithm. It first checks if the number of points is less than or equal to two, in which case it simply returns the points as they already form a convex hull. For larger sets of points, it initializes an empty hull and shuffles the input points randomly to ensure a good distribution for the algorithm's performance. It then calls the recursive functions for constructing the upper and lower hulls, finally ensuring that the leftmost point is not duplicated in the final hull.

The MBCUpperRecursive function is a recursive implementation of the upper hull construction in the Marriage Before Conquest algorithm. It handles base cases for small sets of points and recursively finds the upper bridge, partitioning the points into left and right subsets for further processing, after finding a valid bridge.

The _findUpperBridge_ function identifies the upper bridge for a given set of points in the Marriage Before Conquest algorithm. It initializes the bridge with two points and iteratively refines it by checking the relative positions of other points, ensuring that the bridge correctly represents the upper boundary of the convex hull.

The algorithm for finding the upper bridge takes $O(n)$ expected time, but in the worst case, it can take $O(n^2)$ time due to the nested loops when updating the bridge points. That's why the algorithm relies on randomization (in the _compute_ function) to achieve better average performance.

It is worth noting that the implementation of the lower hull construction and the corresponding bridge finding function follows a similar logic, adjusted for the lower hull case.

=== Complexity Analysis

Let's analyze the time complexity of the Marriage Before Conquest algorithm.

The algorithm starts by shuffling the points, which takes $O(n)$ time.


Finding a bridge takes $O(n)$ time. Points that cannot contribute to the convex hull on either side of the median line are discarded. The algorithm then proceeds recursively on the remaining points to compute the upper and lower parts of the convex hull.

For a balanced partitioning on the median

At each recursion level $i$, the algorithm solves at most  $2^i$ subproblems, each containing at most $n/2^i$ points. Since each subproblem identifies a single edge of the convex hull, the total number of subproblems is bounded by $h$, the number of points on the hull. In the worst case, when no points can be discarded early, the recursion depth is $O(log h)$, and each level processes $O(n)$ points in linear time. This results in an overall time complexity of $O(n log h)$. This makes the Marriage Before Conquest algorithm an output-sensitive algorithm, as its performance depends on the size of the output (the number of points on the convex hull) rather than just the input size.



=== Benchmarks

In our benchmarks, we expect the Marriage Before Conquest algorithm to perform efficiently on average, with a time complexity of $O(n log n)$ for random distributions of points.

The results of our benchmarks shown in the diagram below respects our expectations, with Marriage Before Conquest performing well on circular and square distributions, while showing increased computation time on the parabola distribution as the number of points on the hull increases, since it is output-sensitive.

Following up the results for the Marriage Before Conquest algorithm in linear scale:
// full page
#bench.lq.diagram(
  bench.plot_bench("marriage", "circle"),
  bench.plot_bench("marriage", "square"),
  bench.plot_bench("marriage", "parabola"),
  xaxis: (label: "Number of elements"),
  legend: (position: top + left),
  xscale: bench.log2,
  yscale: "log",
  width: 100%,
  height: 6cm,
)

And in log-log scale. //(see @fig:mbc-bench-loglog).

#bench.lq.diagram(
  bench.plot_bench("marriage", "circle"),
  bench.plot_bench("marriage", "square"),
  bench.plot_bench("marriage", "parabola"),
  xaxis: (label: "Number of elements"),
  xscale: bench.log2,
  yscale: "log",
  legend: (position: top + left),
  width: 100%,
  height: 6cm,
)<fig:mbc-bench-loglog>

=== Optimizations

In order to have a more fair comparison we decided to test the algorithm on also a compiler-optimized version with the -O3 flag enabled. The results are shown below.

#bench.lq.diagram(
  bench.plot_bench("marriage", "circle_optimized"),
  bench.plot_bench("marriage", "square_optimized"),
  bench.plot_bench("marriage", "parabola_optimized"),
  xaxis: (label: "Number of elements"),
  xscale: bench.log2,
  yscale: "log",
  legend: (position: top + left),
  width: 100%,
  height: 6cm,
)<fig:mbc-bench-loglog>

As we could expect, the optimization provided by the compiler improved the performance by a small constant factor across all the different distributions, so overall the behavior of the algorithm remained the same.

In addition, we've also measured time taken for each function inside the implementation with a flame graph (see @fig:mbc-flame).

#figure(
  caption: [Flame graph for Marriage Before Conquest algorithm],
  kind: auto,
  image("../assets/mbc_flame.png", width: 100%)
)<fig:mbc-flame>

The results reported show that the `std::shuffle` function consumes a significant portion of the total execution time, indicating that the randomization step is a notable factor in the algorithm's performance. Thus, we changed the PRNG to a faster one, to reduce the overhead introduced by the shuffling process.

=== Second version
We also implemented a second version of the Marriage Before Conquest algorithm, add an extra pruning step before solving the LP subproblem: we find the point $p_l$ with the smallest x-coordinate (if there are more than one, take the one with the largest y-coordinate) and the point $p_r$ with the largest x-coordinate (if there are more than one, take the one with the largest y-coordinate), then prune all the points that lie under the line segment $overline(p_l p_r)$


#let plot_benchmarks_mbc(shape) = {
  bench.lq.diagram(
    bench.plot_bench("marriage", shape),
    bench.plot_bench("marriagev2", shape),
    xaxis: (label: "Number of elements", scale: bench.log2),
    yaxis: (label: "Running time (ms)", scale: "log"),
    legend: (position: top + left),
    width: 100%,
    height: 6cm,
  )
}

#plot_benchmarks_mbc("circle")
#plot_benchmarks_mbc("parabola")
#plot_benchmarks_mbc("square")

As we can see we get a small improvement in performance only in the circle distribution, while in the other distributions the performance are even worse. For the parabola distribution this is expected since the pruning step does not remove any point and in the lower hull.
