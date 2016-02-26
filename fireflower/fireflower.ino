//Alex Maki-Jokela
//version 0.1
//needs cleanup, speed optimization, actuator function, and move to beaglebone

#include "FastLED.h"


//DISTIRBUTION: 1-30 right of pistil 31-60 left of pistil 60-105 left of perimeter 105-150 right of perimeter

FASTLED_USING_NAMESPACE

#if FASTLED_VERSION < 3001000
#error "Requires FastLED 3.1 or later; check github for latest code."
#endif

#define PIR_PIN   22
#define DATA_PIN0    36
#define DATA_PIN1  38  
#define DATA_PIN2  40  
#define DATA_PIN3  42  
#define DATA_PIN4  44  
#define CLK_PIN0   37
#define CLK_PIN1   39
#define CLK_PIN2   41
#define CLK_PIN3   43
#define CLK_PIN4   45
#define LED_TYPE    APA102
#define COLOR_ORDER GRB
#define NUM_LEDS   150
#define NUM_PETALS 5
#define TIME_BETWEEN_PIR_READ 300
CRGB leds[NUM_PETALS][NUM_LEDS];
int ledhues[NUM_LEDS];
int ledvals[NUM_LEDS];
int MAX_PIXEL_HUE_DIFF=20;
int MAX_PIXEL_VAL_DIFF=20;
int randomaddhue=0;
int randomaddsat=0;
int glowpos=0;
int glowiness=50;
unsigned long previousMillis=0;

#define BRIGHTNESS          96
#define FRAMES_PER_SECOND  60

void setup() {
  delay(3000); // 3 second delay for recovery
  
  // tell FastLED about the LED strip configuration
  //FastLED.addLeds<LED_TYPE,DATA_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  //do this manually because I don't know how #define works in c with arrays and it's 2am 
  FastLED.addLeds<LED_TYPE,DATA_PIN0,CLK_PIN0,COLOR_ORDER>(leds[0], NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.addLeds<LED_TYPE,DATA_PIN1,CLK_PIN1,COLOR_ORDER>(leds[1], NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.addLeds<LED_TYPE,DATA_PIN2,CLK_PIN2,COLOR_ORDER>(leds[2], NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.addLeds<LED_TYPE,DATA_PIN3,CLK_PIN3,COLOR_ORDER>(leds[3], NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.addLeds<LED_TYPE,DATA_PIN4,CLK_PIN4,COLOR_ORDER>(leds[4], NUM_LEDS).setCorrection(TypicalLEDStrip);

  // set master brightness control
  FastLED.setBrightness(BRIGHTNESS);
/*
  //for the actuator (doesnt work right now @#$%@#)
pinMode(8, OUTPUT); //set direction pin as output
pinMode(9, OUTPUT); //set enable pin as output */

   pinMode(PIR_PIN, INPUT);
}

  
void loop()
{
  unsigned long currentMillis = millis();

  
  fireflower(currentMillis);
  FastLED.show();  
  FastLED.delay(1000/FRAMES_PER_SECOND); 

  // do some periodic updates
//  EVERY_N_MILLISECONDS( 20 ) { gHue++; } // slowly cycle the "base color" through the rainbow
//  EVERY_N_SECONDS( 10 ) { nextPattern(); } // change patterns periodically
}



void fireflower(unsigned long currentMillis) 
{
  int pos[NUM_PETALS];
  if ((unsigned long)(currentMillis - previousMillis) >= TIME_BETWEEN_PIR_READ) {
    int pir_state=digitalRead(PIR_PIN);
    
    if (pir_state==HIGH && glowiness <= 240){
      glowiness+=15;
    }
    else if (pir_state==LOW && glowiness > 100){
      glowiness-=2;
    }
    previousMillis = millis();
  }
  
  for (int petl=0; petl < NUM_PETALS; petl++){
   pos[petl] = beatsin16(30,0,40,0,360*petl/NUM_PETALS);
  }
  
  for( int i = 0; i < NUM_LEDS; i++) {
    if (i < 60){
      for (int petl=0; petl < NUM_PETALS; petl++){
       leds[petl][i] = CHSV(160+pos[petl], 200, glowiness);
      }
    }
    else {
      for (int petl=0; petl < NUM_PETALS; petl++){
       leds[petl][i] = CHSV(200-pos[petl], 200, glowiness);
      }
    }
  }
  if (glowiness > 200){
    addGlitter(150);
  }
 if (glowiness > 200){
  if (glowpos < 45){
   glowpos +=1;
  }
  else {
    glowpos=0;
  }
  int glowpos1 = (glowpos+9 )% 45;
  int glowpos2 = (glowpos+18 )% 45;
  int glowpos3 = (glowpos+27 )% 45;
  int glowpos4 = (glowpos+36 )% 45;
  for (int petl=0; petl < NUM_PETALS; petl++){
       leds[petl][60+glowpos] += CRGB::Purple;
       leds[petl][150-glowpos] += CRGB::Purple;
       leds[petl][60+glowpos1] += CRGB::Purple;
       leds[petl][150-glowpos1] += CRGB::Purple;
       leds[petl][60+glowpos2] += CRGB::Purple;
       leds[petl][150-glowpos2] += CRGB::Purple;
       leds[petl][60+glowpos3] += CRGB::Purple;
       leds[petl][150-glowpos3] += CRGB::Purple;
       leds[petl][60+glowpos4] += CRGB::Purple;
       leds[petl][150-glowpos4] += CRGB::Purple;
      }
  }
}


void addGlitter( fract8 chanceOfGlitter) 
{
  if( random8() < chanceOfGlitter) {
    for (int petl=0; petl < NUM_PETALS; petl++){
      leds[petl][ 20+random16(20) ] += CRGB::Blue;
    }
  }
}

