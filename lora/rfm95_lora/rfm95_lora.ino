#include <SPI.h>
#include <LoRa.h>

int counter = 0;
// - Pin configs -
#define ss 5
#define rst 14
#define dio0 2

void setup() {
  Serial.begin(115200);
  while (!Serial);
  delay(1000);

  Serial.println("LoRa Sender");
  // Setup LoRa transceiver module
  LoRa.setPins(ss, rst, dio0);
  
  if (!LoRa.begin(868E6)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }else{
    Serial.println("Starting LoRa successful");
    }
  
}

// Sends a string every 5000ms (5 seconds)
void loop() {
  Serial.print("Sending packet: ");
  Serial.println(counter);

  // send packet
  LoRa.beginPacket();
  LoRa.print("hello ");
  LoRa.print(counter);
  LoRa.endPacket();

  counter++;

  delay(5000);
}