#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_cpu_utils.h"
#include "esp_timer.h"

#define ADC 33
#define DAC 25

void testOneSecond();

void app_main() {
    esp_rom_gpio_pad_select_gpio(ADC);
    gpio_set_direction(ADC, GPIO_MODE_INPUT);
    esp_rom_gpio_pad_select_gpio(DAC);
    gpio_set_direction(DAC, GPIO_MODE_OUTPUT);

    int core = esp_cpu_get_core_id();
    int cycle_count = esp_cpu_get_cycle_count();
    int cpu_freq = esp_rom_get_cpu_ticks_per_us();
    printf("Core: %d\n", core);
    printf("Cycle count: %d\n", cycle_count);
    printf("CPU freq: %d MHz\n", cpu_freq);

    testOneSecond();

    for (int i = 0; i < 20; i++) {
        uint32_t start = xthal_get_ccount();
        // uint32_t start = esp_cpu_get_cycle_count();
        int adc = gpio_get_level(ADC);

        gpio_set_level(DAC, adc);
        uint32_t end = xthal_get_ccount();
        // uint32_t end = esp_cpu_get_cycle_count();
        
        uint32_t elapsed_time = (end - start) * 1000 / cpu_freq;
        printf("Elapsed time: %ld ns\n", elapsed_time);
    }
}

void testOneSecond() {
    uint32_t start = xthal_get_ccount();
    vTaskDelay(1000 / portTICK_PERIOD_MS); // 1 second delay
    uint32_t end = xthal_get_ccount();

    uint32_t cpu_freq = esp_rom_get_cpu_ticks_per_us();
    printf("Test 1 second: %ld us\n", (end - start) / cpu_freq);
    printf("Test 1 second: %ld ms\n", (end - start) / cpu_freq * 1000);
}