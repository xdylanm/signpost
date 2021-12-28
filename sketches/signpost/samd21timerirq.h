// source material
// https://gist.github.com/jdneo/43be30d85080b175cb5aed3500d3f989
// https://gist.github.com/nonsintetic/ad13e70f164801325f5f552f84306d6f
// https://emalliab.wordpress.com/2021/04/16/comparing-timers-on-samd21-and-samd51-microcontrollers/
// https://ww1.microchip.com/downloads/en/DeviceDoc/SAM-D21DA1-Family-Data-Sheet-DS40001882G.pdf

#include <Arduino.h>

#define TCX_CPU_MHZ 48
#define TCX_PRESCALER 1024

struct hw_timer_t
{
  hw_timer_t() : TC(nullptr), clkctrl_reg(0), one_shot(false), period_ticks(1000) {}
  Tc* TC;                 // the counter
  uint16_t clkctrl_reg;   // GCLK (generic clocks) CLKCTRL register
  IRQn_Type irqn_lbl;     // processor specific typedef enum IRQn {...} IRQn_Type;
  bool one_shot;          // flag
  uint32_t period_ticks;  // clock period (units of pre-scaling (1024)/clock rate)
};


hw_timer_t tcCreate(int id, uint32_t timer_period_us, bool one_shot);
bool tcConfigure(hw_timer_t& timer);
void tcStartCounter(hw_timer_t& timer); 
void tcReset(hw_timer_t& timer);
void tcDisable(hw_timer_t& timer);

inline void tcClearIrq(hw_timer_t& timer)
{
  if (!timer.TC) {
    return;
  }
  timer.TC->COUNT16.INTFLAG.bit.MC0 = 1;
}
