#include <algorithm>
#include <cassert>
#include <common.hpp>
#include <graham_scan.hpp>
#include <util.hpp>
#include <vector>

bool point_cmp(const Point &a, const Point &b) {
  if (a.x != b.x) {
    return a.x < b.x;
  } else {
    return a.y > b.y;
  }
}

/**
 * Returns whether the three given points make the expected turn. The `side`
 * parameter can be:
 * - `1`, meaning that the three points make a right turn, or
 * - `-1`, meaning that they do a left turn
 */
bool turns(float side, const Point &a, const Point &b, const Point &c) {
  auto sidedness = util::sidedness(Line(a, c), b);
  return sidedness * side <= 0;
}

template <typename T>
void compute_inner(const Points &points, T &half, float side) {
  half.clear();
  half.push_back(points[0]);
  half.push_back(points[1]);
  for (size_t i = 2; i < points.size(); i++) {
    while (half.size() >= 2) {
      Point const& m1 = half.back();
      Point const& m2 = *std::prev(half.end(), 2);

      if (turns(side, m2, m1, points[i])) {
        half.pop_back();
      } else {
        break;
      }
    }

    half.push_back(points[i]);
  }
}

template<typename T>
T GrahamScan<T>::compute(const std::vector<Point> &points) const {
  if (points.size() <= 2)
    return T(points.begin(), points.end());

  std::vector<Point> pts(points.begin(), points.end());
  std::sort(pts.begin(), pts.end(), point_cmp);

  T upper;
  compute_inner(pts, upper, 1.0);
  T lower;
  compute_inner(pts, lower, -1.0);

  lower.pop_back();
  for (int i = lower.size() - 2; i >= 0; i--) {
    upper.push_back(lower.back());
    lower.pop_back();
  }

  return upper;
}

template<>
PointsDeque GrahamScan<PointsDeque>::compute(Points const& points) const {
  if (points.size() <= 2)
    return PointsDeque(points.begin(), points.end());

  std::vector<Point> pts(points.begin(), points.end());
  std::sort(pts.begin(), pts.end(), point_cmp);

  PointsDeque res;
  res.push_back(pts[0]);
  res.push_back(pts[1]);
  res.push_front(pts[1]);
  int uh = 2, lh = 2;
  for (size_t i = 2; i < pts.size(); i++) {
    // Upper
    while (uh >= 2) {
      Point const& m1 = res.back();
      Point const& m2 = *std::prev(res.end(), 2);
      if (turns(1.0, m2, m1, pts[i])) {
        res.pop_back();
        uh -= 1;
      } else {
        break;
      }
    }
    res.push_back(pts[i]);
    uh += 1;

    // Lower
    while (lh >= 2) {
      Point const& m1 = res.front();
      Point const& m2 = * ++res.begin();
      if (turns(-1.0, m2, m1, pts[i])) {
        res.pop_front();
        lh -= 1;
      } else {
        break;
      }
    }
    res.push_front(pts[i]);
    lh += 1;
  }

  res.pop_back();
  std::rotate(res.begin(), res.begin()+lh-1, res.end());
  return res;
}

template Points GrahamScan<Points>::compute(const std::vector<Point> &points) const;
template PointsList GrahamScan<PointsList>::compute(const std::vector<Point> &points) const;
