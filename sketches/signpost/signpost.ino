
#include <Adafruit_DotStar.h>
#include <Adafruit_NeoPixel.h>
//#include <Bounce2.h>
#include "samd21timerirq.h"
#include "startupanimation.h"

// onboard DotStar -- debug
Adafruit_DotStar dspixel (1,INTERNAL_DS_DATA,INTERNAL_DS_CLK,DOTSTAR_BGR);

// mode button
const int MODE_BUTTON_PIN = 0;
//Bounce2::Button mode_button = Bounce2::Button();

bool button_state = false;

// NeoPixel strip
const int NEOPIX_RIGHT_PIN = 3;
const int NEOPIX_LEFT_PIN = 4;
const int NUM_PIX = 12;
Adafruit_NeoPixel pixels_right(NUM_PIX, NEOPIX_RIGHT_PIN, NEO_GRBW + NEO_KHZ800);

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

StartupAnimation ani0(pixels_right);

void setup() {
  Serial.begin(115200);

  pinMode(PIR_PIN, INPUT_PULLUP);
  pinMode(MODE_BUTTON_PIN, INPUT_PULLUP);

//  mode_button.attach( MODE_BUTTON_PIN ,  INPUT_PULLUP ); // Use internal pull-up
//  mode_button.interval(5); 
//  mode_button.setPressedState(LOW); // pressed when low (short to ground)

  pixels_right.begin();
  pixels_right.show();
  pixels_right.setBrightness(63);

  dspixel.begin();
  dspixel.show();

  frame_timer = tcCreate(5,33333,false); // 30FPS
  if (!tcConfigure(frame_timer)) {
    dspixel.setPixelColor(0, 64, 0, 0);
    dspixel.show();
    while(1);
  }

  tcStartCounter(frame_timer);
}

bool is_pressed = false;
int pressed_start = -1;
int released_start = -1;
bool is_released = true;
int debounce_time_ms = 20;

void loop() 
{
//  pir_val = digitalRead(PIR_PIN);
//  if (pir_val == HIGH) {
//    if (pir_state == LOW) { //edge
//      dspixel.setPixelColor(0, color_wheel[ndx_color]/2);
//      dspixel.show();
//      Serial.println("PIR ON");
//      pir_state = HIGH;
//    }
//  } else {
//    if (pir_state == HIGH) { //edge
//      dspixel.setPixelColor(0, 0);
//      dspixel.show();
//      Serial.println("PIR off");
//      pir_state = LOW;
//    }
//  }

  int const pin_val = digitalRead(MODE_BUTTON_PIN);
  if (is_released && (pin_val == 0)) {
    if (pressed_start < 0) {
      pressed_start = millis();
    } else if (!is_pressed && (millis() - pressed_start > debounce_time_ms)) {
      is_pressed = true; 
      is_released = false;  
      pressed_start = -1;

      // do stuff
      button_state = !button_state;
      dspixel.setPixelColor(0, 0, button_state ? 64 : 0, 0);
      dspixel.show();
      if (!ani0.is_playing()) {
        ani0.reset();
      }
    }
  } else if (is_pressed && (pin_val == 1)) {  // releasing?
    if (released_start < 0) {
      released_start = millis();
    } else if (!is_released && (millis() - released_start > debounce_time_ms)) {
      is_released = true;   
      released_start = -1;
      is_pressed = false;
    }
  }

//  mode_button.update();
//  if (mode_button.pressed()) {
//    button_state = !button_state;
//    dspixel.setPixelColor(0, 0, button_state ? 64 : 0, 0);
//    dspixel.show();
//    if (!ani0.is_playing()) {
//      ani0.reset();
//    }
//  }

  if (frame_event) {
    ani0.tick();
    frame_event = false;
  }

}

  
