#include <Wire.h>
#include <Adafruit_NeoPixel.h> // Adafruit library for LED rings
#include "RTClib.h"  // rtc library for clock

RTC_DS1307 rtc;

Adafruit_NeoPixel strip1 = Adafruit_NeoPixel(12, 10, NEO_GRB + NEO_KHZ800); // Hours hand
Adafruit_NeoPixel strip2 = Adafruit_NeoPixel(60, 9, NEO_GRB + NEO_KHZ800); // Minutes and seconds hands

long interval = 50;
long previousMillis = 0;

// RGB
const int Hr1 = 255; // Sets the colours for each hand
const int Hr2 = 50;
const int Hr3 = 50;
const int Min1 = 0;
const int Min2 = 150;
const int Min3 = 50;
const int Sec1 = 200;
const int Sec2 = 0;
const int Sec3 = 250;
const int Light = 3; // Light sensor on analog pin 3
int val = 0; // Used to alter brightness of each ring based on ambient light value


void setup() 

{  
  Serial.begin(57600); // Serial support for debugging
  Wire.begin();
  rtc.begin();
  strip1.begin();
  strip2.begin();
  strip1.show(); // Initialize all pixels to 'off'
  strip2.show(); // Initialize all pixels to 'off'
//  rtc.adjust(DateTime(__DATE__,__TIME__)); // rtc collects system time on script load
}

void loop() 
{
  unsigned long currentMillis = millis(); // Condition to ensure LED sweep occurs only once
    
    val = analogRead(Light); // Sets the ring brightness from 1 to 25, based on ambient light
    val = map(val, 50, 1000, 1, 25);
    val = constrain(val, 1 , 25);
  
  
  DateTime now = rtc.now();
  Serial.print(now.year(), DEC);
  Serial.print("/");
  Serial.print(now.month(), DEC);
  Serial.print("/");
  Serial.print(now.day(), DEC);
  Serial.print("  ");
  Serial.print(now.hour(), DEC);
  Serial.print(":");
  Serial.print(now.minute(), DEC);
  Serial.print(":");
  Serial.print(now.second(), DEC);
  Serial.println();
  Serial.println(millis());
  //Serial.println(val);
  
  //delay(1000); // Used to slow down the serial monitor for debugging
  
 
 strip1.setBrightness(val); // Sets the brightness for the hours hand
 strip2.setBrightness(val); // Sets the brightness for the minutes and seconds hands
  
  
  
    strip1.setPixelColor(now.hour() %12 ,Hr1,Hr2,Hr3); // Sets the color for the hours hand
    
    
    if ((now.hour() %12) == 0) // If the hour is 12, this switches off LED representing 11
    {strip1.setPixelColor(11,0,0,0);}
    else {strip1.setPixelColor((now.hour() %12) - 1, 0,0,0);} // If the hour is anything other than 12, this switches off the previous hour
 
    if (now.minute() == 0) // If the minute is 60, this switches off LED representing 59
    {strip2.setPixelColor(59,0,0,0);}
    else {strip2.setPixelColor(now.minute() - 1, 0,0,0);} // If the minute is anything other than 60, this switches off the previous minute
    
    
    if (now.minute() ==0 && now.second() == 0)
    {
      for (int i=1;i<60; i++) { // Sweep up
         strip2.setPixelColor(i,100,255,155);
         strip2.show();
  delay(20);}
  //delay(1000);
      for (int i=59; i > 0; i--) { // Sweep down
         strip2.setPixelColor(i,0,0,0);
         strip2.show();
       delay(20);} 
     }
    
    previousMillis = currentMillis; // Condition(s) to sweep seconds lights to minutes after each 15 minutes
    
    if (now.second() == 0) // If the second is 60, this switches off LED representing 59 
    {
      strip2.setPixelColor(59,0,0,0);
     
       
        if(currentMillis - previousMillis < interval && now.minute() %15 == 0)
        {
          
       for (int i=0;i<now.minute(); i++) { // Sweep up
    strip2.setPixelColor(i,100,255,155);
    strip2.show();
  delay(20);}
   
       for (int i=now.minute(); i > 0; i--) { // Sweep down
         strip2.setPixelColor(i,0,0,0);
         strip2.show();
       delay(20);} 
       currentMillis = millis();
     }
    }

     
    else {strip2.setPixelColor(now.second() - 1, 0,0,0);} // If the second is anything other than 60, this switches off the previous second
    
    if (now.second()-now.minute()==1){ // This removes the issue where the seconds hand erased the minutes when passing
      strip2.setPixelColor(now.minute(), Min1,Min2,Min3);}
    
   
    for (int i=0; i<65; i=i+5) // This illuminates every 5 minute interval
    {
      if (now.second() == i) {
      strip2.setPixelColor(i,Sec1,Sec2,Sec3);} // Makes the seconds hand overwrite the 5 minute interval
    else {strip2.setPixelColor(i,50,50,50);}
    
    if (now.minute() == now.second()){ // Makes the minute hand overwrite the second hand
      strip2.setPixelColor(now.minute(),Min1,Min2,Min3);}
      else{strip2.setPixelColor(now.second(),Sec1,Sec2,Sec3);} // Creates the second hand
    
    if (now.minute() == i) { // Makes the minute hand overwrite the 5 minute interval
      strip2.setPixelColor(i,Min1,Min2,Min3);}
      else {strip2.setPixelColor(i,155,155,155);
    strip2.setPixelColor(now.minute(), Min1,Min2,Min3);} // Creates the minute hand
    
    }
    
    strip1.show(); 
    strip2.show();
 
}
