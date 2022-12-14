#include <si5351.h>
#include "Wire.h"
#include <EEPROM.h>

#define RX 18 // RX SWITCH (GP18)

Si5351 si5351;

int cal_factor = 163000;

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
}

void setup()
{
  int ret;

  pinMode(RX, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(RX, 0); // Turn off RX
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

  si5351.output_enable(SI5351_CLK0, 0); // Safety first
  delay(7000);
}

void loop()
{
  led_flash();
  si5351.set_freq(14074000 * 100ULL, SI5351_CLK0);
  si5351.output_enable(SI5351_CLK0, 1);
  delay(7000);

  si5351.output_enable(SI5351_CLK0, 0);
  delay(7000);
}
