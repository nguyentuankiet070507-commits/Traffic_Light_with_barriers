#include <ESP32Servo.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

Adafruit_SSD1306 display(
  SCREEN_WIDTH,
  SCREEN_HEIGHT,
  &Wire,
  -1
);

#define R 5
#define Y 18
#define G 19

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("OLED is not found!");
    while (true);
  }

  display.clearDisplay();
  display.setTextSize(2);
  display.setCursor(60, 30);
  display.setTextColor(WHITE);
  Serial.println("Start!");
  pinMode(R, OUTPUT);
  pinMode(Y, OUTPUT);
  pinMode(G, OUTPUT);

  digitalWrite(R, LOW);
  digitalWrite(Y, LOW);
  digitalWrite(G, LOW);
}

void loop() {
  delay(500);

  // Phase 1: Red light (5 s)
  digitalWrite(R, HIGH);

  for (long i = 5000; i >= 0; i -= 1000) {
    display.clearDisplay();
    display.setTextSize(2);
    display.setCursor(60, 30);
    display.setTextColor(WHITE);
    display.println(i / 1000);
    display.display();
    delay(1000);
  }
  digitalWrite(R, LOW);
  // Phase 2: Yellow light (2 s)
  digitalWrite(Y, HIGH);
  
  for (long i = 2000; i >= 0; i -= 1000) {
    display.clearDisplay();
    display.setTextSize(2);
    display.setCursor(60, 30);
    display.setTextColor(WHITE);
    display.println(i / 1000);
    display.display();
    delay(1000);
  }
  digitalWrite(Y, LOW);
  // Phase 3: Green light (3 s)
  digitalWrite(G, HIGH);
  
  for (long i = 3000; i >= 0; i -= 3000) {
    display.clearDisplay();
    display.setTextSize(2);
    display.setCursor(60, 30);
    display.setTextColor(WHITE);
    display.println(i / 1000);
    display.display();
    delay(1000);
  }
  digitalWrite(G, LOW);


}
