#include <Wire.h>
#include <LiquidCrystal_I2C.h>

//CONFIGURACIÓN DE PINES Y PANTALLA
LiquidCrystal_I2C lcd(0x27, 16, 2); 

//BUZZER
#define BUZZER_PIN 25

//DEFINICIÓN DE NOTAS (Frecuencias) 
#define NOTE_G3  196
#define NOTE_AS3 233
#define NOTE_C4  262
#define NOTE_D4  294
#define NOTE_DS4 311
#define NOTE_F4  349
#define NOTE_G4  392
#define REST     0

//ESTRUCTURA DE LA CANCIÓN 
struct KaraokeStep {
  int note;      // Nota musical
  int duration;  // 4=negra, 8=corchea, 16=semi (más alto = más rápido)
  String text;
  bool clearLine;
};

//CANCIÓN: BLACKOUT
KaraokeStep song[] = {

  // "Y yo me le pe-gué"
  {NOTE_G3, 8, "Y", true}, 
  {NOTE_G3, 8, "yo", false}, 
  {NOTE_G3, 8, "me", false}, 
  {NOTE_G3, 8, "le", false}, 
  {NOTE_AS3, 4, "PEGUE", false},
  
  {REST, 8, "", false}, // Silencio breve

  // "y él se me pe-gó"
  {NOTE_G3, 8, "y", true}, 
  {NOTE_G3, 8, "el", false}, 
  {NOTE_G3, 8, "se", false}, 
  {NOTE_G3, 8, "me", false}, 
  {NOTE_AS3, 4, "PEGO", false},

  {REST, 8, "", false},

  // "(Woh-woh)"
  {NOTE_G3, 8, "woh", true}, 
  {NOTE_AS3, 8, "woh", false}, 
  {NOTE_C4, 4, "WOH!", false},
  
  // "To-da la no-che pe-gao"
  {NOTE_C4, 8, "To", true}, 
  {NOTE_C4, 8, "da", false}, 
  {NOTE_C4, 8, "la", false}, 
  {NOTE_C4, 8, "no", false}, 
  {NOTE_C4, 8, "che", false}, 
  {NOTE_AS3, 4, "PE-", false}, 
  {NOTE_G3, 2, "GAO", false},

  //  REPETICIÓN DEL ESTRIBILLO 
  {REST, 4, "...", true},
  
  {NOTE_G3, 8, "Y", true}, {NOTE_G3, 8, "yo", false}, {NOTE_G3, 8, "me", false}, {NOTE_G3, 8, "le", false}, {NOTE_AS3, 4, "PEGUE", false},
  {REST, 8, "", false},
  
  {NOTE_G3, 8, "y", true}, {NOTE_G3, 8, "el", false}, {NOTE_G3, 8, "se", false}, {NOTE_G3, 8, "me", false}, {NOTE_AS3, 4, "PEGO", false},
  {REST, 8, "", false},
  
  {NOTE_G3, 8, "woh", true}, {NOTE_AS3, 8, "woh", false}, {NOTE_C4, 4, "WOH", false},
  
  {NOTE_C4, 8, "To", true}, {NOTE_C4, 8, "da", false}, {NOTE_C4, 8, "la", false}, {NOTE_C4, 8, "no", false}, {NOTE_C4, 8, "che", false}, {NOTE_AS3, 4, "PE", false}, {NOTE_G3, 2, "GAO", false},

  //  PARTE RÁPIDA (EMILIA) 
  // "Tú y yo en la intimidad"
  {REST, 4, "...", true},
  {NOTE_C4, 16, "Tu", true}, {NOTE_C4, 16, "y", false}, {NOTE_C4, 16, "yo", false}, 
  {NOTE_C4, 16, "en", false}, {NOTE_C4, 16, "la", false}, {NOTE_DS4, 8, "INTI", false}, {NOTE_DS4, 8, "MIDAD", false},
  
  // "no va a pasar de acá"
  {REST, 8, "", false},
  {NOTE_C4, 16, "no", true}, {NOTE_C4, 16, "va", false}, {NOTE_C4, 16, "a", false}, 
  {NOTE_C4, 16, "pa", false}, {NOTE_DS4, 8, "SAR", false}, {NOTE_DS4, 8, "DE", false}, {NOTE_D4, 4, "ACA", false},

  // FINAL
  {REST, 1, "FIN :)", true}
};

int totalSteps = sizeof(song) / sizeof(KaraokeStep);
int tempo = 1400;

void setup() {
  Serial.begin(115200);

  Wire.begin(15, 13); 

  lcd.init();
  lcd.backlight();

  pinMode(BUZZER_PIN, OUTPUT);

  lcd.setCursor(0, 0);
  lcd.print("    CANCION:    ");
  lcd.setCursor(0, 1);
  lcd.print("    BLACKOUT    ");
  delay(3000);
  lcd.clear();
  
  // Dejamos el título fijo arriba
  lcd.setCursor(0, 0);
  lcd.print("Blackout-Emilia");
}

void loop() {
  
  for (int i = 0; i < totalSteps; i++) {
    int note = song[i].note;
    int duration = song[i].duration;
    String text = song[i].text;
    bool clearLine = song[i].clearLine;

    // Calcular duración en ms
    int noteDuration = tempo / duration;

    //  PANTALLA LCD 
    if (text != "") {
      if (clearLine) {
         lcd.setCursor(0, 1);
         lcd.print("                "); // 16 espacios vacíos
      }
      
      if(clearLine) {
        lcd.setCursor(0, 1);
      }
      
      lcd.print(text);
      lcd.print(" ");
    }

    //SONIDO 
    if (note != REST) {
      tone(BUZZER_PIN, note, noteDuration);
    }

    //PAUSA ENTRE NOTAS 
    int pause = noteDuration * 1.30;
    delay(pause);
    noTone(BUZZER_PIN);
  }

  // Reiniciar después de una pausa larga
  delay(5000);
}