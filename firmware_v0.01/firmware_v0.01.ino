#include <si5351.h>
#include "Wire.h"
#include <EEPROM.h>

#include "pdx_common.h"

Si5351 si5351;

// Variables
int pwm_slice;
double fsequences[NFS];
uint32_t ffsk     = 0;
uint32_t f_hi;
uint32_t fclk     = 0;
int32_t  error    = 0;
uint32_t codefreq = 0;
int nfsi   = 0;
uint32_t prevfreq = 0;
double pfo;
unsigned long freq = 14074000; // Start on the "DX band (20m)" by default
int32_t cal_factor = 0; // Sorted out by auto-calibration algorithm
int core1_initialization_partially_done = 0;
int core2_initialization_partially_done = 0;
extern char inTx;

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

  // Pins
  pinMode(RX, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(CAL, INPUT);

  Serial.begin(115200);

  Serial1.setTX(12);
  Serial1.setRX(13);
  Serial1.begin(115200);

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
  si5351.output_enable(SI5351_CLK0, 0); // Turn of TX first - safety!

  core1_initialization_partially_done = 1; // Allow setup1() to run

  while (core2_initialization_partially_done == 0)
    delay(10);

  // Turn on RX
  digitalWrite(RX, 1);
  EEPROM.get(4, cal_factor);
  si5351.set_correction(cal_factor, SI5351_PLL_INPUT_XO);
  si5351.drive_strength(SI5351_CLK1, SI5351_DRIVE_2MA); // Set for reduced power for RX
  si5351.set_freq(freq * 100ULL, SI5351_CLK1);
  si5351.output_enable(SI5351_CLK1, 1);
}

void loop()
{
  int index;
  double fo;

  serialEvent(); // Listen for CAT control events

  if (rp2040.fifo.available() != 0 && inTx) {
    index = rp2040.fifo.pop();
    fo = fsequences[index];
    codefreq = fo;
    if (pfo != fo) {
      si5351.set_freq(((freq + fo) * 100ULL), SI5351_CLK0);
    }
  } else if (rp2040.fifo.available() != 0) {
    rp2040.fifo.pop(); // clear 'false' tx data
  }
}
