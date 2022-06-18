
#include "Image.hpp"

namespace WoohooRT
{
  Image::Image(int width, int height)
  {
    m_width = width;
    m_height = height;
    m_aspectRatio = static_cast<float>(width) / height;
  }

} // namespace WoohooRT
