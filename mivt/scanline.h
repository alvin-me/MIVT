#pragma once
#include <vector>
#include "tgt_math.h"

namespace mivt {
  class ScanLine
  {
  public:
    /**
    * Point-In-Polygon Algorithm - Determining Whether A Point Is Inside A Complex Polygon
    * The function will return YES if the point x,y is inside the polygon, or
    * NO if it is not.  If the point is exactly on the edge of the polygon,
    * then the function may return YES or NO.
    *
    * Note that division by zero is avoided because the division is protected
    * by the "if" clause which surrounds it.
    *
    * @param polygon: coordinates of corners
    * @param point: point to be tested
    * @return: true if the point is inside the polygon, false otherwise.
    * http://alienryderflex.com/polygon/
    */
    static bool PointInPolygon(const std::vector<glm::vec2> &polygon, const glm::vec2 &point);
  };
}


