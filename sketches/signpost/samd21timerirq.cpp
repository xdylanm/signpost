#include "samd21timerirq.h"

hw_timer_t tcCreate(int id, uint32_t timer_period_us, bool one_shot)
{
  // source clock is always 0
  // the same source clock is muxed between TC pairs: [TCC2, TC3], [TC4, TC5]
  // Not sure how to support 32 bit clock with interrupt (should pair TC4+TC5 but that didn't work)

  hw_timer_t timer;
  timer.one_shot = one_shot;

  // clock period ticks = 1024 clocks / (SystemCoreClock clocks/sec) * (1000000 us/sec) * (1/us))
  timer.period_ticks = (timer_period_us * TCX_CPU_MHZ) / TCX_PRESCALER;

  // currently can't support >16 bit clocks (up to 1s)
  if (timer.period_ticks >= (1 << 16)) {
    return timer;
  }
  
  if (id == 3) {
    timer.TC = TC3;
    timer.clkctrl_reg = (uint16_t) (GCLK_CLKCTRL_CLKEN | GCLK_CLKCTRL_GEN_GCLK0 | GCLK_CLKCTRL_ID_TCC2_TC3);
    timer.irqn_lbl = TC3_IRQn;
  } else if (id == 4) {
    timer.TC = TC4;
    timer.clkctrl_reg = (uint16_t) (GCLK_CLKCTRL_CLKEN | GCLK_CLKCTRL_GEN_GCLK0 | GCLK_CLKCTRL_ID_TC4_TC5);
    timer.irqn_lbl = TC4_IRQn;
  } else if (id == 5) {
    timer.TC = TC5;
    timer.clkctrl_reg = (uint16_t) (GCLK_CLKCTRL_CLKEN | GCLK_CLKCTRL_GEN_GCLK0 | GCLK_CLKCTRL_ID_TC4_TC5);
    timer.irqn_lbl = TC5_IRQn;
  } else {
    return timer;
  }

  return timer;
}


bool tcConfigure(hw_timer_t& timer)
{
  if (!timer.TC) {
    return false;
  }
  // select the generic clock generator used as source to the generic clock multiplexer
  GCLK->CLKCTRL.reg = timer.clkctrl_reg;
  while (GCLK->STATUS.bit.SYNCBUSY);

  //tcReset(timer); //reset
  bool const is_16bit = timer.period_ticks < (1 << 16);
  if (!is_16bit) {
    return false;
  }
  // Set Timer counter Mode to 16 bits, it will become a 16bit counter ('mode1' in the datasheet)
  timer.TC->COUNT16.CTRLA.reg |= TC_CTRLA_MODE_COUNT16;
  // Set TCx waveform generation mode to 'match frequency'
  timer.TC->COUNT16.CTRLA.reg |= TC_CTRLA_WAVEGEN_MFRQ;
  //set prescaler
  //the clock normally counts at the GCLK_TC frequency, but we can set it to divide that frequency to slow it down
  //you can use different prescaler divisons here like TC_CTRLA_PRESCALER_DIV1 to get a different range
  timer.TC->COUNT16.CTRLA.reg |= TC_CTRLA_PRESCALER_DIV1024 | TC_CTRLA_ENABLE; //it will divide GCLK_TC frequency by 1024

  if (timer.one_shot) {
    timer.TC->COUNT16.CTRLBSET.reg |= TC_CTRLBSET_ONESHOT;
  }
  //set the compare-capture register. 
  //The counter will count up to this value (it's a 16bit counter so we use uint16_t)
  //this is how we fine-tune the frequency, make it count to a lower or higher value
  timer.TC->COUNT16.CC[0].reg = (uint16_t) timer.period_ticks;

  while (timer.TC->COUNT16.STATUS.bit.SYNCBUSY);
 
  // Configure interrupt request
  NVIC_EnableIRQ(timer.irqn_lbl);

  // Enable the TCx interrupt request
  timer.TC->COUNT16.INTENSET.bit.MC0 = 1;
  while (timer.TC->COUNT16.STATUS.bit.SYNCBUSY);

  return true;
}

void tcStartCounter(hw_timer_t& timer) 
{
  if (!timer.TC) {
    return;
  }
  timer.TC->COUNT16.CTRLA.reg |= TC_CTRLA_ENABLE; //set the CTRLA register
  while (timer.TC->COUNT16.STATUS.bit.SYNCBUSY);
}

//Reset TC
void tcReset(hw_timer_t& timer)
{
  if (!timer.TC) {
    return;
  }
  timer.TC->COUNT16.CTRLA.reg = TC_CTRLA_SWRST;
  while (timer.TC->COUNT16.STATUS.bit.SYNCBUSY);
  while (timer.TC->COUNT16.CTRLA.bit.SWRST);
}

//disable TC
void tcDisable(hw_timer_t& timer)
{
  if (!timer.TC) {
    return;
  }
  timer.TC->COUNT16.CTRLA.reg &= ~TC_CTRLA_ENABLE;
  while (timer.TC->COUNT16.STATUS.bit.SYNCBUSY);
}
