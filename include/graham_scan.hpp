#ifndef GRAM_SCAN_CONVEX_HULL_HPP
#define GRAM_SCAN_CONVEX_HULL_HPP
#include "common.hpp"

/* Gramham Scan Implementation */
class GrahamScan : public ConvexHull<Points> {
public:
  Points compute(const Points &points) override;
};
#endif // GRAM_SCAN_CONVEX_HULL_HPP
