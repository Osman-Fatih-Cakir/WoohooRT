
#include "App.hpp"

#include <iostream>
#include <memory>
#include <chrono>

namespace WoohooRT
{
  void App::Start()
  {
    renderer = std::make_shared<CPURenderer>();

    Render(RenderHardware::CPU);
  }

  void App::Render(RenderHardware rh)
  {
    if (rh == RenderHardware::CPU)
    {
      // Start rendering
      renderer->BeginRender();
    }
    else
    {
      std::cerr << "Not implemented rendering hardware type." << std::flush;
    }
  }

  void App::Quit()
  {
    // TODO implement
    // TODO may quit in the middle of the process
  }

} // namespace WoohooRT
