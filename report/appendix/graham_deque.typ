= Graham's Scan - single pass version <app:graham_deque>

```cpp
std::deque<Point> GrahamScan::compute(std::vector<Point> const& points) const {
  if (points.size() <= 2)
    return std::deque<Point>(points.begin(), points.end());

  std::vector<Point> pts(points.begin(), points.end());
  std::sort(pts.begin(), pts.end(), point_cmp);

  std::deque<Point> res;
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
```
