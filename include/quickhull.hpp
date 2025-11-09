#include <common.hpp>

/* QuickHull Implementation */
namespace QuickHullNS {
class QuickHull : public ConvexHull<Points> {
protected:
  void QuickHullRecursive(const Point &p1, const Point &p2,
                          const Points &points, Points &hull);

public:
  Points compute(const Points &points) override;
};
} // namespace QuickHullNS
