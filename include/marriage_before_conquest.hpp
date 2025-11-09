#include <common.hpp>

/* QuickHull Implementation */

namespace MarriageNS {
class MarriageBeforeConquest : public ConvexHull<Points> {
protected:
  void MarriageBeforeConquestRecursive(const Points &points, Points &hull);
  Line findBridge(const Points &points, float midX);

public:
  Points compute(const Points &points) override;
};
} // namespace MarriageNS
