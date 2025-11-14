#include "common.hpp"
#include <limits>
#include <util.hpp>

namespace util {
float sidedness(const Point &p1, const Point &p2, const Point &p3) {
  return (p2.x - p1.x) * (p3.y - p1.y) - (p2.y - p1.y) * (p3.x - p1.x);
}

float sidedness(const Line &l, const Point &p) {
  return sidedness(l.p1, l.p2, p);
}

bool isLeft(const Point &p1, const Point &p2, const Point &p3) {
  return (p3.x - p2.x) * (p1.y - p2.y) - (p3.y - p2.y) * (p1.x - p2.x) > std::numeric_limits<float>::epsilon();
}
bool isRight(const Point &p1, const Point &p2, const Point &p3) {
  return (p3.x - p2.x) * (p1.y - p2.y) - (p3.y - p2.y) * (p1.x - p2.x) < -std::numeric_limits<float>::epsilon();
}

bool isLeft(const Line &l, const Point &p) { return isLeft(l.p1, l.p2, p); }

bool isRight(const Line &l, const Point &p) { return isRight(l.p1, l.p2, p); }

bool is_inside(const Triangle &t, const Point &p) {
  /* The point must be on the same side of all the triangle's edges */
  float d1 = sidedness(t.p1, t.p2, p);
  float d2 = sidedness(t.p2, t.p3, p);
  float d3 = sidedness(t.p3, t.p1, p);

  // std::cout << "d1: " << d1 << ", d2: " << d2 << ", d3: " << d3 << std::endl;

  bool all_positive = (d1 > 0) && (d2 > 0) && (d3 > 0);
  bool all_negative = (d1 < 0) && (d2 < 0) && (d3 < 0);
  return all_positive || all_negative;
}

float partial_distance(const Line &l, const Point &p) {
  // https://en.wikipedia.org/wiki/Distance_from_a_point_to_a_line#Line_defined_by_an_equation
  // but we are not considering the denominator as it would be the same for all
  // points
  return std::abs((l.p2.y - l.p1.y) * p.x - (l.p2.x - l.p1.x) * p.y +
                  l.p2.x * l.p1.y - l.p2.y * l.p1.x);
}

bool is_inside(const Points &polygon, const Point &p) {
  size_t n = polygon.size();
  if (n < 3) {
    return false; // A polygon must have at least 3 vertices
  }

  float initial_side = sidedness(polygon[0], polygon[1], p);
  for (size_t i = 1; i < n; ++i) {
    float current_side = sidedness(polygon[i], polygon[(i + 1) % n], p);
    if (initial_side * current_side < 0) {
      return false; // Point is on the opposite side of an edge
    }
  }
  return true; // Point is inside the polygon
}

bool is_partial_inside(const Points &polygon, const Point &p) {
  size_t n = polygon.size();
  if (n < 3) {
    return false; // A polygon must have at least 3 vertices
  }

  float initial_side = sidedness(polygon[0], polygon[1], p);
  for (size_t i = 1; i < n - 1; ++i) {
    float current_side = sidedness(polygon[i], polygon[(i + 1) % n], p);
    if (initial_side * current_side < 0) {
      return false; // Point is on the opposite side of an edge
    }
  }
  return true; // Point is inside the polygon
}

bool is_hull(const Points &hull, const Points &points) {
  for (const auto &p : points) {
    bool on_hull = false;

    // Check if point is on hull
    for (const auto &hp : hull) {
      if (p == hp) {
        on_hull = true;
        break;
      }
    }

    if (!on_hull) {
      // Check if point is inside hull
      // A point is inside if it's on the same side of ALL edges
      if (!is_inside(hull, p)) {
        return false;
      }
    }
  }
  return true;
}

bool is_partial_hull(const Points &hull, const Points &points) {
  for (const auto &p : points) {
    bool on_hull = false;

    // Check if point is on hull
    for (const auto &hp : hull) {
      if (p.x == hp.x && p.y == hp.y) {
        on_hull = true;
        break;
      }
    }

    if (!on_hull) {
      // Check if point is inside hull
      // A point is inside if it's on the same side of ALL edges
      if (!is_partial_inside(hull, p)) {
        return false;
      }
    }
  }
  return true;
}

template <typename T>
void print_points(const T &points) {
  for (const auto &p : points) {
    std::cout << "(" << p.x << ", " << p.y << ")  ";
  }
  std::cout << std::endl;
}

template <typename T>
bool is_valid_inside(const T &polygon, const Point &p) {
  size_t n = polygon.size();
  if (n < 3) {
    return false; // A polygon must have at least 3 vertices
  }

  for (auto it = polygon.begin(); it != polygon.end(); ++it) {
    auto it2 = it;
    auto p1 = *it2;
    if (++it2 == polygon.end()) it2 = polygon.begin();
    auto p2 = *it2;
    if (isLeft(Line(p1, p2), p)) {
      return false; // Point is on the left side of an edge
    }
  }
  return true; // Point is inside the polygon
}

template <typename T>
bool is_valid_hull(const T &hull, const Points &points) {
  // check that the hull is convex
  size_t n = hull.size();
  if (n < 3) {
    std::cout << "Hull has less than 3 vertices." << std::endl;
    return false; // A hull must have at least 3 vertices
  }

  auto it = hull.begin();
  for (size_t i = 0; i < n; ++i) {
    auto it2 = it;
    auto i1 = *it2;
    if (++it2 == hull.end()) it2 = hull.begin();
    auto i2 = *it2;
    if (++it2 == hull.end()) it2 = hull.begin();
    auto i3 = *it2;

    if (util::isLeft(i1, i2, i3)) {
      print_points(hull);

      std::cout << "Hull is not convex: "
                << i << " " << sidedness(i1, i2, i3) << std::endl
                << "a=Point(" << i1.x << ", " << i2.y << "); "
                << "b=Point(" << i2.x << ", " << i2.y << "); "
                << "c=Point(" << i3.x << ", " << i3.y << ")" << std::endl;

      return false; // Hull is not convex
    }
  }

  // check that all points are either on the hull or inside it
  for (const auto &p : points) {
    bool on_hull = false;

    // Check if point is on hull
    for (const auto &hp : hull) {
      if (p == hp) {
        on_hull = true;
        break;
      }
    }

    if (!on_hull) {
      // Check if point is inside hull
      // A point is inside if it's on the same side of ALL edges
      if (!is_valid_inside<T>(hull, p)) {
        std::cout << "Point (" << p.x << ", " << p.y << ") is outside the hull."
                  << std::endl;
        return false;
      }
    }
  }

  return true;
}

template bool is_valid_hull<Points>(const Points &hull, const Points &points);
template bool is_valid_hull<PointsList>(const PointsList &hull, const Points &points);

} // namespace util
