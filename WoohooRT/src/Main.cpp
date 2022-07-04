
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include "App.hpp"

int main()
{
  _CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_DEBUG);

  std::shared_ptr<WoohooRT::App> app(new WoohooRT::App());
  app->Start();

  _CrtDumpMemoryLeaks();

  return 0;
}
