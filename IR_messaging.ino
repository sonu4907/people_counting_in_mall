#include <WiFi.h>
#include <HTTPClient.h>
#include <WiFiClientSecure.h>

// ===== Wi-Fi Credentials =====
const char* ssid     = "Realme_8";
const char* password = "shreeram";

// ===== Telegram Bot Info =====
const String botToken = "8530342095:AAEP0W9EbE5p1b_b_zk_5XCjgtKpKQ5mBwA";
const String chatID   = "5076971597";

// ===== IR Sensor Pins =====
const int irEntry = 15;  // Entry sensor
const int irExit  = 4;   // Exit sensor

int lastEntryState = HIGH;
int lastExitState  = HIGH;

int peopleCount = 0;

unsigned long lastNotify = 0;
const unsigned long notifyInterval = 2; // minimum 2 seconds between messages

// ===== URL Encoder =====
String urlEncode(const String &str) {
  String encoded = "";
  char c;
  for (size_t i = 0; i < str.length(); i++) {
    c = str[i];
    if (isalnum(c) || c == '-' || c == '_' || c == '.' || c == '~') encoded += c;
    else if (c == ' ') encoded += '+';
    else {
      char buf[5];
      sprintf(buf, "%%%02X", (unsigned char)c);
      encoded += buf;
    }
  }
  return encoded;
}

// ===== Send Telegram Message =====
void sendTelegram(const String &message) {
  if (WiFi.status() != WL_CONNECTED) return;
  WiFiClientSecure client;
  client.setInsecure();
  HTTPClient https;
  String text = urlEncode(message);
  String request = "https://api.telegram.org/bot" + botToken +
                   "/sendMessage?chat_id=" + chatID + "&text=" + text;
  https.begin(client, request);
  int httpCode = https.GET();
  https.end();
  Serial.printf("Telegram sent (code %d)\n", httpCode);
}

// ===== Setup =====
void setup() {
  Serial.begin(115200);
  pinMode(irEntry, INPUT);
  pinMode(irExit, INPUT);

  WiFi.setAutoReconnect(true);
  WiFi.persistent(true);

  Serial.print("Connecting to WiFi");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("\n✅ WiFi Connected!");
  Serial.println(WiFi.localIP());
  
  sendTelegram("🔔 System Started: People Counter Ready!");
}

// ===== Main Loop =====
void loop() {
  int entryState = digitalRead(irEntry);
  int exitState  = digitalRead(irExit);

  unsigned long now = millis();

  // Entry Detection
  if (entryState == LOW && lastEntryState == HIGH) {
    delay(50); // debounce
    if (digitalRead(irEntry) == LOW) {
      peopleCount++;
      Serial.printf("➡️ Entry detected. People count: %d\n", peopleCount);
      if (now - lastNotify > notifyInterval) {
        sendTelegram("➡️ Entry detected. Total people: " + String(peopleCount));
        lastNotify = now;
      }
    }
  }

  // Exit Detection
  if (exitState == LOW && lastExitState == HIGH) {
    delay(50);
    if (digitalRead(irExit) == LOW) {
      if (peopleCount > 0) peopleCount--;
      Serial.printf("⬅️ Exit detected. People count: %d\n", peopleCount);
      if (now - lastNotify > notifyInterval) {
        sendTelegram("⬅️ Exit detected. Total people: " + String(peopleCount));
        lastNotify = now;
      }
    }
  }

  lastEntryState = entryState;
  lastExitState  = exitState;

  delay(5); // small delay for stability
}
