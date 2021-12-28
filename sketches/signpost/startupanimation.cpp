#include "startupanimation.h"

StartupAnimation::StartupAnimation(Adafruit_NeoPixel& strip) : Animation(strip)
{
  set_frame_data(6, 12, &_ani[0]);
}

int const StartupAnimation::_ani[] = {
  6, 5, 0, 0, 0, 0, 0, 5, 5, 0, 0, 0, 0,
  6, 5, 7, 0, 0, 0, 7, 5, 5, 7, 0, 0, 0,
  6, 5, 7, 9, 0, 9, 7, 5, 5, 7, 9, 0, 0,
  6, 5, 7, 9, 11, 9, 7, 5, 5, 7, 9, 11, 0,
  15, 5, 7, 9, 11, 9, 7, 5, 5, 7, 9, 11, 13,
  1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  
