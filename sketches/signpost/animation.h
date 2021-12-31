#ifndef animation_h_
#define animation_h_

#include <Adafruit_NeoPixel.h>

class Animation 
{
public:
  Animation(Adafruit_NeoPixel& strip_left, Adafruit_NeoPixel& strip_right);
  
  static uint32_t const color_wheel[];
  static int const num_colors;
  bool tick(); 
  void reset();
  bool is_playing() const;
  virtual bool interruptable() const;
  
protected:
  void set_frame_data(int nframes, int npx, int const* data);
  
private:
  Adafruit_NeoPixel& _strip_left;
  Adafruit_NeoPixel& _strip_right;
  int const* _frame_data;
  int _num_frames;
  int _px_per_frame;
  int _cur_frame_remain; 
  int _ndx_cur_frame;
};

#endif
