#ifndef GRAM_SCAN_CONVEX_HULL_HPP
#define GRAM_SCAN_CONVEX_HULL_HPP
#include "common.hpp"

/* Gramham Scan Implementation */
class GrahamScan : public ConvexHull<std::vector<Point>> {
public:
    std::vector<Point> compute(const std::vector<Point>& points) override;

};
#endif // GRAM_SCAN_CONVEX_HULL_HPP
