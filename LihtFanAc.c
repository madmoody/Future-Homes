#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#define GPIO_PIN "/sys/class/gpio/gpio26/value"  // Replace "26" with the actual GPIO pin number

int main() {
    int fd;

    // Export the GPIO pin
    int export_fd = open("/sys/class/gpio/export", O_WRONLY);
    if (export_fd < 0) {
        perror("Error exporting GPIO pin");
        return 1;
    }
    write(export_fd, "26", 2);
    close(export_fd);

    // Set the GPIO pin direction as output
    int direction_fd = open("/sys/class/gpio/gpio26/direction", O_WRONLY);
    if (direction_fd < 0) {
        perror("Error setting GPIO direction");
        return 1;
    }
    write(direction_fd, "out", 3);
    close(direction_fd);

    // Turn ON the lights
    fd = open(GPIO_PIN, O_WRONLY);
    if (fd < 0) {
        perror("Error opening GPIO pin");
        return 1;
    }
    write(fd, "1", 1);
    close(fd);

    // Add a delay to keep the lights ON (adjust the delay as needed)
    sleep(5); // 5 seconds delay

    // Turn OFF the lights
    fd = open(GPIO_PIN, O_WRONLY);
    if (fd < 0) {
        perror("Error opening GPIO pin");
        return 1;
    }
    write(fd, "0", 1);
    close(fd);

    return 0;
}
