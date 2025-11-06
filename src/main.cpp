#include "../include/common.hpp"
#include "../include/gram_scan_convex_hull.hpp"
#include <iostream>

// test purpose
int main() {
	/* We generate manually some points and compute their convex hull */
	std::vector<Point> points = {
		Point(0, 0),
		Point(1, 1),
		Point(2, 0),
		Point(1, -1),
		Point(0, 2),
		Point(2, 2)
	};

	/* Create an instance of GrahamScan and compute the convex hull using dependency injection for the compute method */
	ConvexHull<std::vector<Point>>* convexHullAlgorithm = new GrahamScan();
	std::vector<Point> hull = convexHullAlgorithm->compute(points);
	return 0;
}
