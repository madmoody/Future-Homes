#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>

#define ADC_PATH "/sys/bus/iio/devices/iio:device0/in_voltage%d_raw"
#define ADC_CHANNEL 0

// Function to read the ADC value from the specified channel
int read_adc_value(int channel) {
    int fd, len;
    char buf[64];
    int value;

    snprintf(buf, sizeof(buf), ADC_PATH, channel);

    fd = open(buf, O_RDONLY);
    if (fd < 0) {
        perror("Error opening ADC channel");
        return -1;
    }

    len = read(fd, buf, sizeof(buf));
    if (len < 0) {
        perror("Error reading ADC value");
        close(fd);
        return -1;
    }

    value = atoi(buf);
    close(fd);

    return value;
}

int main() {
    int adc_value;

    while (1) {
        adc_value = read_adc_value(ADC_CHANNEL);
        if (adc_value < 0) {
            printf("Error reading ADC value\n");
            return 1;
        }

        // Convert the ADC value to voltage (assuming 12-bit ADC, 1.8V reference voltage)
        double voltage = (adc_value * 1.8) / 4095.0;

        // Assuming MQ9 sensor sensitivity curve to CO gas concentration (adjust accordingly)
        double co_gas_concentration = voltage * 100; // Replace with appropriate calibration

        printf("ADC Value: %d, Voltage: %.2f V, CO Gas Concentration: %.2f ppm\n", adc_value, voltage, co_gas_concentration);

        // Add a delay (adjust as needed) before taking the next reading
        usleep(500000); // 500 ms delay
    }

    return 0;
}
