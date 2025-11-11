#include <driver/i2s.h>
#include <math.h>

#define I2S_BCK   3
#define I2S_LRCK  9
#define I2S_DATA  10

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("I2S sine test...");

  i2s_config_t cfg = {
    .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_TX),
    .sample_rate = 44100,
    .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,
    .channel_format = I2S_CHANNEL_FMT_RIGHT_LEFT,
    .communication_format = I2S_COMM_FORMAT_I2S,
    .intr_alloc_flags = 0,
    .dma_buf_count = 8,
    .dma_buf_len = 64,
    .use_apll = false
  };
  i2s_pin_config_t pins = {
    .bck_io_num = I2S_BCK,
    .ws_io_num = I2S_LRCK,
    .data_out_num = I2S_DATA,
    .data_in_num = I2S_PIN_NO_CHANGE
  };
  i2s_driver_install(I2S_NUM_0, &cfg, 0, NULL);
  i2s_set_pin(I2S_NUM_0, &pins);
}

void loop() {
  static float phase = 0;
  const float freq = 440.0;
  const float sr = 44100.0;
  int16_t buf[128*2];
  for (int i = 0; i < 128; i++) {
    float s = sinf(phase) * 30000;
    buf[2*i] = buf[2*i+1] = (int16_t)s;
    phase += 2 * M_PI * freq / sr;
    if (phase > 2*M_PI) phase -= 2*M_PI;
  }
  size_t written;
  i2s_write(I2S_NUM_0, buf, sizeof(buf), &written, portMAX_DELAY);
}
