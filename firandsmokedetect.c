#include <stdio.h>
#include <stdint.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/ioctl.h>
#include <linux/spi/spidev.h>

#define SPI_DEVICE "/dev/spidev1.0"  // The SPI device file for the MCP3008
#define SPI_SPEED 1000000  // SPI clock speed in Hz
#define ADC_CHANNEL 0  // The ADC channel connected to the MQ2 sensor

// Function to read the ADC value from the MCP3008
int read_adc_value(int channel) {
    int fd, ret;
    uint8_t tx_buf[3], rx_buf[3];

    fd = open(SPI_DEVICE, O_RDWR);
    if (fd < 0) {
        perror("Error opening SPI device");
        return -1;
    }

    // Set SPI mode and bits per word
    uint8_t mode = SPI_MODE_0;
    uint8_t bits = 8;
    ret = ioctl(fd, SPI_IOC_WR_MODE, &mode);
    ret |= ioctl(fd, SPI_IOC_RD_MODE, &mode);
    ret |= ioctl(fd, SPI_IOC_WR_BITS_PER_WORD, &bits);
    ret |= ioctl(fd, SPI_IOC_RD_BITS_PER_WORD, &bits);
    if (ret < 0) {
        perror("Error setting SPI mode or bits per word");
        close(fd);
        return -1;
    }

    // Configure SPI speed
    uint32_t speed = SPI_SPEED;
    ret = ioctl(fd, SPI_IOC_WR_MAX_SPEED_HZ, &speed);
    ret |= ioctl(fd, SPI_IOC_RD_MAX_SPEED_HZ, &speed);
    if (ret < 0) {
        perror("Error setting SPI speed");
        close(fd);
        return -1;
    }

    // MCP3008 command to read the specified ADC channel
    tx_buf[0] = 0x01;  // Start bit
    tx_buf[1] = 0x80 | (channel << 4);  // Single-ended, channel number
    tx_buf[2] = 0x00;  // Don't care bits

    struct spi_ioc_transfer tr = {
        .tx_buf = (unsigned long)tx_buf,
        .rx_buf = (unsigned long)rx_buf,
        .len = 3,
        .delay_usecs = 0,
        .speed_hz = SPI_SPEED,
        .bits_per_word = 8,
    };

    // Perform SPI transfer
    ret = ioctl(fd, SPI_IOC_MESSAGE(1), &tr);
    if (ret < 0) {
        perror("Error reading ADC value");
        close(fd);
        return -1;
    }

    close(fd);

    // Extract the ADC value from the received data
    int adc_value = ((rx_buf[1] & 0x03) << 8) | rx_buf[2];

    return adc_value;
}

int main() {
    int adc_value;

    while (1) {
        adc_value = read_adc_value(ADC_CHANNEL);
        if (adc_value < 0) {
            printf("Error reading ADC value\n");
            return 1;
        }

        // Convert the ADC value to voltage (assuming 10-bit ADC, 3.3V reference voltage)
        double voltage = (adc_value * 3.3) / 1023.0;

        // Implement your smoke/gas concentration calculation based on sensor calibration data
        // This will vary depending on the sensor and the specific gases you want to detect

        printf("ADC Value: %d, Voltage: %.2f V\n", adc_value, voltage);

        // Add a delay (adjust as needed) before taking the next reading
        usleep(500000); // 500 ms delay
    }

    return 0;
}
