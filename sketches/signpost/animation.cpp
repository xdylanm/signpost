#include "animation.h"


uint32_t const Animation::color_wheel[] = {
  0x00000000,
  0x00ff0000,
  0x00ff8000,
  0x00ffff00,
  0x0080ff00,
  0x0000ff00,
  0x0000ff40,
  0x0000ffff,
  0x000080ff,
  0x000000ff,
  0x008000ff,
  0x00ff00ff,
  0x00ff0080,
  0x00ffffff};
int const Animation::num_colors = 14;



Animation::Animation(Adafruit_NeoPixel& strip_l, Adafruit_NeoPixel& strip_r) 
: _strip_left(strip_l), _strip_right(strip_r), _frame_data(nullptr), _num_frames(0), 
  _px_per_frame(0), _cur_frame_remain(0), _ndx_cur_frame(0)
{

  
}

bool Animation::is_playing() const
{
  return !((_cur_frame_remain == 0) && (_ndx_cur_frame >= _num_frames));
}

bool Animation::interruptable() const
{
  return false;
}

bool Animation::tick() 
{
  if (!_frame_data) {
    return false;
  }
  if (_cur_frame_remain == 0) { // load next frame
    _ndx_cur_frame++;
    if (_ndx_cur_frame >= _num_frames) {
      return false;  // done
    }
    _cur_frame_remain = _frame_data[_ndx_cur_frame*(_px_per_frame+1)];  // first entry in a row is the display time
  }
  
  _strip_left.clear();
  _strip_right.clear();
  for (int i = 0; i < _px_per_frame; ++i) {
    int const ndx = _frame_data[_ndx_cur_frame*(_px_per_frame+1) + i + 1];
    if (i < 12) {
      _strip_left.setPixelColor(i, color_wheel[ndx]);
    } else {
      _strip_right.setPixelColor(i-12, color_wheel[ndx]);
    }
  }
  _cur_frame_remain -= 1;
  _strip_left.show();
  _strip_right.show();

  return true;
}

void Animation::set_frame_data(int nframes, int npx, int const* data)
{
  _num_frames = nframes;
  _px_per_frame = npx;
  _ndx_cur_frame = 0;
  _cur_frame_remain = data[0];
  _frame_data = data;
}

void Animation::reset() 
{
  if (!_frame_data) {
    return;
  }
  _ndx_cur_frame = 0;
  _cur_frame_remain = _frame_data[0];
}
 
