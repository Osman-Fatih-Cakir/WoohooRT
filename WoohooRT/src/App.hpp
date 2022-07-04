#pragma once

#include "Engine/Renderer.hpp"

namespace WoohooRT
{
  enum class RenderHardware
  {
    CPU,
    GPU
  };

  class App
  {
  public:
    void Start();
    void Render(RenderHardware rh = RenderHardware::CPU);
    void Quit();

  private:
    std::shared_ptr<WoohooRT::Renderer> renderer;
  };

} // WoohooRT
