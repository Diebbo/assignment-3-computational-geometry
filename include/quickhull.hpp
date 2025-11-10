#include <common.hpp>

/* QuickHull Implementation */
namespace QuickHullNS {
class QuickHull : public ConvexHull<Points> {
private:
  void findHullRecursive(const Point &p1, const Point &p2, const Points &points,
                         Points &hull, int side_multiplier);
protected:
  void findTopHullRecursive(const Point &p1, const Point &p2, const Points &points,
                         Points &hull);

  void findBottomHullRecursive(const Point &p1, const Point &p2, const Points &points,
                         Points &hull);
public:
  Points compute(const Points &points) override;
};
} // namespace QuickHullNS
