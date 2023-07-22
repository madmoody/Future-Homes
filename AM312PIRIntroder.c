#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#define GPIO_PIN "/sys/class/gpio/gpio26/value"  // Replace "26" with the actual GPIO pin number

// Function to read the PIR sensor status
int read_pir_sensor_status() {
    int fd, value;
    char buffer[2];

    // Open the GPIO pin file
    fd = open(GPIO_PIN, O_RDONLY);
    if (fd < 0) {
        perror("Error opening GPIO pin");
        return -1;
    }

    // Read the GPIO pin value
    if (read(fd, buffer, sizeof(buffer)) < 0) {
        perror("Error reading GPIO value");
        close(fd);
        return -1;
    }

    close(fd);

    // Convert the read value to an integer (0 or 1)
    value = atoi(buffer);

    return value;
}

int main() {
    int pir_status;

    // Export the GPIO pin
    int export_fd = open("/sys/class/gpio/export", O_WRONLY);
    if (export_fd < 0) {
        perror("Error exporting GPIO pin");
        return 1;
    }
    write(export_fd, "26", 2);
    close(export_fd);

    // Set the GPIO pin direction as input
    int direction_fd = open("/sys/class/gpio/gpio26/direction", O_WRONLY);
    if (direction_fd < 0) {
        perror("Error setting GPIO direction");
        return 1;
    }
    write(direction_fd, "in", 2);
    close(direction_fd);

    // Read the PIR sensor status in a loop
    while (1) {
        pir_status = read_pir_sensor_status();
        if (pir_status < 0) {
            printf("Error reading PIR sensor status\n");
            return 1;
        }

        // The pir_status will be 1 when motion is detected and 0 when no motion is detected
        if (pir_status == 1) {
            printf("Motion detected!\n");
            // Implement your response actions here, e.g., activate an alarm, notify authorities, etc.
        } else {
            printf("No motion detected.\n");
        }

        // Add a delay (adjust as needed) before checking the PIR sensor status again
        usleep(500000); // 500 ms delay
    }

    return 0;
}
