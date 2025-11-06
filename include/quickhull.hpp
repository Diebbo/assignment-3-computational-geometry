#include "util.hpp"

/* QuickHull Implementation */

namespace QuickHullNS {
class QuickHull : public ConvexHull<std::vector<Point>> {
protected:
    void QuickHullRecursive(const Point& p1, const Point& p2, const std::vector<Point>& points, std::vector<Point>& hull);
public:
    std::vector<Point> compute(const std::vector<Point>& points) override;
};
}
