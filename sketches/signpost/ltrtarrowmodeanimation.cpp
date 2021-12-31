#include "ltrtarrowmodeanimation.h"


LtRtArrowModeAnimation::LtRtArrowModeAnimation(Adafruit_NeoPixel& strip_l, Adafruit_NeoPixel& strip_r) 
  : Animation(strip_l, strip_r)
{
  set_alternate();
}

void LtRtArrowModeAnimation::reset(int t0, int t1, int t2, int t3)
{
  _ani[0] = t0;
  _ani[25] = t1;
  _ani[50] = t2;
  _ani[75] = t3;

  for (int f = 0; f < 4; ++f) {
    for (int i = 1; i < 25; ++i) {
      _ani[f*25 + i] = 0;
    }
  }
}

// blinks 15:"< >", 5:" ", 15:"< >", 5:" "
void LtRtArrowModeAnimation::set_random()
{
  
  reset(15,5,15,5);
  for (int frame = 0; frame < 3; frame += 2) {
    for (int i = 0; i < 12; ++i) {
      _ani[frame*25 + 1 + i] = _ani_small_arrow[i];
      _ani[frame*25 + 13 + i] = _ani_small_arrow[i];
    }
  }
  set_frame_data(5, 24, &_ani[0]);
}

// blinks 10:"<  ", 10:" >", 10:"<  ", 10:" >"
void LtRtArrowModeAnimation::set_alternate()
{
  reset(10,10,10,10);
  for (int frame = 0; frame < 4; frame++) {
    for (int i = 0; i < 12; ++i) {
      _ani[frame*25 + 1 + 12*(frame % 2) + i] = _ani_small_arrow[i];
    }
  }
  set_frame_data(5, 24, &_ani[0]);
}

//                                              0   1   2   3   4   5   6   7   8   9   10  11
int const LtRtArrowModeAnimation::_ani_small_arrow[] = {0, 10,  0,  0, 10, 10,  0,  0,  0,  0,  0,  0};

int LtRtArrowModeAnimation::_ani[] = {
  //   0   1   2   3   4   5   6   7   8   9  10  11   
  10,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  10,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  10,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  10,  0,  0,  0,  0,  0,  0,  0,  0,  0,  5,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  1,   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0};
