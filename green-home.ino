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

#define SensorPin A2            //pH meter Analog output to Arduino Analog Input 0
#define Offset 40.533      //deviation compensate
#define LED 13
#define samplingInterval 20
#define printInterval 800
#define ArrayLenth  40    //times of collection
#define uart  Serial
int pHArray[ArrayLenth];   //Store the average value of the sensor feedback
int pHArrayIndex = 0;

//define the displays
Seeed_Digital_Tube humTube;
Seeed_Digital_Tube tempTube;
Seeed_Digital_Tube PHTube;

void DisplayInfo(float hum, float temp, float PH);
void readDHT(float& hum, float& temp);
void readPH(float& PH);


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
float hum = 0;
float temp = 0;
float PH = 0;

void loop() {
    readDHT(hum, temp);
    readPH(PH);
    DisplayInfo(hum, temp, PH);
 
    delay(1500);
}

void DisplayInfo(float hum, float temp, float PH){
  humTube.displayNum(hum);
  tempTube.displayNum(temp);
  PHTube.displayNum(PH);
}

void readDHT(float& hum, float& temp){
  //read temp and humidity
  float temp_hum_val[2] = {0};
  if (!dht.readTempAndHumidity(temp_hum_val)) {
  }else{
    debug.println("Failed to get temprature and humidity value.");
  }
  hum = temp_hum_val[0];
  //convert from celcius to farenheit
  temp = (temp_hum_val[1] * 9/5) + 32;
}

void readPH(float& PH){
  //read PH
  static unsigned long samplingTime = millis();
  static unsigned long printTime = millis();
  static float pHValue, voltage;
  if (millis() - samplingTime > samplingInterval)
  {
    pHArray[pHArrayIndex++] = analogRead(SensorPin);
    if (pHArrayIndex == ArrayLenth)pHArrayIndex = 0;
    voltage = avergearray(pHArray, ArrayLenth) * 5.0 / 1024;
    pHValue = -19.18518519 * voltage + Offset;
    samplingTime = millis();
  }
  PH = pHValue;
    if (millis() - printTime > printInterval)  //Every 800 milliseconds, print a numerical, convert the state of the LED indicator
  {
    uart.print("Voltage:");
    uart.print(voltage, 2);
    uart.print("    pH value: ");
    uart.println(pHValue, 2);
    digitalWrite(LED, digitalRead(LED) ^ 1);
    printTime = millis();
  }
}

double avergearray(int* arr, int number) {
  int i;
  int max, min;
  double avg;
  long amount = 0;
  if (number <= 0) {
    uart.println("Error number for the array to avraging!/n");
    return 0;
  }
  if (number < 5) { //less than 5, calculated directly statistics
    for (i = 0; i < number; i++) {
      amount += arr[i];
    }
    avg = amount / number;
    return avg;
  } else {
    if (arr[0] < arr[1]) {
      min = arr[0]; max = arr[1];
    }
    else {
      min = arr[1]; max = arr[0];
    }
    for (i = 2; i < number; i++) {
      if (arr[i] < min) {
        amount += min;      //arr<min
        min = arr[i];
      } else {
        if (arr[i] > max) {
          amount += max;  //arr>max
          max = arr[i];
        } else {
          amount += arr[i]; //min<=arr<=max
        }
      }//if
    }//for
    avg = (double)amount / (number - 2);
  }//if
  return avg;
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
