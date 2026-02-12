#include <TetrisMatrixDraw.h>
#include <TetrisLetters.h>
#include <TetrisNumbers.h>
#include <WiFi.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// WiFi
const char* ssid = "";
const char* password = "";

// OLED
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1
#define OLED_ADDR     0x3C
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Tetris
TetrisAnimation animation;

// Callback para dibujar píxeles
void drawPixel(int x, int y, uint16_t color) {
  display.drawPixel(x, y, color ? WHITE : BLACK);
}

// NTP
const long  gmtOffset_sec = -10800;
const int   daylightOffset_sec = 0;

void setup() {
  Serial.begin(115200);

  Wire.begin(15, 13); // Ajusta según tu placa

  if(!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR)) {
    Serial.println(F("OLED no encontrado"));
    for(;;);
  }

  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.println("Conectando WiFi...");
  display.display();

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi Conectado.");

  configTime(gmtOffset_sec, daylightOffset_sec, "pool.ntp.org");

  animation.setDrawPixelCallback(drawPixel);
  animation.begin(4, 1); // tamaño de bloque, retardo
}

int lastMinute = -1;

void loop() {
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)) return;

  if (timeinfo.tm_min != lastMinute) {
    lastMinute = timeinfo.tm_min;
    char timeStr[6];
    sprintf(timeStr, "%02d:%02d", timeinfo.tm_hour, timeinfo.tm_min);
    animation.setTime(timeStr);
  }

  display.clearDisplay();
  animation.draw();
  display.display();
  delay(100);
}
