#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"

#define ADC 33
#define DAC 25

void app_main() {
    esp_rom_gpio_pad_select_gpio(ADC);
    gpio_set_direction(ADC, GPIO_MODE_INPUT);
    esp_rom_gpio_pad_select_gpio(DAC);
    gpio_set_direction(DAC, GPIO_MODE_OUTPUT);

    for (int i = 0; i < 20; i++) {
        uint32_t start = xthal_get_ccount();
        // Read the ADC
        int adc = gpio_get_level(ADC);

        // Write the ADC to the DAC
        gpio_set_level(DAC, adc);
        uint32_t end = xthal_get_ccount();
        printf("Time: %ld\n", end - start);
    }
}