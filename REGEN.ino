#include <SoftwareSerial.h>
SoftwareSerial HM10(10, 11);

#define AOUT_PIN A0
#define STORMSURGE 200
#define HEAVYRAIN 450
#define LIGHTRAIN 800
#define NO_RAIN_THRESHOLD 10
#define SENSOR_POWER 3
#define DO_PIN 4

int timer = 10;
int noRainDuration = 0;
bool isSensorActive = true;
int noRainMessageCount = 0;
bool sensorInactivePrinted = false;
int SensorActivate;

void setup() {
  Serial.begin(9600);
  HM10.begin(9600);
  pinMode(SENSOR_POWER, OUTPUT);
  pinMode(DO_PIN, INPUT);
  digitalWrite(SENSOR_POWER, HIGH);
}

void loop() {


  int stormCount = 0, heavyCount = 0, lightCount = 0, noRainCount = 0;

  while (timer > 0) {
    int value = analogRead(AOUT_PIN);

    if (isSensorActive) {
      if (value <= STORMSURGE) {
        Serial.print("Storm (");
        stormCount++;
        noRainDuration = 0;
        HM10.print("1,");
      } else if (value <= HEAVYRAIN) {
        Serial.print("Heavy (");
        heavyCount++;
        noRainDuration = 0;
        HM10.print("1,");
      } else if (value <= LIGHTRAIN) {
        Serial.print("Light (");
        lightCount++;
        noRainDuration = 0;
        HM10.print("1,");
      } else {
        Serial.print("No Rain (");
        noRainCount++;
        noRainDuration++;
        HM10.print("1,");
      }
    } else {
      if (!sensorInactivePrinted) {
        Serial.println("Sensor is inactive.");
        HM10.println("0, 0, 0");
        sensorInactivePrinted = true;
      }
    }

    if (isSensorActive) {
      Serial.print(value);
      Serial.println(")");
      HM10.print(value);
      HM10.println(",");
    }

    delay(2300);

    timer--;
  }

  int maxCount = max(stormCount, max(heavyCount, max(lightCount, noRainCount)));
  int value = analogRead(AOUT_PIN);

  if (HM10.available() > 0) {
    int valueFromHM10 = HM10.parseInt();

    if (valueFromHM10 > 0) {
      Serial.println("Received a value greater than 1. Turning on the sensor.");
      isSensorActive = true;
      digitalWrite(SENSOR_POWER, HIGH);
      sensorInactivePrinted = false;
    }
  }

  int rain_state = digitalRead(DO_PIN);

  if (noRainMessageCount >= 2) {
    if (isSensorActive) {
      Serial.println("Threshold reached. Turning off the sensor.");
      isSensorActive = false;
      digitalWrite(SENSOR_POWER, LOW);
      HM10.println("0, 0, 0");
      value = 0;
      noRainMessageCount = 0;  // Reset the count for next activation
      sensorInactivePrinted = true;


    }
  } else {
    if (value > 0) {
      Serial.println("Detected rain. Turning on the sensor.");
      isSensorActive = true;
      digitalWrite(SENSOR_POWER, HIGH);
      noRainDuration = 0;
      sensorInactivePrinted = false;
    }

    if (isSensorActive) {
      sensorInactivePrinted = false;
      if (maxCount == stormCount) {
        Serial.println("The most frequent weather condition observed was: Storm");
        HM10.print("1,");
        HM10.print(value);
        HM10.print(",");
        HM10.println("4");
      } else if (maxCount == heavyCount) {
        Serial.println("The most frequent weather condition observed was: Heavy Rain");
        HM10.print("1,");
        HM10.print(value);
        HM10.print(",");
        HM10.println("3");
      } else if (maxCount == lightCount) {
        Serial.println("The most frequent weather condition observed was: Light Rain");
        HM10.print("1,");
        HM10.print(value);
        HM10.print(",");
        HM10.println("2");
      } else {
        Serial.println("The most frequent weather condition observed was: No Rain");
        HM10.print("1,");
        HM10.print(value);
        HM10.print(",");
        HM10.println("1");
        noRainMessageCount++;
      }
    }
  }

  if (HM10.available()) {
    SensorActivate = HM10.read();
    if (SensorActivate == 1) {
      isSensorActive = true;
      digitalWrite(SENSOR_POWER, HIGH);
      noRainDuration = 0;
      sensorInactivePrinted = false;
    }
  }

  delay(1000);
  timer = 10;
}
