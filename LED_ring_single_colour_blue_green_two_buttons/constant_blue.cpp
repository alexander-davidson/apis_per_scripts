

// library for neopixel LEDs
#include <Adafruit_NeoPixel.h>

// specifies AVR microcontroller (arduino, as opposed to other models with different microcontrollers)
#ifdef __AVR__
  #include <avr/power.h>
#endif

// which pin is outputs to LEDs
#define PIN 12

// define how many rings there are
int number_of_rings = 1;

// define how many LEDs there are in each
int number_of_leds = 14;

// number of pixels (i.e. LEDs in ring, put number of LEDs in array times number arrays)
#define NUMPIXELS number_of_leds * number_of_rings

// parameters are number of pixels, which pin, pixel type (Neo, Green, Red, Blue), kHz datastream (800 kHz)
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

// set input pin for button to start showing colour
int ring_button = 8;

// set the value for duration of each colour
unsigned long del_val = 1200000;

unsigned long reward_del_val = 5000;

unsigned long acclimation_delay = 15000;

unsigned long stick_delay = 10000;

unsigned long rest_delay = 10000;

// for while loop to turn of colours until button press
bool running = true;

// set inital button state to off
bool ring_buttonState = false;

// set initial pixel (LED in the ring) to minus number of LEDs in array
// so that the button press adds the value that tells the arduino
// to start (the first LED is pixel 0)
int ring = - number_of_leds;

////////////////////////////////////////////////////////////
////////////////// FOR COLOUR CONTROL //////////////////////
////////////////////////////////////////////////////////////

// the imput pin for the button to change condition
int col_button = 7;

// setting current state to HIGH
int col_button_state = HIGH;

// the current reading from the input pin
int reading;

// the previous state of the button
int col_button_prev = LOW;

// will store the time from last button press
unsigned long time = 0;

// the debounce time forthe button
unsigned long debounce = 200UL;

// set on board LED so we can tell which colour comes first
// if LED is on, then green comes first
#define obled 13
#define redled 11

void setup() {
  // initialises Adafruit Neopixel library
  pixels.begin();
  pinMode(col_button, INPUT);
  pinMode(ring_button, INPUT);
  pinMode(redled, OUTPUT);
  pinMode(obled, OUTPUT);
}

void loop() {
  // fill the ring. Parameters are: RGB value, starting LED (from 0), number of LEDs.
  running = true;
  // only run the code if the button has set 'ring' to 0
  // this avoids waiting for the delay commands to finish
  // before you can trigger the LEDs
  if (ring >= 0){
    if (col_button_state == HIGH){

      

      pixels.fill(pixels.Color(0, 0, 255/4), ring, number_of_leds);
      // sends colour info to LEDs to be displayed
      pixels.show();

      delay(del_val);

      // IMPORTANT!! The reward_del_val is the time to deliver reward. In the Lichtenstein
      // paper they used single colour. For example, in a 10 second stimulus, the light
      // lasted for 13 seconds, the last three for the reward. This is the reward_del_val here.
      // However, since we are using two colours, I have set the delay + reward_del_val
      // so that both colours show for the same amount of time

      pixels.fill(pixels.Color(0, 0, 255/4), ring, number_of_leds);
      pixels.show();
      delay(del_val);

      digitalWrite(obled, !digitalRead(obled));
      digitalWrite(redled, HIGH);
      delay(reward_del_val);
      digitalWrite(obled, !digitalRead(obled));

      pixels.fill(pixels.Color(0, 0, 0), ring, number_of_leds);
      pixels.show();
      digitalWrite(redled, LOW);

      delay(rest_delay);
      digitalWrite(redled, HIGH);
      delay(500);
      digitalWrite(redled, LOW);

    }
    else if (col_button_state == LOW){

      // indicator on for 15s
      digitalWrite(redled, HIGH);
      delay(acclimation_delay);
      // 10s off everything
      digitalWrite(redled, LOW);
      delay(stick_delay);

      pixels.fill(pixels.Color(0, 255/4, 0), ring, number_of_leds);
      
      pixels.show();

      delay(del_val);

      pixels.fill(pixels.Color(0, 255/4, 0, 0), ring, number_of_leds);
      pixels.show();
      delay(del_val);

      digitalWrite(obled, !digitalRead(obled));
      digitalWrite(redled, HIGH);
      delay(reward_del_val);
      digitalWrite(obled, !digitalRead(obled));

      pixels.fill(pixels.Color(0, 0, 0), ring, number_of_leds);
      pixels.show();
      digitalWrite(redled, LOW);

      delay(rest_delay);
      digitalWrite(redled, HIGH);
      delay(500);
      digitalWrite(redled, LOW);
    }
  }
  

  // keeps LEDs off in infinite loop
  while (running){
  pixels.fill(pixels.Color(0, 0, 0), ring, number_of_leds);
  pixels.show();

  // the pixels.fill() takes 'ring' as starting led, so if we reach the last
  // ring, we check if the value is equal to the staring pixel of the last
  // ring and then set the value to minus the number of leds in the first ring
  // this will start over again and the button press will set it to 0 and light
  // the first ring
  if (ring == number_of_leds*(number_of_rings - 1)){
    ring = - number_of_leds;
  }
  
  // take reading of button press (goes HIGH when pressed)
  reading = digitalRead(col_button);
  // if pressed and previous state is LOW and the time 
  // elapsed - time since previous press is more than 200 ms
  if (reading == HIGH && col_button_prev == LOW && millis() - time > debounce){
    // if the state is high, make it low
    if (col_button_state == HIGH) {
      col_button_state = LOW;
      //set time since push
      time = millis();
    }
    else if (col_button_state == LOW) {
      col_button_state = HIGH;
      time = millis();
    }
  }

  if (col_button_state == LOW){
    digitalWrite(obled, HIGH);
  }
  else {
    digitalWrite(obled, LOW);
  }

  ring_buttonState = digitalRead(ring_button);
  if (ring_buttonState == true) {
    ring += number_of_leds;
    
    running = false;
  }

  }
}