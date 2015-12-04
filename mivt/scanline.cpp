#include "scanline.h"

namespace mivt {
  bool ScanLine::PointInPolygon(const std::vector<glm::vec2> &polygon, const glm::vec2 &point)
  {
    int polyNumber = (int)polygon.size();
    int i, j = polyNumber - 1;
    bool oddNodes = false;
    for (i = 0; i < polyNumber; i++) {
      if ((polygon[i].y < point.y && polygon[j].y >= point.y
        || polygon[j].y < point.y && polygon[i].y >= point.y)
        && (polygon[i].x <= point.x || polygon[j].x <= point.x)) {
        oddNodes ^= (polygon[i].x + (point.y - polygon[i].y) / (polygon[j].y - polygon[i].y)
          * (polygon[j].x - polygon[i].x) < point.x);
      }
      j = i;
    }
    return oddNodes;
  }
}
