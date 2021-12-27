
#include <Adafruit_DotStar.h>
#include <Adafruit_NeoPixel.h>
#include <Bounce2.h>

// onboard DotStar -- debug
const int DATA_PIN = 7;
const int CLK_PIN = 8;
Adafruit_DotStar dspixel (1,DATA_PIN,CLK_PIN,DOTSTAR_BGR);

// mode button
const int MODE_BUTTON_PIN = 0;
Bounce2::Button mode_button = Bounce2::Button();

// NeoPixel strip
const int NEOPIX_RIGHT_PIN = 3;
const int NEOPIX_LEFT_PIN = 4;
const int NUM_PIX = 12;
Adafruit_NeoPixel pixels_right(NUM_PIX, NEOPIX_RIGHT_PIN, NEO_GRBW + NEO_KHZ800);
const uint32_t color_wheel[] = {
  0x00ff0000,
  0x00ff8000,
  0x00ffff00,
  0x0080ff00,
  0x0000ff00,
  0x0000ff80,
  0x0000ffff,
  0x000080ff,
  0x000000ff,
  0x008000ff,
  0x00ff00ff,
  0x00ff0080};
int ndx_color = 0;
const int num_color = 12;

int lit_pix = 0;

// PIR
const int PIR_PIN = 2;
int pir_state = LOW;
int pir_val = 0;


void draw_test()
{
  pixels_right.clear();
  for (int i = 0; i <= lit_pix; ++i) {
    pixels_right.setPixelColor(i,color_wheel[ndx_color]);
  }
  pixels_right.show();
}


void setup() {
  Serial.begin(115200);

  pinMode(PIR_PIN, INPUT_PULLUP);

  mode_button.attach( MODE_BUTTON_PIN ,  INPUT_PULLUP ); // Use internal pull-up
  mode_button.interval(20); 
  mode_button.setPressedState(LOW); // pressed when low (short to ground)

  pixels_right.begin();
  pixels_right.show();
  pixels_right.setBrightness(63);

  dspixel.begin();
  dspixel.show();
}

void loop() {
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
    
  mode_button.update();
  if (mode_button.pressed()) {
    ndx_color = (ndx_color + 1) % num_color;
    Serial.printf("ndx_color=%d\n",ndx_color);
  }

  draw_test();
  lit_pix = (lit_pix + 1) % NUM_PIX;
  delay(500);

  
}
  
