#include "config.h"

#define TAG "MAIN"
#define PROCESS_TAG "PROCESS"

static i2s_chan_handle_t tx_chan;

static int16_t buffer[DMA_BUF_LEN];

void process_task(void *pvParams){

    float phase = 0.0f;
    const float step = 2.0f * M_PI * SINE_FREQ / SAMPLE_RATE;

    ESP_LOGI(PROCESS_TAG, "Enviando senoidal de 1 kHz ...");

    while(1){

        for (int i = 0; i < DMA_BUF_LEN; i++)
        {
            buffer[i] = (int16_t)(AMPLITUDE * sinf(phase));
            phase += step;
            if (phase >= 2.0f * M_PI)
                phase -= 2.0f * M_PI;
        }

        size_t bytes_written = 0;

        ESP_ERROR_CHECK(i2s_channel_write(tx_chan, buffer, sizeof(buffer), &bytes_written, portMAX_DELAY));
    }
}

void app_main(void)
{
    ESP_LOGI(TAG, "Iniciando el sistema...");

    ESP_ERROR_CHECK(i2s_init(&tx_chan));

    xTaskCreate(process_task, "PROCESS TASK", configMINIMAL_STACK_SIZE*4, NULL, tskIDLE_PRIORITY+PROCESS_PTY, NULL);

    vTaskDelete(NULL);
}