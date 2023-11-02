#include <SoftwareSerial.h>
SoftwareSerial HM10(10, 11);

#define AOUT_PIN A0
#define STORMSURGE 200
#define HEAVYRAIN 450
#define LIGHTRAIN 800

int timer = 10;  // Dito nakalagay kung ilang seconds yung timer, seconds seconds

void setup() {
  Serial.begin(9600);
  HM10.begin(9600);
}

void loop() {
  int stormCount = 0, heavyCount = 0, lightCount = 0, noRainCount = 0;

  while (timer > 0) {
    int value = analogRead(AOUT_PIN);

    if (value <= STORMSURGE) {
      Serial.print("Storm (");
      stormCount++;
    } else if (value <= HEAVYRAIN) {
      Serial.print("Heavy (");
      heavyCount++;
    } else if (value <= LIGHTRAIN) {
      Serial.print("Light (");
      lightCount++;
    } else {
      Serial.print("No Rain (");
      noRainCount++;
    }

    Serial.print(value);
    Serial.println(")");
    HM10.print(value);
    HM10.println(",");

    delay(1000);
    timer--;
  }

  int maxCount = max(stormCount, max(heavyCount, max(lightCount, noRainCount)));
  int value = analogRead(AOUT_PIN);

  if (maxCount == stormCount) {
    Serial.println("The most frequent weather condition observed was: Storm");
    HM10.print(value);
    HM10.print(",");
    HM10.println("4");
  } else if (maxCount == heavyCount) {
    Serial.println("The most frequent weather condition observed was: Heavy Rain");
    HM10.print(value);
    HM10.print(",");
    HM10.println("3");
  } else if (maxCount == lightCount) {
    Serial.println("The most frequent weather condition observed was: Light Rain");
    HM10.print(value);
    HM10.print(",");
    HM10.println("2");
  } else {
    Serial.println("The most frequent weather condition observed was: No Rain");
    HM10.print(value);
    HM10.print(",");
    HM10.println("1");
  }

  delay(1000);
  timer = 10;
}
