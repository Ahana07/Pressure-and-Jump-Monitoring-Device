#include <esp_now.h>
#include <WiFi.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

int LEDState = 0;
int LEDPin = 2;
int buttonPin = 18;
int var;
int flag = 1;
int maxDistance = 0;    // Variable to store the maximum distance

// Structure example to receive data
// Must match the sender structure
typedef struct struct_message {
    char a[32];
    int distance;
} struct_message;

// Create a struct_message called myData
struct_message myData;

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// callback function that will be executed when data is received
void OnDataRecv(const uint8_t *mac, const uint8_t *incomingData, int len) {
  memcpy(&myData, incomingData, sizeof(myData));
  
  if (LEDState == 1) {
    // While LED is ON (collecting data), keep track of max distance
    if (myData.distance > maxDistance) {
      maxDistance = myData.distance;
    }
  }
}

void setup() {
  Serial.begin(115200);
  
  pinMode(LEDPin, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);

  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  
  // Register for receive callback
  esp_now_register_recv_cb(esp_now_recv_cb_t(OnDataRecv));

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);
  }
  delay(2000);
  display.clearDisplay();

  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 10);
}

void loop() {
  var = digitalRead(buttonPin);
  if (flag == 1 && var == 0) {
    delay(300); // Debounce
    if (LEDState == 0) {
      digitalWrite(LEDPin, HIGH);
      LEDState = 1;
      maxDistance = 0; // Reset the maximum value at the start of collection
    } else {
      digitalWrite(LEDPin, LOW);
      LEDState = 0;
      
      // When the button is pressed again, display the maximum distance

    }
  }
  flag = var;

  if (LEDState == 1) {
    display.clearDisplay();
    display.setTextSize(3);
    display.setCursor(40, 25);
    display.println(myData.distance);
    display.display();
  } else {
      display.clearDisplay();
      display.setTextSize(2);
      display.setCursor(10, 25);
      display.print("Max: ");
      display.println(maxDistance);
      display.display();
  }
}
