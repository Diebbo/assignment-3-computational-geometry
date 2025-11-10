#include <common.hpp>

/* QuickHull Implementation */

namespace MarriageNS {
class MarriageBeforeConquest : public ConvexHull<Points> {
protected:
  void MBCUpperRecursive(const Points &points, Points &hull);
  void MBCLowerRecursive(const Points &points, Points &hull);
  Line findUpperBridge(const Points &points);
  Line findLowerBridge(const Points &points);

public:
  Points compute(const Points &points) override;
};
} // namespace MarriageNS
