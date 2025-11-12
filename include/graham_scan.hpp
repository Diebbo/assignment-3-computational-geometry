#ifndef GRAHAM_SCAN_HPP
#define GRAHAM_SCAN_HPP

#include <common.hpp>

/* Gramham Scan Implementation */
class GrahamScan : public ConvexHull<Points> {
public:
  Points compute(const Points &points) const override;
};

#endif // GRAHAM_SCAN_HPP
