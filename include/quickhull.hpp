#include <common.hpp>

/* QuickHull Implementation */
namespace QuickHullNS {
class QuickHull : public ConvexHull<Points> {
private:
  void findHullRecursive(const Point &p1, const Point &p2, const Points &points,
                         Points &hull, int side_multiplier) const;
protected:
  void findTopHullRecursive(const Point &p1, const Point &p2, const Points &points,
                         Points &hull) const;

  void findBottomHullRecursive(const Point &p1, const Point &p2, const Points &points,
                         Points &hull) const;
public:
  Points compute(const Points &points) const override;
};
} // namespace QuickHullNS
