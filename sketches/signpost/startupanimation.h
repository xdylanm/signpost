#ifndef startupanimation_h_
#define startupanimation_h_

#include "animation.h"

class StartupAnimation : public Animation
{
public:
  StartupAnimation(Adafruit_NeoPixel& strip);

private:
  static int const _ani[];
  
};



#endif
