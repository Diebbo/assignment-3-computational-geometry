= Graham's Scan <app:graham>

```cpp
bool point_cmp(const Point &a, const Point &b) {
  if (a.x != b.x) {
    return a.x < b.x;
  } else {
    return a.y > b.y;
  }
}

bool turns(float side, const Point &a, const Point &b, const Point &c) {
  return side * util::sidedness(Line(a, c), b) <= 0;
}

void compute_half(vector<Point> const& points, vector<Point> &half, float side) {
  half.clear();
  half.push_back(points[0]);
  half.push_back(points[1]);
  for (size_t i = 2; i < points.size(); i++) {
    while (half.size() >= 2) {
      Point const& m1 = half.back();
      Point const& m2 = *std::prev(half.end(), 2);

      if (turns(side, m2, m1, points[i]))
        half.pop_back();
      else
        break;
    }

    half.push_back(points[i]);
  }
}

vector<Point> GrahamScan::compute(const vector<Point> &points) const {
  if (points.size() <= 2) points;

  vector<Point> pts(points.begin(), points.end());
  std::sort(pts.begin(), pts.end(), point_cmp);

  T upper; compute_half(pts, upper, 1.0);
  T lower; compute_half(pts, lower, -1.0);

  lower.pop_back();
  for (int i = lower.size() - 2; i >= 0; i--) {
    upper.push_back(lower.back());
    lower.pop_back();
  }

  return upper;
}
```
