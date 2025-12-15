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
  let start_idx = lines.position(l => l.starts-with("\"bench/"))

  // if none found, panic
  if start_idx == none {
    panic("No benchmark data found in file: " + algorithm + "_" + shape + ".csv")
  }
  // slice from there
  let filtered = lines.slice(start_idx, lines.len())
  let data = csv(bytes(filtered.join("\n")))
  let col_idx = if col == "iterations" { 1 } else if col == "real" { 2 } else if col == "cpu" { 3 } else {
    panic("Can only specify col as `real`, `cpu` or `iterations`")
  }
  (
    data.map(x => x.at(0).split("/").last()).map(int),
    data.map(x => x.at(col_idx)).map(float).map(y => y / 1000000),
  )
}


/// Log scale, with base 2.
/// To use:
/// ```typst
/// #lq.diagram(
///   // ... plots here ...
/// xscale: "log",
/// yscale: "log",
/// )
/// ```
#let log2 = lq.scale.log(base: 2)

/// Create a plot for the given benchmark.
// remove the second part of the shape name for the label
// To use:
/// ```typst
/// #lq.diagram(
///   plot_bench("graham", "circle")
///   // ... plot options here ...
/// )
/// ```
#let plot_bench(algorithm, shape, col: "real", label: auto) = lq.plot(
  ..read_bench(algorithm, shape),
  label: if label != auto {
    label
  } else {
    shape.replace("_", " ") + " - " + algorithm
  },
)


/// Plot points used in the benchmarks.
#let read_points(shape, size) = {
  // print path
  let filepath = "/build/tests/" + shape + "/" + size
  let data = read(filepath)
  // remove first line that contains the number of points
  let lines = data.split("\n").slice(1)
  let points = lines
    .filter(l => l.trim() != "") // filter out empty lines
    .map(l => {
      let coords = l.split(" ").map(s => s.trim()).filter(s => s != "") // handle multiple spaces
      (float(coords.at(0)), float(coords.at(1))) // convert to float
    })
  points
}

/// Create a scatter plot of the given points.
#let plot_points(shape, size) = {
  let points = read_points(shape, size)
  lq.scatter(
    points.map(p => p.at(0)),
    points.map(p => p.at(1)),
    // point_size: 2pt,
    label: shape + " - " + size + " points",
  )
}

/// Create a diagram of the given points.
#let diagram_points(shape, size) = lq.diagram(
  plot_points(shape, size),
  xaxis: (label: "X"),
  yaxis: (label: "Y"),
  legend: (position: top + left),
)

#let read_hull(shape, algorithm, size) = {
  let filepath = "/build/output/" + shape + "/" + str(size) + "/" + algorithm
  let data = read(filepath)
  let lines = data.split("\n")
  let points = lines
    .filter(l => l.trim() != "") // filter out empty lines
    .map(l => {
      let coords = l.split(" ").map(s => s.trim()).filter(s => s != "") // handle multiple spaces
      (float(coords.at(0)), float(coords.at(1))) // convert to float
    })
  points
}

/// plot the points and connect them printing the convex hull
#let plot_hull(shape, size, algorithm) = {
  let points = read_points(shape, str(size))
  let hull = read_hull(shape, algorithm, size)

  // Close the hull by appending the first point
  let hull_closed = hull + (hull.at(0),)

  lq.diagram(
    lq.scatter(
      points.map(p => p.at(0)),
      points.map(p => p.at(1)),
      label: "Points",
    ),
    ..hull_closed
      .enumerate()
      .slice(0, -1)
      .map(((i, pt)) => {
        lq.line(
          pt,
          hull_closed.at(i + 1),
          stroke: (paint: red, thickness: 1.5pt),
          label: if i == 0 { "Convex Hull" } else { none }, // Only label first segment
        )
      }),
    xaxis: (label: "X"),
    yaxis: (label: "Y"),
    legend: (position: top + left),
  )
}

#let algorithm_plot(algo) = lq.diagram(
  plot_bench(algo, "circle"),
  plot_bench(algo, "square"),
  plot_bench(algo, "parabola"),
  xaxis: (label: "Number of elements", scale: log2),
  yaxis: (label: "Running time (ms)", scale: "log"),
  legend: (position: top + left),
  width: 100%,
  height: 6cm,
)

#let algocmp_plot(algo1, algo2, label1: none, label2: none) = lq.diagram(
  plot_bench(algo1, "square", label: if label1 != auto { label1 } else { algo1 }),
  plot_bench(algo2, "square", label: if label2 != auto { label2 } else { algo2 }),
  xaxis: (label: "Number of elements", scale: log2),
  yaxis: (label: "Running time (ms)", scale: "log"),
  legend: (position: top + left),
  width: 100%,
  height: 6cm,
)

// create a diagram with all benchmarks
#let plot_benchmarks(shape) = {
  lq.diagram(
    plot_bench("grahamvec", shape),
    plot_bench("marriage", shape),
    plot_bench("quick", shape),
    //plot_bench("marriagev2", shape),
    xaxis: (label: "Number of elements", scale: log2),
    yaxis: (label: "Running time (ms)", scale: "log"),
    legend: (position: top + left),
    width: 100%,
    height: 6cm,
  )
}

#let point_hull_size(algorithm, shape, size) = {
  let points = read_points(shape, str(size))
  let hsize = read_hull(shape, algorithm, size).len()
  // Y: size of the hull
  // X: log2 of the number of points
  (
    points.len(),
    hsize,
  )
}

#let plot_hull_sizes(algorithm) = {
  // gather points for values from 2**8 to 2**15
  let sizes = range(8, 13).map(i => calc.pow(2, i))
  
  let circle_data = sizes.map(size => point_hull_size(algorithm, "circle", size))
  let square_data = sizes.map(size => point_hull_size(algorithm, "square", size))
  
  lq.diagram(
    lq.plot(
      circle_data.map(d => d.at(0)),
      circle_data.map(d => d.at(1)),
      label: "Circle",
    ),
    lq.plot(
      square_data.map(d => d.at(0)),
      square_data.map(d => d.at(1)),
      label: "Square",
    ),
    xaxis: (label: "log2(Number of points)", scale: log2),
    yaxis: (label: "Hull Size", scale: log2),
    legend: (position: top + left),
    width: 100%,
    height: 6cm,
  )
}

#let plot_hull_size_increasing_factor() = {
  // plot the ration of hull size to number of points for both circle and square
  let sizes = range(8, 13).map(i => calc.pow(2, i))
  
  // Y: hull size circle / hull size square
  // X: log2 of the number of points
  let result_data = sizes.map(size => {
    let csize = point_hull_size("quick", "circle", size).at(1)
    let ssize = point_hull_size("quick", "square", size).at(1)
    (
      calc.log(size, base: 2),
      csize / ssize,
    )
  })

  //         R = 10.0 * (size / 256)
  let radius_data = sizes.map(size => {
    (
      calc.log(size, base: 2),
      10.0 * (size / 256),
    )
  })

  lq.diagram(
    lq.plot(
      result_data.map(d => d.at(0)),
      result_data.map(d => d.at(1)),
      label: "increasing factor",
    ),
    // plot for reference the radius
    lq.plot(
      radius_data.map(d => d.at(0)),
      radius_data.map(d => d.at(1)),
      label: "reference radius",
    ),
    xaxis: (label: "log2(Number of points)", scale: log2),
    yaxis: (label: "Hull Size Circle / Hull Size Square", scale: log2),
    legend: (position: top + left),
    width: 100%,
    height: 6cm,
  )
}
