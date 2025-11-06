#include "common.hpp"

/* Gramham Scan Implementation */
class GrahamScan : public ConvexHull<std::vector<Point>> {
public:
    std::vector<Point> compute(const std::vector<Point>& points) override {
        // Implementation of Graham Scan algorithm to compute the convex hull
        std::vector<Point> hull;
        // ... algorithm implementation goes here ...
        std::cout << "Computing convex hull using Graham Scan..." << std::endl;
        return hull;
    }
};
