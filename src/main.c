#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_cpu_utils.h"
#include "esp_timer.h"

#define ADC 33
#define DAC 25

void app_main() {
    esp_rom_gpio_pad_select_gpio(ADC);
    gpio_set_direction(ADC, GPIO_MODE_INPUT);
    esp_rom_gpio_pad_select_gpio(DAC);
    gpio_set_direction(DAC, GPIO_MODE_OUTPUT);

    // Set cpu freq to 240MHz
    // int core = esp_cpu_get_core_id();
    // int cycle_count = esp_cpu_get_cycle_count();
    int cpu_freq = esp_rom_get_cpu_ticks_per_us() * 1000000;
    // printf("Core: %d\n", core);
    // printf("Cycle count: %d\n", cycle_count);
    printf("CPU freq: %d MHz\n", cpu_freq / 1000000);
    printf("100 ticks = %ld ms\n", pdTICKS_TO_MS(100));

    for (int i = 0; i < 20; i++) {
        // uint32_t start = xthal_get_ccount();
        uint32_t start = esp_cpu_get_cycle_count();
        // Read the ADC
        int adc = gpio_get_level(ADC);

        // Write the ADC to the DAC
        gpio_set_level(DAC, adc);
        // uint32_t end = xthal_get_ccount();
        uint32_t end = esp_cpu_get_cycle_count();
        uint32_t elapsed_cycles = end - start;
        uint32_t elapsed_us = elapsed_cycles * 1000000 / cpu_freq;
        printf("Elapsed time: %ld us\n", elapsed_us);
        printf("Elapsed cycles: %ld ticks\n", elapsed_cycles);
        printf("Elapsed cycles: %ld ms\n", pdTICKS_TO_MS(elapsed_cycles));
        // uint32_t elapsed_ns = elapsed_cycles * 1000000000 / cpu_freq;
        // printf("Elapsed time: %ld us (%ld ns)\n", elapsed_us, elapsed_ns);
    }
}