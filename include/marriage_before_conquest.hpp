#include <common.hpp>

/* QuickHull Implementation */

namespace MarriageNS {
class MarriageBeforeConquest : public ConvexHull<Points> {
protected:
  void MBCUpperRecursive(const Points &points, Points &hull) const;
  void MBCLowerRecursive(const Points &points, Points &hull) const;
  Line findUpperBridge(const Points &points) const;
  Line findLowerBridge(const Points &points) const;

public:
  Points compute(const Points &points) const override;
};

class MarriageBeforeConquestV2 : public ConvexHull<Points> {
protected:
  void MBCUpperRecursive(const Points &points, Points &hull) const;
  void MBCLowerRecursive(const Points &points, Points &hull) const;
  Line findUpperBridge(const Points &points, const Line &extremes) const;
  Line findLowerBridge(const Points &points, const Line &extremes) const;

public:
  Points compute(const Points &points) const override;
};

} // namespace MarriageNS
