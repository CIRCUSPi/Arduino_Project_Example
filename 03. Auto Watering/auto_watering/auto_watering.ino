/*
 * Moisture Sensor
 * S ------ A0
 * VCC ---- 5V
 * GND ---- GND
 * 
 * Relay
 * IN ----- 15(A1)
 * GND ---- GND
 * VCC ---- 5V
 */

#define moisture A0
#define  relay   15

void setup() {
  Serial.begin(9600);
  pinMode(moisture, INPUT);
  pinMode(relay, OUTPUT);

}

void loop() {
  int waterLevel = analogRead(moisture);
  Serial.println(waterLevel);
  if(waterLevel <= 500) {
    digitalWrite(relay, LOW);
  }
  else {
    digitalWrite(relay, HIGH);
  }
  delay(100);
}
