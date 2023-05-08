#include <fxcg/display.h>
#include <fxcg/keyboard.h>
int main() {
  PrintXY(3,8,"--Hello World!", TEXT_MODE_NORMAL, TEXT_COLOR_RED); // Place it somewhere on the screen.  Note that x,y are 1-indexed.
  int key;
  while(1)
    GetKey(&key); // Loop forever

  return 0; // Needed if you have main returning int, but will never be reached.
};
