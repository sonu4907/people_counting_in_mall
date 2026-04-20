
# ESP32 People Counter with Telegram Notifications

A real-time people counting system using ESP32, IR sensors, and Telegram Bot API for remote monitoring.

## 📌 Features
- Counts people entering/exiting using two IR obstacle sensors
- Sends real-time notifications via Telegram Bot
- Debounce logic for accurate detection
- Automatic WiFi reconnection
- URL-encoded messages for Telegram API compatibility

## 🛠️ Hardware Required
- ESP32 Development Board (any variant)
- 2x IR Obstacle Avoidance Sensors
- Jumper Wires
- USB Cable for programming

## 🔌 Pin Connections
| ESP32 Pin | Component     |
|-----------|---------------|
| GPIO 5    | Entry IR Sensor|
| GPIO 4    | Exit IR Sensor |
| 3.3V/VIN  | Sensor VCC    |
| GND       | Sensor GND    |

## ⚙️ Software Dependencies
- **Arduino IDE** or **PlatformIO**
- **Board Manager**: ESP32 by Espressif Systems
- **Libraries**:
  - WiFi.h (built-in)
  - HTTPClient.h (built-in)
  - WiFiClientSecure.h (built-in)

## 🚀 Setup Instructions

### 1. Telegram Bot Setup
1. Open Telegram and search for **@BotFather**
2. Send `/newbot` and follow instructions
3. Save the **Bot Token** provided
4. Send a message to your new bot
5. Visit `https://api.telegram.org/bot<YOUR_TOKEN>/getUpdates` to find your **Chat ID**

### 2. Configure the Code
Open `people_counter.ino` and update:
```cpp
const char* ssid     = "YOUR_WIFI_NAME";
const char* password = "YOUR_WIFI_PASSWORD";
const String botToken = "YOUR_BOT_TOKEN";
const String chatID   = "YOUR_CHAT_ID";
