#include "config.h"

#define TAG "CONFIG"

esp_err_t i2s_init(i2s_chan_handle_t* tx_chan)
{
    i2s_chan_config_t tx_chan_cfg = I2S_CHANNEL_DEFAULT_CONFIG(I2S_NUM_AUTO, I2S_ROLE_MASTER);
    if (i2s_new_channel(&tx_chan_cfg, tx_chan, NULL) != ESP_OK) return false;

    i2s_std_config_t std_cfg = {
        .clk_cfg = I2S_STD_CLK_DEFAULT_CONFIG(SAMPLE_RATE),
        .slot_cfg = I2S_STD_PHILIPS_SLOT_DEFAULT_CONFIG(I2S_DATA_BIT_WIDTH_16BIT,
                                                        I2S_SLOT_MODE_MONO),
        .gpio_cfg = {
            .mclk = I2S_GPIO_UNUSED,
            .bclk = I2S_BCK_IO,
            .ws   = I2S_WS_IO,
            .dout = I2S_DO_IO,
            .din  = I2S_GPIO_UNUSED,
            .invert_flags = {
            .mclk_inv = false,
            .bclk_inv = false,
            .ws_inv = false,
            },
        },
    };

    if (i2s_channel_init_std_mode(*tx_chan, &std_cfg) != ESP_OK) return false;
    if (i2s_channel_enable(*tx_chan) != ESP_OK) return false;

    // Enviar un bloque de prueba
    int16_t buffer[DMA_BUF_LEN];
    float phase = 0.0f;
    const float step = 2.0f * M_PI * TEST_FREQ / SAMPLE_RATE;

    for (int i = 0; i < DMA_BUF_LEN; i++) {
        buffer[i] = (int16_t)(AMPLITUDE * sinf(phase));
        phase += step;
        if (phase >= 2.0f * M_PI) phase -= 2.0f * M_PI;
    }

    size_t bytes_written = 0;
    esp_err_t err = i2s_channel_write(*tx_chan, buffer, sizeof(buffer), &bytes_written, 1000);

    // i2s_channel_disable(tx_chan);

    // i2s_del_channel(tx_chan);

    if (err == ESP_OK && bytes_written == sizeof(buffer)) {
        ESP_LOGI(TAG, "✅ Canal I2S TX activo y transmisión OK");
        return ESP_OK;
    } else {
        ESP_LOGE(TAG, "❌ Error al escribir en el canal I2S (err=%d, bytes=%d)", err, (int)bytes_written);
        return ESP_ERR_TIMEOUT;
    }
}