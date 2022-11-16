#include <Arduino.h>
#include "pdx_common.h"

// Automatic calibration
uint32_t f_hi_;

void pwm_int_() {
  pwm_clear_irq(7);
  f_hi_++;
}

void do_calibration()
{
  gpio_set_function(CAL, GPIO_FUNC_PWM);

  // Set CLK0 output for calibration
  si5351.drive_strength(SI5351_CLK2, SI5351_DRIVE_2MA); // Set for minimum power as we have an external amplifier
  si5351.set_freq(10000000 * 100ULL, SI5351_CLK2); // used for calibration
  si5351.output_enable(SI5351_CLK2, 1);

  // Warmup
  Serial1.println("Warming up for 7 seconds...");
  Serial1.println("Warming up for 7 seconds...");
  Serial1.println("Warming up for 7 seconds...");
  Serial1.println("Warming up for 7 seconds...");
  Serial1.println("Warming up for 7 seconds...");
  Serial1.println("Warming up for 7 seconds...");
  delay(7000);

  // Frequency counter
  int64_t existing_error = 0;
  int64_t error = 0;
  int count = 3; // reverse count
  uint64_t target_freq = 1000000000ULL; // 10 MHz, in hundredths of hertz
  uint32_t f = 0;
  while (true) {
    count = count - 1;
    pwm_config cfg = pwm_get_default_config();
    pwm_config_set_clkdiv_mode(&cfg, PWM_DIV_B_RISING);
    pwm_init(7, &cfg, false);
    gpio_set_function(CAL, GPIO_FUNC_PWM);
    irq_set_exclusive_handler(PWM_IRQ_WRAP, pwm_int_);
    pwm_set_irq_enabled(7, true);
    irq_set_enabled(PWM_IRQ_WRAP, true);
    f_hi_ = 0;
    si5351.set_correction(0, SI5351_PLL_INPUT_XO); // important - reset calibration
    uint32_t t = time_us_32() + 2;
    while (t > time_us_32());
    pwm_set_enabled(7, true);
    t += 3000000; // Gate time (in uSeconds), 3 seconds
    // t += 100000; // Gate time (in uSeconds), 100 ms
    while (t > time_us_32());
    pwm_set_enabled(7, false);
    f = pwm_get_counter(7);
    f += f_hi_ << 16;
    Serial1.print(f / 3.0); // Divide by gate time in seconds
    Serial1.println(" Hz");
    error = ((f / 3.0) * 100ULL) - target_freq;
    Serial1.print("Current calibration correction value is ");
    Serial1.printf("%" PRId64 "\n", error);
    Serial1.print("Total calibration value is ");
    Serial1.println(error + existing_error);
    if (count <= 0) { // Auto-calibration logic
      Serial1.println();
      Serial1.print(F("Calibration value is "));
      Serial1.println(error);
      Serial1.println(F("Setting calibration value automatically"));
      si5351.set_correction(error + existing_error, SI5351_PLL_INPUT_XO);
      existing_error = existing_error + error;
      si5351.set_pll(SI5351_PLL_FIXED, SI5351_PLLA);
      Serial1.println(F("Resetting target frequency"));
      si5351.set_freq(target_freq, SI5351_CLK0);
      // count = 3;
      break;
    }
  }

  // Save to EEPROM
  uint32_t magic = 0x1CEB00DA;
  EEPROM.put(0, magic);
  EEPROM.put(4, (uint32_t)existing_error);
  EEPROM.commit();
  delay(7000);
  // Serial1.printf("Saving calibration factor (%d) to EEPROM\n", existing_error);
}

uint32_t base_frequency = 700; // TODO: Avoid hardcoding this

// Interrupt IRQ for edge counting overflow
void pwm_int() {
  pwm_clear_irq(pwm_slice);
  f_hi++;
}

void setup1() {
  uint32_t magic;

  while (core1_initialization_partially_done == 0)
    delay(10);

  // Check for existing calibration
  EEPROM.begin(512);
  EEPROM.get(0, magic);
  if (magic != 0x1CEB00DA) {
    do_calibration();
    led_flash();
    watchdog_reboot(0, 0, 1000);
  }

  core2_initialization_partially_done = 1;

  /*
     ZCD algorithm
     defined by FSK_ZCD
     this is based on a pseudo cross detect
     where the rising edge is taken as a
     false cross detection followed by next
     edge which is also a false zcd but
     at the same level thus measuring the
     time between both will yield a period
     measurement proportional to the real
     period of the signal as measured
     two sucessive rising edges
     Measurements are made every 1 mSec
  */
  pwm_slice = pwm_gpio_to_slice_num(FSK);

  while (true) {
    pwm_config cfg = pwm_get_default_config();
    pwm_config_set_clkdiv_mode(&cfg, PWM_DIV_B_RISING);
    pwm_init(pwm_slice, &cfg, false);
    gpio_set_function(FSK, GPIO_FUNC_PWM);
    pwm_set_irq_enabled(pwm_slice, true);
    irq_set_exclusive_handler(PWM_IRQ_WRAP, pwm_int);
    irq_set_enabled(PWM_IRQ_WRAP, true);
    f_hi = 0;

    uint32_t t = time_us_32() + 2;                     // Allow all the settings to stabilize
    while (t > time_us_32());                          //
    uint32_t pwm_cnt = pwm_get_counter(pwm_slice);     // Get current pwm count
    pwm_set_enabled(pwm_slice, true);                  // enable pwm count
    while (pwm_get_counter(pwm_slice) == pwm_cnt) {}   // Wait till the count change
    pwm_cnt = pwm_get_counter(pwm_slice);              // Measure that value
    uint32_t t1 = time_us_32();                        // Mark first tick (t1)
    while (pwm_get_counter(pwm_slice) == pwm_cnt) {}   // Wait till the count change (a rise edge)
    uint32_t t2 = time_us_32();                        // Mark the second tick (t2)
    pwm_set_enabled(pwm_slice, false);                 // Disable counting
    if (t2 != t1) {                                    // Prevent noise to trigger a nul measurement
      float f = double(FSK_USEC) / (t2 - t1);          // Ticks are expressed in uSecs so convert to Hz
      f = f;                                           // Round to the nearest integer
      ffsk = uint32_t(f);                              // Convert to long integer for actual usage
      // TODO: Relax this - make it flexible / smarter
      if (ffsk >= base_frequency && ffsk <= base_frequency + 50) { // Only yield a value if within the baseband
        fsequences[nfsi] = f;
        rp2040.fifo.push_nb(nfsi);                     // Use the rp2040 FIFO IPC to communicate the new frequency
        nfsi = (nfsi + 1) % NFS;                       //
      }                                                //
    }                                                  //
    t = time_us_32() + FSK_SAMPLE;                     // Now wait for 1 mSec till next sample
    while (t > time_us_32()) ;
  }
}
