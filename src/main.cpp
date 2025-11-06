#include "../include/util.hpp"
#include "../include/gram_scan_convex_hull.hpp"
#include <iostream>
#include <cassert>

// test purpose
int main() {
	/* Quick test of the sidedness function */
	Point p1(0, 0);
	Point p2(1, 1);
	Point p3(1, 0);
	float side = util::sidedness(p1, p2, p3);
	assert(side < 0); // p3 is below the line p1p2
	
	/* Quick test of the is_inside function */
	Point p4(0.6, 0.5);
	bool inside = util::is_inside(Triangle(p1, p2, p3), p4);
	assert(inside == true);

	/* Define a point container and read points from a file */
	std::vector<Point> pointContainer;
	util::read_points_from_file("points.txt", pointContainer);
	std::cout << "Read " << pointContainer.size() << " points from file." << std::endl;
	for (const auto& p : pointContainer) {
		std::cout << "(" << p.x << ", " << p.y << ")" << std::endl;
	}


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
