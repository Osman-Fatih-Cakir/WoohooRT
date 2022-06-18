
#include <iostream>
#include <memory>

#include "Engine/Renderer.hpp"

int main()
{
  std::shared_ptr<WoohooRT::CPURenderer> cpuRenderer(new WoohooRT::CPURenderer);
  cpuRenderer->Render();

  return 0;
}
