#include <WiFi.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <TetrisAnimation.h>

//CONFIGURACIÓN WIFI
const char* ssid     = "TU_NOMBRE_WIFI";
const char* password = "TU_CONTRASEÑA_WIFI";

//CONFIGURACIÓN PANTALLA
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1
#define OLED_ADDR     0x3C // Si no funciona, prueba 0x3D
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

//CONFIGURACIÓN TETRIS
TetrisAnimation animation;

void drawPixel(int x, int y, uint16_t color) {
  display.drawPixel(x, y, color ? WHITE : BLACK);
}

//CONFIGURACIÓN HORA
const long  gmtOffset_sec = -10800; 
const int   daylightOffset_sec = 0;

void setup() {
  Serial.begin(115200);

  if(!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR)) {
    Serial.println(F("¡Fallo pantalla OLED!"));
    for(;;);
  }
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(1);
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
  
  animation.setDrawPixelCallback(drawPixel); // Conectamos el puente
  
  animation.begin(4, 1); 
}

int lastMinute = -1;

void loop() {
  struct tm timeinfo;
  
  if(!getLocalTime(&timeinfo)){
    return;
  }

  if (timeinfo.tm_min != lastMinute) {
    lastMinute = timeinfo.tm_min;
    
    // Formatear la hora como texto "HH:MM"
    char timeStr[6];
    sprintf(timeStr, "%02d:%02d", timeinfo.tm_hour, timeinfo.tm_min);
    
    Serial.print("Nueva hora: ");
    Serial.println(timeStr);
    
    animation.setTime(timeStr);
  }

  //BUCLE DE ANIMACIÓN
  display.clearDisplay();
  
  animation.draw();
  
  display.display();
  
  delay(100); 
}
