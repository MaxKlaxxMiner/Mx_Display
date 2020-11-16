
// --- Pin-Tests ---
//#include "Tests/PinTest.h"

// --- SerialDisplay-Tests ---
#include "Tests/DisplayTestSerial1.h"

int main()
{
  init(); // init vanilla-routines
  setup();
  for (;;) loop();
  return 0;
}
