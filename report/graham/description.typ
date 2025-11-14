#set heading(numbering: "1.")

== Algorithm description

_Graham's Scan_ is an algorithm to find the convex hull
for a given set of points.
The main idea of the algorithm is to consider
the "top" and "bottom" halves of the hull separately,
merging the results at the end.

Exactly this final merging step brings forward some practical considerations:
what is the practical performance impact of this final operation?
We do some experiments around that in section @todo.

The following is the pseudocode for Graham's Scan's algorithm.

== TODO: Benchmark graham's scan with std::forward_list, std::deque, maybe std::list <todo>
