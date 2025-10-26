#include <stdio.h>
#include <math.h>
#include "esp_log.h"
#include "driver/i2s_std.h"
#include "driver/gpio.h"
#include "soc/adc_channel.h"
#include "esp_adc/adc_continuous.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

// Pines (ajustar según tu conexión)
#define I2S_WS_IO    GPIO_NUM_4
#define I2S_BCK_IO   GPIO_NUM_5
#define I2S_DO_IO    GPIO_NUM_6

// Parámetros de audio
#define SAMPLE_RATE     48000
#define SINE_FREQ       1000.0f
#define DMA_BUF_LEN     128
#define DMA_BUF_COUNT   2
#define AMPLITUDE       15000  // máx: 32767 (16-bit)

#define TEST_FREQ       1000.0f

#define PROCESS_PTY     2
#define PROCESS_STACK   4

esp_err_t i2s_init(i2s_chan_handle_t* tx_chan);