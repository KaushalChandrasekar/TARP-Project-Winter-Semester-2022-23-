#include <FreqMeasure.h> // https://github.com/PaulStoffregen/FreqMeasure
#include <VirtualWire.h>

const int red1 = 2, yellow1 = 3, green1 = 4;
const int red2 = 5, yellow2 = 6, green2 = 7;
const int red3 = A0, yellow3 = A1, green3 = A2;

void setup() {
    vw_set_rx_pin(11);
    vw_setup(2000);
    vw_rx_start();
    FreqMeasure.begin(); // Measures on pin 8 by default
    Serial.begin(9600);

    pinMode(red1, OUTPUT);
    pinMode(yellow1, OUTPUT);
    pinMode(green1, OUTPUT);
    pinMode(red2, OUTPUT);
    pinMode(yellow2, OUTPUT);
    pinMode(green2, OUTPUT);
    pinMode(red3, OUTPUT);
    pinMode(yellow3, OUTPUT);
    pinMode(green3, OUTPUT);

    // Set initial state of traffic lights
    digitalWrite(green1, HIGH);
    digitalWrite(red2, HIGH);
    digitalWrite(red3, HIGH);
}

double sum = 0;
int fin = 0;
int count = 0;
bool state = false;
float frequency;
int continuity1 = 0;
int continuity2 = 0;

unsigned long previousMillis = 0;
const long interval = 15000; // Updated interval to 15 seconds (10s green + 5s yellow)
int trafficState = 1;

void trafficLightSystem() {
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= interval) {
        previousMillis = currentMillis;
        if (trafficState == 1) {
            digitalWrite(green1, LOW);
            digitalWrite(yellow1, HIGH);
            delay(5000);
            digitalWrite(yellow1, LOW);
            digitalWrite(red1, HIGH);
            digitalWrite(red3, HIGH);
            digitalWrite(red2, LOW);
            digitalWrite(green2, HIGH);

            trafficState = 2;
        } else if (trafficState == 2) {
            digitalWrite(green2, LOW);
            digitalWrite(yellow2, HIGH);
            delay(5000);
            digitalWrite(yellow2, LOW);
            digitalWrite(red2, HIGH);
            digitalWrite(red1, HIGH);
            digitalWrite(red3, LOW);
            digitalWrite(green3, HIGH);

            trafficState = 3;
        } else if (trafficState == 3) {
            digitalWrite(green3, LOW);
            digitalWrite(yellow3, HIGH);
            delay(5000);
            digitalWrite(yellow3, LOW);
            digitalWrite(red3, HIGH);
            digitalWrite(red2, HIGH);
            digitalWrite(red1, LOW);
            digitalWrite(green1, HIGH);

            trafficState = 1;
        }
    }
}

void loop() {
    if (FreqMeasure.available()) {
        // average several reading together
        sum = sum + FreqMeasure.read();
        count = count + 1;

        if (count > 100) {
            frequency = FreqMeasure.countToFrequency(sum / count);
            Serial.println(frequency);
            sum = 0;
            count = 0;
        }
    }

    if (frequency > 2500 && frequency < 4000) {
        continuity1++;
        Serial.print("Continuity -> ");
        Serial.println(continuity1);
        frequency = 0;
    }

    if (frequency > 4000 && frequency < 4500) {
        continuity2++;
        Serial.print("Continuity -> ");
        Serial.println(continuity2);
        frequency = 0;
    }
    if (continuity1 >= 3 && continuity2 >= 3) {
      state = false;
      continuity1 = 0;
      continuity2 = 0;
      Serial.println("Ambulance Siren");
      delay(1000);
      fin = 1;
      if (green1, HIGH){
        digitalWrite(green1, LOW);
        digitalWrite(yellow1, HIGH);
        delay(1000);
        digitalWrite(yellow1, LOW);
        digitalWrite(red1, HIGH);
      }
      if (green2, HIGH){
        digitalWrite(green2, LOW);
        digitalWrite(yellow2, HIGH);
        delay(1000);
        digitalWrite(yellow2, LOW);
        digitalWrite(red2, HIGH);
      }
      digitalWrite(red3,LOW);
      digitalWrite(green3, HIGH);
      delay(10000);
      digitalWrite(green3, LOW);
      digitalWrite(yellow3, HIGH);
      delay(5000);
      digitalWrite(yellow3, LOW);
      digitalWrite(red3, HIGH);
    
      digitalWrite(red1, LOW);
      digitalWrite(green1, HIGH);
      delay(10000);
      fin = 1;
      trafficState = 1;
    }

  
  trafficLightSystem();
  

  uint8_t buf[VW_MAX_MESSAGE_LEN];
  uint8_t buflen = VW_MAX_MESSAGE_LEN;

  if (vw_get_message(buf, &buflen)) {
      Serial.print("Message: ");
      Serial.println((char*)buf);
  }
}