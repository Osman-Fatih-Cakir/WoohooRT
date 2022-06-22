
#include <iostream>
#include <memory>
#include <chrono>

#include "Engine/Renderer.hpp"

int main()
{
  std::shared_ptr<WoohooRT::CPURenderer> cpuRenderer(new WoohooRT::CPURenderer);

  auto start = std::chrono::steady_clock::now();
  cpuRenderer->Render();
  auto end = std::chrono::steady_clock::now();

  std::cerr << "\r\nElapsed Time: " << std::chrono::duration_cast<std::chrono::seconds>(end - start).count() << " seconds.";

  return 0;
}
