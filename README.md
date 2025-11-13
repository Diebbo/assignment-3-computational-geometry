# assignment-3-computational-geometry
Convex Hull Computation

## Compile

```bash
just build
```


## Run

```bash
just run # also build
```

## Report

Generate and run benchmarks:

```bash
just bench
```

In the `reports/` folder you will find the generated data and to plot the graphs use in typst:

```typst
// Example usage of this file:
#lq.diagram(
  plot_bench("graham", "circle"),
  plot_bench("graham", "square"),
  plot_bench("graham", "parabola"),
  xaxis: (label: "Number of elements", scale: log2),
  legend: (position: top + left),
)
```
