#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_cpu_utils.h"
#include "esp_timer.h"

#define ADC 33
#define DAC 25

void test_one_second();
uint32_t ticks_to_ms(uint32_t ticks);
uint32_t ticks_to_us(uint32_t ticks);
uint32_t ticks_to_ns(uint32_t ticks);

void app_main() {
    esp_rom_gpio_pad_select_gpio(ADC);
    gpio_set_direction(ADC, GPIO_MODE_INPUT);
    esp_rom_gpio_pad_select_gpio(DAC);
    gpio_set_direction(DAC, GPIO_MODE_OUTPUT);

    int core = esp_cpu_get_core_id();
    int cpu_freq = esp_rom_get_cpu_ticks_per_us();
    printf("Core: %d\n", core);
    printf("CPU freq: %d MHz\n", cpu_freq);

    test_one_second();

    for (int i = 0; i < 20; i++) {
        uint32_t start = xthal_get_ccount();
        int adc = gpio_get_level(ADC);

        gpio_set_level(DAC, adc);
        uint32_t end = xthal_get_ccount();

        uint32_t elapsed_time = ticks_to_ns(end - start);
        printf("Elapsed time: %ld ns\n", elapsed_time);
    }
}

void test_one_second() {
    uint32_t start = xthal_get_ccount();
    vTaskDelay(1000 / portTICK_PERIOD_MS); // 1 second delay
    uint32_t end = xthal_get_ccount();

    printf("Test 1 second: %ld us\n", ticks_to_us(end - start));
    printf("Test 1 second: %ld ms\n", ticks_to_ms(end - start));
}

uint32_t ticks_to_ms(uint32_t ticks) {
    return ticks / (esp_rom_get_cpu_ticks_per_us() * 1000);
}

uint32_t ticks_to_us(uint32_t ticks) {
    return ticks / esp_rom_get_cpu_ticks_per_us();
}

uint32_t ticks_to_ns(uint32_t ticks) {
    return ticks * 1000 / esp_rom_get_cpu_ticks_per_us();
}
