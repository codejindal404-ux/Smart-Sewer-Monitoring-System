#define BLYNK_TEMPLATE_ID "TMPL3Q_IB40mj"
#define BLYNK_TEMPLATE_NAME "Sewer monitor"
#define BLYNK_PRINT Serial
#define LED_PIN D7
// #define WATER_LED D7
#define GAS_LED D4

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

char auth[] = "w1XH33H3vrixBtb_2Rne2Mq-czqK1lGW";

// 👉 Put your NEW WORKING WiFi here
char ssid[] = "OnePlus Nord 5 z3h4";
char pass[] = "123456789";

#define TRIG D5
#define ECHO D6

void setup() {
  Serial.begin(9600);

  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);
  pinMode(LED_PIN, OUTPUT);
  // pinMode(WATER_LED, OUTPUT);
pinMode(GAS_LED, OUTPUT);

  Serial.println("Starting...");

  // Connect WiFi
  WiFi.begin(ssid, pass);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi Connected!");

  // Connect Blynk
  Blynk.config(auth);
  Blynk.connect();

  Serial.println("Blynk Connected!");
}

void loop() {
  Blynk.run();

  // Ultrasonic
  digitalWrite(TRIG, LOW);
  delayMicroseconds(2);

  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);

  long duration = pulseIn(ECHO, HIGH);
  float distance = duration * 0.034 / 2;

  // Gas
  int gasValue = analogRead(A0);

  // LED logic
static bool alertSent = false;

if (distance < 8) {
  digitalWrite(LED_PIN, HIGH);

  if (!alertSent) {
    Blynk.logEvent("alert", "⚠️ Water level is high!");
    alertSent = true;
  }

} else {
  digitalWrite(LED_PIN, LOW);
  alertSent = false;
}
// gas level alert
static bool gasAlertSent = false;

if (gasValue > 250) {
  digitalWrite(GAS_LED, HIGH);

  Serial.println("GAS ALERT TRIGGERED");

  if (!gasAlertSent && Blynk.connected()) {
    Blynk.logEvent("gas_alert", "🚨 Gas level is HIGH!");
    gasAlertSent = true;
  }

} else {
  digitalWrite(GAS_LED, LOW);
  gasAlertSent = false;
}

  // Send to Blynk
  Blynk.virtualWrite(V0, distance);
  Blynk.virtualWrite(V1, gasValue);

  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.print(" | Gas: ");
  Serial.println(gasValue);

  delay(1000);
}