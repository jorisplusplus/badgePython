#include <esp_log.h>
#include <esp_timer.h>

#include "Adafruit_PixelDust.h"
#include "demo.h"
#include "driver/i2c.h"
#include "driver_hub75.h"

#define N_FLAKES 1200
#define WIDTH 32
#define HEIGHT 19

volatile bool running = true;
int nFlakes = N_FLAKES;  // Runtime flake count (adapts to res)
int i;
int simOption;
Adafruit_PixelDust *snow;
dimension_t x, y;

Color *sandbuffer;

demo::demo() {
  snow = NULL;
  ESP_ERROR_CHECK(i2c_driver_install(I2C_NUM_0, I2C_MODE_MASTER, 32, 32, 0));

  i2c_config_t i2c_config = {};

  i2c_config.mode = I2C_MODE_MASTER;
  i2c_config.sda_io_num = GPIO_NUM_8;
  i2c_config.scl_io_num = GPIO_NUM_9;
  i2c_config.sda_pullup_en = false;
  i2c_config.scl_pullup_en = false;
  i2c_config.master.clk_speed = 100000;

  ESP_ERROR_CHECK(i2c_param_config(I2C_NUM_0, &i2c_config));

  uint8_t payload[] = {0x20, 0b01010111};
  i2c_master_write_to_device(I2C_NUM_0, 0x19, payload, 2, -1);
}

void demo::selectSim(int option, int flakes) {
  nFlakes = flakes;
  simOption = option;
  if (snow) delete (snow);
  if (simOption == 0) {
    snow = new Adafruit_PixelDust(WIDTH, HEIGHT, nFlakes, 2, 180, true);
    if (!snow->begin()) {
      printf("PixelDust init failed\n");
    }

    snow->randomize();  // Initialize random snowflake positions
  }
}

void demo::dispSnow() {
  uint8_t address;
  int16_t ay, ax, az;

  address = 0x28 | 0x80;
  i2c_master_write_read_device(I2C_NUM_0, 0x19, &address, 1, (uint8_t *)&ax, 2, -1);
  address = 0x2A | 0x80;
  i2c_master_write_read_device(I2C_NUM_0, 0x19, &address, 1, (uint8_t *)&ay, 2, -1);
  address = 0x2C | 0x80;
  i2c_master_write_read_device(I2C_NUM_0, 0x19, &address, 1, (uint8_t *)&az, 2, -1);

  snow->iterate(-ax, -ay, 0);

  // Erase canvas and draw new snowflake positions
  for (i = 0; i < WIDTH * HEIGHT; i++) {
    sandbuffer[i].value = 0;
  }

  for (i = 0; i < nFlakes; i++) {
    snow->getPosition(i, &x, &y);
    sandbuffer[x + y * WIDTH].RGB[0] = 0;
    sandbuffer[x + y * WIDTH].RGB[1] = i+50;
    sandbuffer[x + y * WIDTH].RGB[2] = 10;
  }
}

void demo::setBuffer(Color *framebuffer) {
  sandbuffer = framebuffer;
}

extern "C" void init_particlesim() {
  demo *dem = new demo();
  dem->selectSim(0, 150);
  dem->setBuffer(getFrameBuffer());
  compositor_disable();
  while (1) {
    int64_t time = esp_timer_get_time();
    dem->dispSnow();
    int64_t delta = esp_timer_get_time() - time;
    ESP_LOGI("Delta", "%lld", delta);
    vTaskDelay(1);
    
  }
}

extern "C" void destroy_particlesim() {
}