
#include <Adafruit_DotStar.h>
#include <Adafruit_NeoPixel.h>
#include <Bounce2.h>
#include "samd21timerirq.h"
#include "startupanimation.h"
#include "arrowanimation.h"
#include "ltrtarrowmodeanimation.h"
#include "timermodeanimation.h"

// onboard DotStar -- debug
Adafruit_DotStar dspixel (1,INTERNAL_DS_DATA,INTERNAL_DS_CLK,DOTSTAR_BGR);

// mode button
const int MODE_BUTTON_PIN = 0;
Bounce2::Button mode_button = Bounce2::Button();
enum SignpostMode {
  RANDOM_LEFT_RIGHT = 0,
  ALTERNATE_LEFT_RIGHT,
  TIMER_5S,
  TIMER_10S,
  TIMER_15S,
  TIMER_30S,
  NUM_MODES
} op_mode;

uint16_t random_bits= 0;
int random_bits_left = 0;

// NeoPixel strips
const int NEOPIX_RIGHT_PIN = 3;
const int NEOPIX_LEFT_PIN = 4;
const int NUM_PIX = 12;
Adafruit_NeoPixel pixels_right(NUM_PIX, NEOPIX_RIGHT_PIN, NEO_GRBW + NEO_KHZ800);
Adafruit_NeoPixel pixels_left(NUM_PIX, NEOPIX_LEFT_PIN, NEO_GRBW + NEO_KHZ800);

// PIR
const int PIR_PIN = 2;
int pir_state = LOW;
int pir_val = 0;

// timer
hw_timer_t frame_timer;
volatile bool frame_event;
void TC5_Handler() 
{
  frame_event = true;
  tcClearIrq(frame_timer);
}
int const period_ms = 20;  // 50FPS
int timer_timeout_count = -1;
int timer_timeout_count_start = -1;

// animations
Animation* active_ani = nullptr;
StartupAnimation ani0(pixels_left, pixels_right);
ArrowAnimation ani_arrow(pixels_left, pixels_right);
LtRtArrowModeAnimation ani_arrow_lr_mode(pixels_left, pixels_right);
TimerModeAnimation ani_timer_mode(pixels_left, pixels_right);

void setup() {
  Serial.begin(115200);

  pinMode(PIR_PIN, INPUT_PULLUP);

  mode_button.attach( MODE_BUTTON_PIN ,  INPUT_PULLUP ); // Use internal pull-up
  mode_button.interval(5); 
  mode_button.setPressedState(LOW); // pressed when low (short to ground)

  pixels_left.begin();
  pixels_left.show();

  pixels_right.begin();
  pixels_right.show();

  dspixel.begin();
  dspixel.show();

  frame_timer = tcCreate(5,period_ms*1000,false); // 
  if (!tcConfigure(frame_timer)) {
    dspixel.setPixelColor(0, 64, 0, 0);
    dspixel.show();
    while(1);
  }

  tcStartCounter(frame_timer);
  active_ani = &ani0;
  
}

void loop() 
{
  pir_val = digitalRead(PIR_PIN);
  if ((pir_val == HIGH) && (pir_state == LOW)) {//edge
    dspixel.setPixelColor(0, 0, 0, 64);
    dspixel.show();
    if (!active_ani) {
      ani_arrow.reset();
      if (op_mode == RANDOM_LEFT_RIGHT) {
        if (random_bits_left == 0) {
          random_bits = random((1<<16));
        }
        if (random_bits & 0x0001) {
          ani_arrow.set_left_only();
        } else {
          ani_arrow.set_right_only();
        }
        random_bits >>= 1;
        random_bits_left--;
        active_ani = &ani_arrow;
      } else if (op_mode == ALTERNATE_LEFT_RIGHT) {
        ani_arrow.toggle();
        active_ani = &ani_arrow;
      }
    }
    pir_state = HIGH;
  } else if ((pir_val == LOW) && (pir_state == HIGH)) { //edge
    dspixel.setPixelColor(0, 0);
    dspixel.show();
    pir_state = LOW;
  }

  mode_button.update();
  if (mode_button.pressed()) {
    if (!active_ani || active_ani->interruptable()) {
      op_mode = static_cast<SignpostMode>((static_cast<int>(op_mode) + 1) % static_cast<int>(NUM_MODES));
      if (active_ani) {
        active_ani->reset();
      }
      switch(op_mode) {
        case RANDOM_LEFT_RIGHT:
          ani_arrow_lr_mode.set_random();
          active_ani = &ani_arrow_lr_mode;
          timer_timeout_count = -1;
          random_bits_left = 0;
          break;
        case ALTERNATE_LEFT_RIGHT:
          ani_arrow_lr_mode.set_alternate();
          active_ani = &ani_arrow_lr_mode;
          timer_timeout_count = -1;
          break;
        case TIMER_5S:
          ani_timer_mode.set_display(0,5);
          active_ani = &ani_timer_mode;
          timer_timeout_count_start = 5000/period_ms;
          timer_timeout_count = timer_timeout_count_start;
          break;
        case TIMER_10S:
          ani_timer_mode.set_display(1,0);
          active_ani = &ani_timer_mode;
          timer_timeout_count_start = 10000/period_ms;
          timer_timeout_count = timer_timeout_count_start;
          break;
        case TIMER_15S:
          ani_timer_mode.set_display(1,5);
          active_ani = &ani_timer_mode;
          timer_timeout_count_start = 15000/period_ms;
          timer_timeout_count = timer_timeout_count_start;
          break;
        case TIMER_30S:
          ani_timer_mode.set_display(3,0);
          active_ani = &ani_timer_mode;
          timer_timeout_count_start = 30000/period_ms;
          timer_timeout_count = timer_timeout_count_start;
          break;
        default:
          break;
      }
    }
  }

  if (frame_event) {
    if (timer_timeout_count > 0) {
      timer_timeout_count--;
    } else if (timer_timeout_count == 0) {
      timer_timeout_count = timer_timeout_count_start;
      if (!active_ani) {
        ani_arrow.reset();
        ani_arrow.set_both();
        active_ani = &ani_arrow;
      }
    }
    
    if (active_ani && !active_ani->tick()) {
      active_ani = nullptr;
    }
    frame_event = false;
  }

}
