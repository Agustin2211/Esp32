#include <WiFi.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <TetrisAnimation.h> // Esta es la librería de Toblum que acabas de instalar

// --- TUS DATOS WIFI ---
const char* ssid     = "TU_NOMBRE_WIFI";
const char* password = "TU_CONTRASEÑA_WIFI";

// --- CONFIGURACIÓN PANTALLA OLED ---
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1
#define OLED_ADDR     0x3C 
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// --- TETRIS ---
TetrisAnimation animation;

// Esta función conecta la lógica del Tetris con tu pantalla OLED
void drawPixel(int x, int y, uint16_t color) {
  // La librería manda color, nosotros pintamos Blanco o Negro
  display.drawPixel(x, y, color ? WHITE : BLACK);
}

// --- HORA (NTP) ---
const long  gmtOffset_sec = -10800; // Configurado para -3 (Arg/Uru/Chi)
const int   daylightOffset_sec = 0;

void setup() {
  Serial.begin(115200);

  // 1. Configuración de tus pines (SDA=15, SCL=13)
  Wire.begin(15, 13);

  // 2. Iniciar Pantalla
  if(!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR)) {
    Serial.println(F("¡Error! No encuentro la pantalla OLED."));
    Serial.println(F("Verifica cables en pines 15 y 13"));
    for(;;);
  }
  
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.setCursor(0,0);
  display.println("Conectando WiFi...");
  display.display();

  // 3. Conectar WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi Conectado.");

  // 4. Configurar hora de internet
  configTime(gmtOffset_sec, daylightOffset_sec, "pool.ntp.org");
  
  // 5. Configurar Tetris
  // Aquí le decimos a la librería "Toblum" cómo dibujar en nuestra pantalla
  animation.setDrawPixelCallback(drawPixel);
  
  // Iniciar animación (Tamaño bloque: 4, Velocidad: 1)
  animation.begin(4, 1); 
}

int lastMinute = -1;

void loop() {
  struct tm timeinfo;
  
  // Intentar leer la hora
  if(!getLocalTime(&timeinfo)){
    return; // Si falla, intentamos en la siguiente vuelta
  }

  // Si cambió el minuto, le decimos al Tetris que "construya" la nueva hora
  if (timeinfo.tm_min != lastMinute) {
    lastMinute = timeinfo.tm_min;
    
    char timeStr[6];
    // Formato HH:MM
    sprintf(timeStr, "%02d:%02d", timeinfo.tm_hour, timeinfo.tm_min);
    
    Serial.print("Hora cambio a: ");
    Serial.println(timeStr);
    
    // Esta función resetea el tablero y empieza a dejar caer los nuevos números
    animation.setTime(timeStr);
  }

  // --- DIBUJAR CUADRO ---
  display.clearDisplay();
  
  // La librería calcula dónde va cada bloque en este milisegundo
  animation.draw(); 
  
  display.display(); // Muestra el resultado en la OLED real
  
  delay(50); // Controla la velocidad de la animación
}
