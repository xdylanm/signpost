#ifndef arrowanimation_h_
#define arrowanimation_h_

#include "animation.h"

class ArrowAnimation : public Animation
{
public:
  ArrowAnimation(Adafruit_NeoPixel& strip_l, Adafruit_NeoPixel& strip_r);

  void set_left_only();
  void set_right_only();
  void set_both();
  void toggle();

private:
  bool _is_left;
  static int const _ani_left[];
  static int const _ani_right[];
  static int const _ani_both[];
  
};


#endif
