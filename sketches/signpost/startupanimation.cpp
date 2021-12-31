#include "startupanimation.h"

StartupAnimation::StartupAnimation(Adafruit_NeoPixel& strip_l, Adafruit_NeoPixel& strip_r) 
  : Animation(strip_l, strip_r)
{
  set_frame_data(6, 24, &_ani[0]);
}

int const StartupAnimation::_ani[] = {
  3, 5, 0, 0, 0, 0, 0, 5, 5, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 5, 5, 0, 0, 0, 0,
  4, 5, 7, 0, 0, 0, 7, 5, 5, 7, 0, 0, 0, 5, 7, 0, 0, 0, 7, 5, 5, 7, 0, 0, 0,
  5, 5, 7, 9, 0, 9, 7, 5, 5, 7, 9, 0, 0, 5, 7, 9, 0, 9, 7, 5, 5, 7, 9, 0, 0,
  6, 5, 7, 9, 11, 9, 7, 5, 5, 7, 9, 11, 0, 5, 7, 9, 11, 9, 7, 5, 5, 7, 9, 11, 0,
  15, 5, 7, 9, 11, 9, 7, 5, 5, 7, 9, 11, 13, 5, 7, 9, 11, 9, 7, 5, 5, 7, 9, 11, 13,
  1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  
