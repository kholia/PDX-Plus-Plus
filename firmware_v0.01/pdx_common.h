#ifndef PDX_common
#define PDX_common

#define VERSION        "2.0"
#define BUILD          1

#include <stdint.h>
#include <si5351.h>

#include "hardware/watchdog.h"
#include "Wire.h"
#include <EEPROM.h>
#include "pico/stdlib.h"
#include "pico/binary_info.h"
#include "hardware/gpio.h"
#include "hardware/sync.h"
#include "hardware/structs/ioqspi.h"
#include "hardware/structs/sio.h"
#include <stdio.h>
#include "hardware/pwm.h"
#include "pico/multicore.h"
#include "hardware/adc.h"
#include "hardware/uart.h"

// Pinout
#define PDX_I2C_SDA    16 // I2C SDA
#define PDX_I2C_SCL    17 // I2C SCL
#define UART_TX        12
#define UART_RX        13
#define RX             18 // RX SWITCH (GP18)
#define FSK            27 // Frequency counter algorithm
#define CAL            15 // Automatic calibration entry

#define BAUD            38400

#define FSKMIN                        300 // Minimum FSK frequency computed
#define FSKMAX                       2500 // Maximum FSK frequency computed
#define FSK_USEC                  1000000
#define FSK_SAMPLE                   1000 // 1000 works fine!
#define FSK_ERROR                       4
#define FSK_RA                         20
#define FSK_IDLE      5*FSK_SAMPLE*FSK_RA

extern unsigned long  freq;
extern uint16_t       mode;

extern int32_t        cal_factor;
extern unsigned long  Cal_freq; // Calibration Frequency: 1 Mhz = 1000000 Hz

#define NFS 32
extern double         fsequences[NFS]; // Ring buffer for communication across cores
extern int            nfsi;
extern double         pfo; // Previous output frequency

extern Si5351         si5351;

extern uint32_t       ffsk;
extern int            pwm_slice;
extern uint32_t       f_hi;
extern uint32_t       fclk;
extern int32_t        error;
extern uint32_t       codefreq;
extern uint32_t       prevfreq;

extern int core1_initialization_partially_done;
extern int core2_initialization_partially_done;

void serialEvent();
void led_flash();

#endif
