== Graham's Scan

=== Algorithm description

_Graham's Scan_ is an algorithm to find the convex hull
for a given set of points.
The main idea of the algorithm is to consider
the "top" and "bottom" halves of the hull separately,
merging the results at the end.
Our implementation of the algorithm can be found in @app:graham.

Exactly this final merging step brings forward some practical considerations:
what is the practical performance impact of this final operation?
We do some experiments around that in section @chap:graham_deque.
