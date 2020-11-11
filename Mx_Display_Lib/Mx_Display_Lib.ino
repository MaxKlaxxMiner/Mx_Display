
// --- Pin-Tests ---
#include "Tests/PinTest.h"

int main()
{
  init(); // init vanilla-routines
  setup();
  for (;;) loop();
  return 0;
}
