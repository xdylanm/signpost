#ifndef ltrtarrowmodeanimation_h_
#define ltrtarrowmodeanimation_h_

#include "animation.h"

class LtRtArrowModeAnimation : public Animation
{
public:
  LtRtArrowModeAnimation(Adafruit_NeoPixel& strip_l, Adafruit_NeoPixel& strip_r);

  void set_random();
  void set_alternate();

  bool interruptable() const override { return true; }
private:

  void reset(int t0, int t1, int t2, int t3);

  static int _ani[];
  static int const _ani_small_arrow[];
  
};


#endif
