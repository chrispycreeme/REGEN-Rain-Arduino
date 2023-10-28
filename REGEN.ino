#include <SoftwareSerial.h>
SoftwareSerial HM10(10, 11);

int sensor;
#define AOUT_PIN A0
#define STORMSURGE 250
#define HEAVYRAIN 450
#define MEDIUMRAIN 650
#define LIGHTRAIN 850

int lightRainTimer = 0;
int mediumRainTimer = 0;
int heavyRainTimer = 0;
int stormSurgeTimer = 0;

int stormSurgeMessageTimer = 0;
int heavyRainMessageTimer = 0;
int mediumRainMessageTimer = 0;
int lightRainMessageTimer = 0;

const int TIMER_THRESHOLD = 5;
bool floodDetected = false;

void setup() {
  Serial.begin(9600);
  HM10.begin(9600);
}

void loop() {
  sensor = analogRead(A0);

  int value = analogRead(AOUT_PIN);

  if (value <= STORMSURGE) {
    stormSurgeTimer++;
    lightRainTimer = mediumRainTimer = heavyRainTimer = 0;
  } else if (value <= HEAVYRAIN) {
    heavyRainTimer++;
    lightRainTimer = mediumRainTimer = stormSurgeTimer = 0;
  } else if (value <= MEDIUMRAIN) {
    mediumRainTimer++;
    lightRainTimer = heavyRainTimer = stormSurgeTimer = 0;
  } else if (value <= LIGHTRAIN) {
    lightRainTimer++;
    mediumRainTimer = heavyRainTimer = stormSurgeTimer = 0;
  } else {
    lightRainTimer = mediumRainTimer = heavyRainTimer = stormSurgeTimer = 0;
    floodDetected = false;
  }

  if (lightRainTimer >= TIMER_THRESHOLD) {
    lightRainMessageTimer++;
  } else if (mediumRainTimer >= TIMER_THRESHOLD) {
    mediumRainMessageTimer++;
  } else if (heavyRainTimer >= TIMER_THRESHOLD) {
    heavyRainMessageTimer++;
  } else if (stormSurgeTimer >= TIMER_THRESHOLD) {
    stormSurgeMessageTimer++;
  }

  if (stormSurgeMessageTimer >= TIMER_THRESHOLD) {
    floodDetected = true;
    Serial.println("Storm Surge Detected for 5 Minutes!");
    stormSurgeMessageTimer = 0;
  } else if (heavyRainMessageTimer >= TIMER_THRESHOLD) {
    floodDetected = true;
    Serial.println("Heavy Rain Detected for 5 Minutes!");
    heavyRainMessageTimer = 0;
  } else if (mediumRainMessageTimer >= TIMER_THRESHOLD) {
    floodDetected = true;
    Serial.println("Medium Rain Detected for 5 Minutes!");
    mediumRainMessageTimer = 0;
  } else if (lightRainMessageTimer >= TIMER_THRESHOLD) {
    floodDetected = true;
    Serial.println("Light Rain Detected for 5 Minutes!");
    lightRainMessageTimer = 0;
  }

  if (floodDetected && (value < LIGHTRAIN || value >= STORMSURGE)) {
    lightRainTimer = mediumRainTimer = heavyRainTimer = stormSurgeTimer = 0;
    floodDetected = false;
  }

  if (value <= STORMSURGE) {
    HM10.print("Storm (");
  } else if (value <= HEAVYRAIN) {
    HM10.print("Heavy (");
  } else if (value <= MEDIUMRAIN) {
    HM10.print("Med (");
  } else if (value <= LIGHTRAIN) {
    HM10.print("Light (");
  } else {
    HM10.print("No Rain (");
  }

  HM10.print(value);
  HM10.println(")");

  delay(1000);
}
