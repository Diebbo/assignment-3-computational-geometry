#include "common.hpp"
#include <string>

namespace util {
    /* Determine if point p is above line l
     *
     * A point is considered "above" a line if it lies on the side of the line
     * that is opposite to the direction of the line's normal vector.
     *
     * Parameters:
     *  - p: The point to check
     *  - l: The line to compare against
     *
     * Returns:
     *  > 0 if point is above the line
     *  < 0 if point is below the line
     *  = 0 if point is on the line
     */
    float sidedness(const Line& l, const Point& p) ; // sidedness(Line(p1,p2), p3);
    float sidedness(const Point& p1, const Point& p2, const Point& p3);


    /* Determine if point p is inside triangle t
     *
     * A point is considered "inside" a triangle if it lies within the area
     * bounded by the triangle's three edges.
     *
     * Parameters:
     *  - t: The triangle to check against
     *  - p: The point to check
     *
     * Returns:
     *  true if point is inside the triangle
     *  false otherwise
     */
    bool is_inside(const Triangle& t, const Point& p);

    /* Read points from a file
     *
     * The file should contain points in the following format:
     * x1 y1
     * x2 y2
     * ...
     *
     * Parameters:
     *  - filename: The name of the file to read from
     *  - container: The container to store the points in (e.g., std::vector<Point>)
     */
    template<typename T>
    void read_points_from_file(const std::string& filename, T& container);
    
}

