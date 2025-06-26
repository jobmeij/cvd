#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DFRobotDFPlayerMini.h>
#include <SoftwareSerial.h>

// OLED scherm instellingen
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// DFPlayer serial communicatie
SoftwareSerial dfSerial(7, 6); // RX, TX
DFRobotDFPlayerMini dfplayer;

// Knoppen pin nummers
const int buttonForward = 4;
const int buttonSelect  = 3;
const int buttonBack    = 2;

// Menu
int menuIndex = 0;
const int menuCount = 4;
int currentTrack = 1;
int volumeLevel = 10;
bool isPlaying = false;
bool repeat = false;

// Debounce voor knopjes
unsigned long lastDebounce = 0;
const unsigned long debounceDelay = 200;

// Bitmaps voor icoontjes
const unsigned char PROGMEM playIcon [] = {
  0b00011000,
  0b00011100,
  0b00011110,
  0b00011111,
  0b00011110,
  0b00011100,
  0b00011000,
  0b00000000
};

const unsigned char PROGMEM pauseIcon [] = {
  0b11000011,
  0b11000011,
  0b11000011,
  0b11000011,
  0b11000011,
  0b11000011,
  0b11000011,
  0b00000000
};

const unsigned char PROGMEM repeatIcon [] = {
  0b00111100,
  0b01000010,
  0b10000001,
  0b01000010,
  0b00111100,
  0b00000000,
  0b00000000,
  0b00000000
};

void setup() {
  Serial.begin(9600);

  // Display
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("OLED niet gevonden");
    while (1);
  }
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println("Start...");
  display.display();

  // Knoppen
  pinMode(buttonForward, INPUT_PULLUP);
  pinMode(buttonSelect, INPUT_PULLUP);
  pinMode(buttonBack, INPUT_PULLUP);

  // DFPlayer
  dfSerial.begin(9600);
  delay(5000);
  while (!dfplayer.begin(dfSerial)) {
    Serial.println("DFPlayer niet gevonden!");
    display.println("DFPlayer fout!");
    display.display();
    delay(1000);
  }
  Serial.println("DFPlayer gestart.");
  display.println("DFPlayer gestart");
  display.display();

  dfplayer.volume(volumeLevel);
  dfplayer.play(currentTrack);
  isPlaying = true;

  delay(1000);
  updateDisplay();
}

void loop() {
  if (millis() - lastDebounce > debounceDelay) {
    if (digitalRead(buttonForward) == LOW) {
      handleForward();
      lastDebounce = millis();
    }
    if (digitalRead(buttonSelect) == LOW) {
      handleSelect();
      lastDebounce = millis();
    }
    if (digitalRead(buttonBack) == LOW) {
      handleBack();
      lastDebounce = millis();
    }
  }
}

void handleForward() {
  switch (menuIndex) {
    case 0:
      currentTrack++;
      dfplayer.play(currentTrack);
      isPlaying = true;
      break;
    case 1:
      if (volumeLevel < 30) {
        volumeLevel++;
        dfplayer.volume(volumeLevel);
      }
      break;
    case 2:
      if (isPlaying) {
        dfplayer.pause();
        isPlaying = false;
      } else {
        dfplayer.play(currentTrack);
        isPlaying = true;
      }
      break;
    case 3:
      repeat = !repeat;
      if (repeat) {
        dfplayer.enableLoop();
      } else {
        dfplayer.disableLoop();
        dfplayer.play(currentTrack);
      }
      break;
  }
  updateDisplay();
}

void handleBack() {
  switch (menuIndex) {
    case 0:
      if (currentTrack > 1) currentTrack--;
      dfplayer.play(currentTrack);
      isPlaying = true;
      break;
    case 1:
      if (volumeLevel > 0) {
        volumeLevel--;
        dfplayer.volume(volumeLevel);
      }
      break;
    case 2:
      if (isPlaying) {
        dfplayer.pause();
        isPlaying = false;
      } else {
        dfplayer.play(currentTrack);
        isPlaying = true;
      }
      break;
    case 3:
      repeat = !repeat;
      if (repeat) {
        dfplayer.enableLoop();
      } else {
        dfplayer.disableLoop();
        dfplayer.play(currentTrack);
      }
      break;
  }
  updateDisplay();
}

void handleSelect() {
  menuIndex++;
  if (menuIndex >= menuCount) menuIndex = 0;
  updateDisplay();
}

void updateDisplay() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 0);

  for (int i = 0; i < menuCount; i++) {
    if (i == menuIndex) display.print("> ");
    else display.print("  ");

    switch (i) {
      case 0:
        display.print("Track: ");
        display.println(currentTrack);
        break;
      case 1:
        display.print("Volume: ");
        display.println(volumeLevel);
        break;
      case 2:
        display.println(isPlaying ? "Pauzeer" : "Hervat");
        break;
      case 3:
        display.print("Repeat: ");
        display.println(repeat ? "Aan" : "Uit");
        break;
    }
  }

  // Iconen
  display.drawBitmap(112, 0, isPlaying ? playIcon : pauseIcon, 8, 8, SSD1306_WHITE);
  // display.drawBitmap(112, 16, repeat ? repeatIcon : NULL, 8, 8, SSD1306_WHITE);
  if (repeat) {
    display.drawBitmap(112, 16, repeatIcon, 8, 8, SSD1306_WHITE);
  }


  // Volume bar
  display.drawRect(100, 56, 24, 6, SSD1306_WHITE);
  display.fillRect(102, 58, map(volumeLevel, 0, 30, 0, 20), 2, SSD1306_WHITE);

  display.display();
}
