#include <WiFi.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "time.h"

//DATOS WIFI
const char* ssid     = "TU_NOMBRE_WIFI";
const char* password = "TU_CONTRASEÑA_WIFI";

//PANTALLA OLED
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1
#define OLED_ADDR     0x3C 
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

//HORA
const long  gmtOffset_sec = -10800;
const int   daylightOffset_sec = 0;

void setup() {
  Serial.begin(115200);
  
  Wire.begin(15, 13);

  if(!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR)) {
    Serial.println(F("¡Fallo OLED! Revisa cables en 15 y 13"));
    for(;;);
  }
  
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.setCursor(10, 20);
  display.println("Conectando WiFi...");
  display.display();

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi Conectado.");

  configTime(gmtOffset_sec, daylightOffset_sec, "pool.ntp.org");
}

void loop() {
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    Serial.println("Error obteniendo hora");
    return;
  }

  display.clearDisplay();

  //DIBUJAR LA HORA
  display.setTextSize(3);
  display.setTextColor(WHITE);
  
  display.setCursor(20, 10); 
  
  if(timeinfo.tm_hour < 10) display.print("0");
  display.print(timeinfo.tm_hour);
  
  if(timeinfo.tm_sec % 2 == 0) {
    display.print(":"); 
  } else {
    display.print(" ");
  }
  
  if(timeinfo.tm_min < 10) display.print("0");
  display.print(timeinfo.tm_min);

  //DIBUJAR FECHA
  display.setTextSize(1);
  display.setCursor(35, 40);
  // Formato simple: DD/MM
  display.print(timeinfo.tm_mday);
  display.print("/");
  display.print(timeinfo.tm_mon + 1);
  display.print("/");
  display.print(timeinfo.tm_year + 1900);

  //ANIMACIÓN: BARRA DE SEGUNDOS
  int barWidth = map(timeinfo.tm_sec, 0, 60, 0, SCREEN_WIDTH);
  
  display.fillRect(0, 55, barWidth, 8, WHITE); 
  
  display.drawRect(0, 55, SCREEN_WIDTH, 8, WHITE);

  display.display();
  delay(200);
}
