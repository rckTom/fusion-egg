#include <FastLED.h>


//lichterkette: PWM 2
//mikrofon: A1
#define LED_PIN 2
#define NUM_LEDS 144


#define SAMPLING_FREQUENCY_BP 40       // number of energy chunks per second
#define SAMPLING_FREQUENCY_CONTROL 1   // check number of times per second if the current band pass is the best one
#define Q 20.   // quality factor of band pass filters
#define PI 3.1415926535897932384626433832795
#define n_BP 30  //number of band pass filters


CRGB leds[NUM_LEDS];

unsigned long sampling_period_bp = 1000000L/SAMPLING_FREQUENCY_BP;
unsigned long sampling_period_control = 1000000L/SAMPLING_FREQUENCY_CONTROL;
double energy = 0;
unsigned long last_us_bp = 0L;
unsigned long last_us_control = 0L;

float a0[n_BP];
float a1[n_BP];
float a2[n_BP];
float b0[n_BP];
//float b1[n_BP]; 
float b2[n_BP];

float a[n_BP];
float w0[n_BP];

float yy1[n_BP];
float yy2[n_BP];
float yy3[n_BP];
float yy4[n_BP];
float yy5[n_BP];
float yy6[n_BP];

float u1[n_BP];
float u2[n_BP];
float y[n_BP];
float y_fil[n_BP];

float angle;
float angle2;

double energy_fil = 800.;


float pos_target = NUM_LEDS/2;
float pos_target_filtered = NUM_LEDS/2;

float microphone_offset = 512;
long initial_time;

int active = 15;
int candidate = 15;
int rounds = 0;




void setup() {

  Serial.begin(115200);
  
  FastLED.addLeds<WS2812, LED_PIN, RGB>(leds, NUM_LEDS);
  FastLED.setMaxPowerInVoltsAndMilliamps(5, 300);

//  for(int i = 0; i < NUM_LEDS; i++)
//  { int brightness = get_value(i, pos_target_filtered);
//    leds[i].setRGB(brightness, brightness, brightness); }
//  FastLED.show();
//
//  long sumsamples = 0;
//  for(int j = 1; j<1000; j++)
//  {
//    int sample = analogRead(1);
//    sumsamples += sample;
//    delay(1);
//    if(j==500)
//    {
//      sumsamples = 0;
//    }
//  }
//  microphone_offset = sumsamples/500;      


  set_filter();
  initial_time = micros();


}

void set_filter() {
  for(int i = 0; i < n_BP; i++)
  {
    float frequency = 1.75+i*(2.4-1.75)/n_BP;
    w0[i] = 2.*PI*frequency/SAMPLING_FREQUENCY_BP;
    a[i] = sin(w0[i]/(2.*Q)); 
    b0[i] = a[i];
    //b1[i] = 0;
    b2[i] = -a[i];
    a0[i] = 1.+a[i];
    a1[i] = -2.*cos(w0[i]);
    a2[i] = 1.-a[i];
  }
}

int get_value(int pos, float pos0) {
  if (abs(pos0-pos) > 20) { return 0; }
  else { return (40-abs(pos0-pos)*2); }
}

void loop() {
  
   int sample = int(analogRead(1) - microphone_offset);
   energy += abs(sample)*abs(sample);
   
   if (micros() - last_us_bp > sampling_period_bp)
   {

    Serial.println(sample);

   microphone_offset += (analogRead(1)-microphone_offset)*0.001;

   //Serial.println(microphone_offset);
    
       last_us_bp += sampling_period_bp;
       energy_fil += (energy - energy_fil) * 0.01;
      //Serial.println(energy);
        for(int i = 0; i < n_BP; i++)
        {
           y[i] = (b0[i]/a0[i])*energy + 0. + (b2[i]/a0[i])*u2[i] - (a1[i]/a0[i])*yy1[i] - (a2[i]/a0[i])*yy2[i];
           u2[i] = u1[i];
           u1[i] = energy;
           yy6[i] = yy5[i];
           yy5[i] = yy4[i];
           yy4[i] = yy3[i];
           yy3[i] = yy2[i];
           yy2[i] = yy1[i];
           yy1[i] = y[i];
           y_fil[i] += (abs(y[i]) - y_fil[i]) * 0.005; //linie der scheitelpunkte
        }

       float delays = constrain( SAMPLING_FREQUENCY_BP * 0.25/(1.75+active*(2.4-1.75)/n_BP) , 4., 6.);

       float delayed = 0;
       if (delays > 5)
       {delayed = yy5[active]*(1-delays+5) + yy6[active]*(delays-5); }
       else
       {delayed = yy4[active]*(1-delays+4) + yy5[active]*(delays-4); }

       angle = atan2(delayed , y[active]);

       if (PI < abs(angle - angle2) && abs(angle - angle2)  < 3*PI)
       { angle2 = angle + 2*PI; }
       else
       { angle2 = angle; }

       pos_target = map(angle2, -PI, 3*PI, -0.3*NUM_LEDS, NUM_LEDS*1.5);
        
       if (pos_target > pos_target_filtered)
       { pos_target_filtered += (pos_target - pos_target_filtered)*0.35; }
       else
       { pos_target_filtered = pos_target; }

//       Serial.print(y_fil[active]);
//       Serial.print(",");
//       Serial.println(y[active]);

       energy = 0;


      
      
       for(int i = 0; i < NUM_LEDS; i++)
       { 
         int brightness = get_value(i, pos_target_filtered);
         
         //leds[i].setRGB(brightness, brightness, brightness); 
         leds[i].setHSV( 160, (rounds == 6) ? 60 : 0, brightness);
       }
       FastLED.show();
   }

   if (micros() - last_us_control > sampling_period_control)
   {          
      last_us_control += sampling_period_control;
      int argmax = -1;
      float valuemax = 0;
      for(int i = 0; i < n_BP; i++)
      {
        if(y_fil[i] > valuemax)
        {
          valuemax = y_fil[i];
          argmax = i;
        }
      }
      
      if(argmax > -1)
      {
          if(argmax == candidate)
          {
            rounds ++;              
          }
          else
          {
            rounds = 0;
            candidate = argmax;
          }
          if(rounds > 6)
          {
            rounds = 0;
            active = candidate;
          }
      }           
   }
}
