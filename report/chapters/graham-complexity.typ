=== Complexity Analysis

Let's analyze the time complexity of the Graham's Scan algorithm:
- The first step is sorting the points, which takes $O(n log n)$ time.
- Then, it computes the two half-hulls.
  Since doing that requires iterating over all of the points,
  this takes $O(2n) = O(n)$ time.
- Finally, it merges the two resulting hulls,
  which again takes $O(n)$ time.

Thus, the overall time complexity is $O(n log n)$.
