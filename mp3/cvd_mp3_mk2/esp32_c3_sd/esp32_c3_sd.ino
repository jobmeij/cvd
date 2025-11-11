// Requires library "ESP8266Audio" (AudioGeneratorMP3, AudioFileSourceSD, AudioOutputI2S)
// Install via Arduino Library Manager: "ESP8266Audio" by Earle F. Philhower

#include <SPI.h>
#include <SD.h>
#include <AudioFileSourceSD.h>
#include <AudioGeneratorMP3.h>
#include <AudioOutputI2S.h>

//
// --- Pin configuration ---
// Change these if you move pins. Beware: GPIO0 is a boot strapping pin; GPIO1 is UART TX.
#define SD_CS    7   // SD chip select
#define SD_SCK   4
#define SD_MISO  5
#define SD_MOSI  6

// I2S pins (you said you wired these)
#define I2S_BCK     3  // Bit clock (BCK)
#define I2S_LRCK    9  // Word select / LRCK
#define I2S_DATA    10 // Data out (DIN on PCM5102)

const char *MP3_FILENAME = "/Rick Astley - Never Gonna Give You Up.mp3"; // change to your filename (case sensitive on FAT32)

AudioGeneratorMP3 *mp3 = nullptr;
AudioFileSourceSD *file = nullptr;
AudioOutputI2S *out = nullptr;

void setup() {
  Serial.begin(115200);
  while (!Serial) { delay(1); } // wait serial (useful for USB serial)

  Serial.println("\n=== MP3 -> PCM5102 I2S demo ===");

  // Initialize SPI for SD with your custom pins
  SPI.begin(SD_SCK, SD_MISO, SD_MOSI, SD_CS);
  if (!SD.begin(SD_CS, SPI)) {
    Serial.println("SD.begin() failed! Check wiring and CS pin.");
    while (1) delay(1000);
  }
  Serial.println("SD initialized.");

  // List files on the card (quick check)
  File root = SD.open("/");
  Serial.println("Files on SD root:");
  printDirectory(root, 0);
  root.close();

  // Try open mp3 file
  file = new AudioFileSourceSD(MP3_FILENAME);
  if (!file || !file->isOpen()) {
    Serial.printf("Failed to open '%s'\n", MP3_FILENAME);
    delete file;
    file = nullptr;
    return;
  }
  Serial.printf("Opened '%s'\n", MP3_FILENAME);

  // Setup I2S output
  out = new AudioOutputI2S();
  // Optional: reduce volume/gain if too loud (0..1.0)
  out->SetGain(0.1f);

  // IMPORTANT: set I2S pinout (bck, ws/lrck, data)
  out->SetPinout(I2S_BCK, I2S_LRCK, I2S_DATA);

  // Create MP3 generator and begin streaming
  mp3 = new AudioGeneratorMP3();
  mp3->begin(file, out);

  Serial.println("Playback started.");
}

void loop() {
  if (mp3 && mp3->isRunning()) {
    if (!mp3->loop()) {
      Serial.println("MP3 finished or error. Cleaning up.");
      mp3->stop();
    }
  }

  // if (Serial.available()) {
  //   char c = Serial.read();
  //   if (c == '+') out->SetGain(out->GetGain() + 0.1f);
  //   if (c == '-') out->SetGain(out->GetGain() - 0.1f);
  // }

  delay(10);
}

void printDirectory(File dir, int numTabs) {
  while (true) {
    File entry = dir.openNextFile();
    if (!entry) break;
    for (int i = 0; i < numTabs; i++) Serial.print('\t');
    Serial.print(entry.name());
    if (entry.isDirectory()) {
      Serial.println("/");
      printDirectory(entry, numTabs + 1);
    } else {
      Serial.print("\t\t");
      Serial.println(entry.size(), DEC);
    }
    entry.close();
  }
}
