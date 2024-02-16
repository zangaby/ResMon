#include <Wire.h>
#include "SH1106Wire.h"
#include "images.h"

SH1106Wire display(0x3c, SDA, SCL);

typedef void (*SysData)(int, int, String);

int Mode = 0;

void drawSplashScreen() {
  display.clear();

  display.setFont(ArialMT_Plain_16);
  display.setTextAlignment(TEXT_ALIGN_CENTER);
  display.drawString(64, 20, "SysResMon");
  display.display();
  delay(3000);  // Display splash screen for 3 seconds
}

void setup() {
  Serial.begin(115200);
  Serial.println();

  display.init();
  display.flipScreenVertically();
  display.setFont(ArialMT_Plain_10);

  drawSplashScreen();  // Display the splash screen
}

void drawSystemInfo(int cpuLoad, int memoryUsage, String ipAddress) {
  display.clear();

  display.setFont(ArialMT_Plain_16);
  display.drawXbm(5, 2, cpu_width, cpu_height, cpu_bits);
  display.drawString(80, 2, String(cpuLoad) + " %");

  display.drawXbm(5, 22, memory_width, memory_height, memory_bits);
  display.drawString(110, 22, String(memoryUsage) + " MB");

  display.drawXbm(5, 40, ethernet_width, ethernet_height, ethernet_bits);
  display.setFont(ArialMT_Plain_10);
  display.drawString(105, 45, ipAddress);

  display.setTextAlignment(TEXT_ALIGN_RIGHT);
  display.display();
}

SysData sys[] = {drawSystemInfo};

void loop() {
  if (Serial.available() > 0) {
    String data = Serial.readStringUntil('\n');
    data.trim();

    // Split the string into values
    int commaIndex1 = data.indexOf(',');
    int commaIndex2 = data.indexOf(',', commaIndex1 + 1);

    if (commaIndex1 != -1 && commaIndex2 != -1) {
      float cpuLoad = data.substring(0, commaIndex1).toInt();
      float memoryUsage = data.substring(commaIndex1 + 1, commaIndex2).toInt();
      String ipAddress = data.substring(commaIndex2 + 1);

      // Call the sys function with the obtained data
      sys[Mode](cpuLoad, memoryUsage, ipAddress);

      // Display the information
      display.display();
    }
  }

  delay(1000);  // Adjust delay based on the refresh rate you want
}
