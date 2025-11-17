#ifndef GRAHAM_SCAN_HPP
#define GRAHAM_SCAN_HPP

#include <common.hpp>
#include <list>
#include <vector>

/* Gramham Scan Implementation */
template <typename Points>
class GrahamScan : public ConvexHull<Points> {
public:
  Points compute(const std::vector<Point> &points) const override;
};

std::list<Point> compute_list(const std::vector<Point> &points);

#endif // GRAHAM_SCAN_HPP
