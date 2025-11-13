#import "@preview/lilaq:0.5.0" as lq

// Example usage of this file:
// #lq.diagram(
//   plot_bench("graham", "circle"),
//   plot_bench("graham", "square"),
//   plot_bench("graham", "parabola"),
//   xaxis: (label: "Number of elements", scale: log2),
//   legend: (position: top + left),
// )

/// Reads the benchmark results of the given algorithm,
/// and returns a tuple of two arrays. The first array
/// contains the number of points, while the second array contains
/// the time to calculate the hull.
#let read_bench(algorithm, shape, col: "real") = {
  let data = read("data/" + algorithm + "_" + shape + ".csv")
  let lines = data.split("\n")
  // find the first index where the line starts with "bench/"
  let start_idx = lines
    .position(l => l.starts-with("\"bench/"))
  
  // if none found, panic
  if start_idx == none {
    panic("No benchmark data found in file: " + algorithm + "_" + shape + ".csv")
  }
  // slice from there
  let filtered = lines.slice(start_idx, lines.len())
  let data = csv(bytes(filtered.join("\n")))
  let col_idx = if col == "iterations" { 1 }
    else if col == "real" { 2 }
    else if col == "cpu" { 3 }
    else { panic("Can only specify col as `real`, `cpu` or `iterations`") }
  (
    data.map(x => x.at(0).split("/").last()).map(int),
    data.map(x => x.at(col_idx)).map(float),
  )
}


/// Log scale, with base 2.
/// To use:
/// ```typst
/// #lq.diagram(
///   // ... plots here ...
///   xaxis: (scale: log2),
/// )
/// ```
#let log2 = lq.scale.log(base: 2)

/// Create a plot for the given benchmark.
// To use:
/// ```typst
/// #lq.diagram(
///   plot_bench("graham", "circle")
///   // ... plot options here ...
/// )
/// ```
#let plot_bench(algorithm, shape, col: "real") = lq.plot(..read_bench(algorithm, shape), label: shape)
