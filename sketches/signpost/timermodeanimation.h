#ifndef timermodeanimation_h_
#define timermodeanimation_h_

#include "animation.h"

class TimerModeAnimation : public Animation
{
public:
  TimerModeAnimation (Adafruit_NeoPixel& strip_l, Adafruit_NeoPixel& strip_r);

  void set_display(int num_left, int num_right);
  bool interruptable() const override { return true; }

private:

  void write_digit(int const* digit, int offset);

  int _color_code;
  
  static int _ani[];
  static int const _ani_blank[];
  static int const _ani_0[];
  static int const _ani_1[];
  static int const _ani_3[];
  static int const _ani_5[];
    
};


#endif
