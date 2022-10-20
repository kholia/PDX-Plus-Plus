#include <si5351.h>
#include "Wire.h"
#include <EEPROM.h>

#define RX 18 // RX SWITCH (GP18)

Si5351 si5351;

int cal_factor = 163000;

// int cal_factor = 0;

// Debug helper
void led_flash()
{
  digitalWrite(LED_BUILTIN, HIGH);
  delay(250);
  digitalWrite(LED_BUILTIN, LOW);
  delay(250);
  digitalWrite(LED_BUILTIN, HIGH);
  delay(250);
  digitalWrite(LED_BUILTIN, LOW);
  delay(250);
  digitalWrite(LED_BUILTIN, HIGH);
  delay(250);
  digitalWrite(LED_BUILTIN, LOW);
  delay(250);
  digitalWrite(LED_BUILTIN, HIGH);
  delay(250);
  digitalWrite(LED_BUILTIN, LOW);
}

void setup()
{
  int ret;

  pinMode(RX, OUTPUT);
  digitalWrite(RX, 1);
  Serial.begin(115200);

  // I2C pins
  Wire.setSDA(16);
  Wire.setSCL(17);
  Wire.begin();

  // Initialize the Si5351
  ret = si5351.init(SI5351_CRYSTAL_LOAD_8PF, 0, cal_factor);
  if (ret != true) {
    led_flash();
    watchdog_reboot(0, 0, 1000);
  }

  // si5351.set_pll(SI5351_PLL_FIXED, SI5351_PLLA);
  si5351.drive_strength(SI5351_CLK1, SI5351_DRIVE_2MA); // Set for reduced power for RX
  si5351.set_freq(21074000 * 100ULL, SI5351_CLK1);
  si5351.output_enable(SI5351_CLK1, 1);
}

void loop()
{
  Serial.println("Alive!");
  delay(100);
}
