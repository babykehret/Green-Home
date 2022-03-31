//Green Home Control Code
//SPU Senior Design Group CJ & The Planet Lovers, 2022
//Written by: Parker Charles Kehret

#include "Wire.h"
#include "DHT.h"
#include "grove_alphanumeric_display.h"

#define DHTTYPE DHT20   // DHT 20
/*Notice: The DHT10 and DHT20 is different from other DHT* sensor ,it uses i2c interface rather than one wire*/
/*So it doesn't require a pin.*/
DHT dht(DHTTYPE);         //   DHT10 DHT20 don't need to define Pin
 
#if defined(ARDUINO_ARCH_AVR)
    #define debug  Serial
 
#elif defined(ARDUINO_ARCH_SAMD) ||  defined(ARDUINO_ARCH_SAM)
    #define debug  SerialUSB
#else
    #define debug  Serial
#endif

//define the displays
Seeed_Digital_Tube humTube;
Seeed_Digital_Tube tempTube;
Seeed_Digital_Tube PHTube;

void DisplayInfo(float hum, float temp, float PH);
void readSensors(float& hum, float& temp, float& PH);


void setup() {
 
    debug.begin(115200);
    debug.println("DHTxx test!");
    Wire.begin();

    //set up displays
    humTube.setTubeType(TYPE_2,0x70);
    humTube.setBrightness(15);
    humTube.setBlinkRate(BLINK_OFF);

    tempTube.setTubeType(TYPE_2,0x72);
    tempTube.setBrightness(15);
    tempTube.setBlinkRate(BLINK_OFF);

    PHTube.setTubeType(TYPE_2,0x74);
    PHTube.setBrightness(15);
    PHTube.setBlinkRate(BLINK_OFF);
    
    dht.begin();
}
 
void loop() {
    float hum = 0;
    float temp = 0;
    float PH = 0;

    readSensors(hum, temp, PH);
    DisplayInfo(hum, temp, PH);
 
    delay(1500);
}

void DisplayInfo(float hum, float temp, float PH){
  humTube.displayNum(hum);
  tempTube.displayNum(temp);
  PHTube.displayNum(PH);
}

void readSensors(float& hum, float& temp, float& PH){
  float temp_hum_val[2] = {0};
  if (!dht.readTempAndHumidity(temp_hum_val)) {
  }else{
    debug.println("Failed to get temprature and humidity value.");
  }
  hum = temp_hum_val[0];
  //convert from celcius to farenheit
  temp = (temp_hum_val[1] * 9/5) + 32;
  PH = 0;
}
void checkVent(float hum, float temp){
  if(temp >= 75){
    //turn on vent
  }else if(hum >= 90){
    //turn on vent
  }else{
    //turn off vent
  }
}
