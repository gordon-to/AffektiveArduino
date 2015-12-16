#include <Arduino.h>
#include <SPI.h>

#include <Adafruit_BLE.h>
#include <Adafruit_BluefruitLE_SPI.h>
#include <Adafruit_BluefruitLE_UART.h>
#include "BluefruitConfig.h"

#include <DueTimer.h>
#include <Wire.h>

int32_t hrmMeasureCharId;           // id for ble characteristic
int32_t gsrMeasureCharId;           // id for ble characteristic
volatile int BPM;                   // used to hold the pulse rate
volatile boolean QS = false;        // becomes true when Arduoino finds a beat.
volatile float gsr_value = 0;

Adafruit_BluefruitLE_SPI ble(BLUEFRUIT_SPI_SCK, BLUEFRUIT_SPI_MISO,
                             BLUEFRUIT_SPI_MOSI, BLUEFRUIT_SPI_CS,
                             BLUEFRUIT_SPI_IRQ, BLUEFRUIT_SPI_RST);



void hr_handler(){
  read_hr();
}

void gsr_handler(){
  read_gsr();
}

void ble_publish(){
  if (QS){
    set_gattchar(ble, hrmMeasureCharId, BPM);
  } else {
    set_gattchar(ble, hrmMeasureCharId, 0);
  }
  set_gattchar(ble, gsrMeasureCharId, gsr_value);
}


void setup() {
  pinMode(13, OUTPUT);
  Serial.begin(115200);
  Serial.println("Affektive BLE Stress Sensor");
  ble_setup();
  delay(50);
  Serial.println("Start hr timer");
  Timer.getAvailable().attachInterrupt(hr_handler).start(2000); // every 2 ms;
  Serial.println("Start hr timer");
  Timer.getAvailable().attachInterrupt(gsr_handler).start(5000); // every 5 ms;
  Serial.println("End of setup");

}

void loop() {
  delay(1000);
  Serial.print("QS: ");
  Serial.print(QS);
  Serial.print(" BPM: ");
  Serial.println(BPM);
  
  Serial.print("GSR Value: ");
  Serial.println(gsr_value);
  if (QS && gsr_value > 200 && BPM < 200 && BPM > 45){
    ble_publish();
    Serial.println("sent");
  }
  else{
    Serial.println("check sensor position");
  }
  // put your main code here, to run repeatedly:

}
