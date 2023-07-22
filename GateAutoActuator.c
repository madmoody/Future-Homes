#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

#define GPIO_PIN "/sys/class/gpio/gpio26/value"  // Replace "26" with the actual GPIO pin number

// Function to activate the actuator
void activate_actuator() {
    int fd = open(GPIO_PIN, O_WRONLY);
    if (fd < 0) {
        perror("Error opening GPIO pin");
        return;
    }

    // Turn ON the GPIO pin to activate the actuator
    write(fd, "1", 1);

    // Add a delay (adjust as needed) to keep the actuator activated
    usleep(2000000); // 2 seconds delay

    // Turn OFF the GPIO pin to deactivate the actuator
    write(fd, "0", 1);

    close(fd);
}

int main() {
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

    // Activate the actuator
    activate_actuator();

    return 0;
}
