
#include "Types.hpp"

namespace WoohooRT
{
  class Image
  {
  public:
    Image(int width, int height);

  public:
    float m_aspectRatio;
    int m_width;
    int m_height;
  };

} // namespace WoohooRT
