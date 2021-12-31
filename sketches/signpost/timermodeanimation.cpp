#include "timermodeanimation.h"


TimerModeAnimation::TimerModeAnimation(Adafruit_NeoPixel& strip_l, Adafruit_NeoPixel& strip_r) 
  : Animation(strip_l, strip_r), _color_code(10)
{
  set_display(0,0);
}

void TimerModeAnimation::set_display(int const num_left, int const num_right)
{
  int offset = 1;
  for (int idigit = 0; idigit < 2; ++idigit) {
    int const digit = idigit == 0 ? num_left : num_right;
    switch (digit) {
    case 0:
      write_digit(&_ani_0[0],offset);
      break;
    case 1:
      write_digit(&_ani_1[0],offset);
      break;
    case 3:
      write_digit(&_ani_3[0],offset);
      break;
    case 5:
      write_digit(&_ani_5[0],offset);
      break;
    default:
      write_digit(&_ani_blank[0],offset);
      break;
    }
    offset += 12;
  }
  set_frame_data(2, 24, &_ani[0]);
}

void TimerModeAnimation::write_digit(int const* digit, int const offset) {
  for (int i = 0; i < 12; ++i) {
    _ani[offset + i] = digit[i] * _color_code;
  }
}

int TimerModeAnimation::_ani[] = {
  29,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  1,   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0};

//                                        0  1  2  3  4  5  6  7  8  9 10 11
int const TimerModeAnimation::_ani_0[] = {1, 1, 0, 0, 1, 0, 1, 0, 1, 1, 0, 0};
int const TimerModeAnimation::_ani_1[] = {0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0};
int const TimerModeAnimation::_ani_3[] = {0, 0, 0, 0, 1, 1, 1, 0, 1, 1, 0, 0};
int const TimerModeAnimation::_ani_5[] = {1, 0, 1, 0, 1, 1, 1, 0, 0, 1, 0, 0};
int const TimerModeAnimation::_ani_blank[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
