#include <fstream>
#include <syslog.h>

#include "gpio.hpp"

using namespace std;

namespace RPiFanController {

const char *gpio_export_path = "/sys/class/gpio/export";
const char *gpio_direction_path = "/sys/class/gpio/gpio%d/direction";
const char *gpio_value_path = "/sys/class/gpio/gpio%d/value";

GPIO::GPIO(int pin) {
    this->pin = pin;
}

bool enable(int pin) {
    try {
        ofstream node(gpio_export_path);

        if (node) {
            node << "out";
        } else {
            syslog(LOG_CRIT, "Error writing to %s", gpio_export_path);
            return false;
        }

        node.close();

        return true;
    } catch (...) {
        syslog(LOG_CRIT, "Unhandled error on enabling pin!");
        return false;
    }
}

bool set_output(int pin) {
    try {
        char *path = new char[35];

        sprintf(path, gpio_direction_path, pin);

        ofstream node(path);

        if (node) {
            node << "out";
        } else {
            syslog(LOG_CRIT, "Error writing to %s", gpio_export_path);
            return false;
        }

        node.close();

        return true;
    } catch (...) {
        syslog(LOG_CRIT, "Unhandled error on setting pin to output mode!");
        return false;
    }
}

bool write(int pin, int value) {
    try {
        char *path = new char[35];

        sprintf(path, gpio_value_path, pin);

        ofstream node(path);

        if (node) {
            node << value;
        } else {
            syslog(LOG_CRIT, "Error writing to %s", gpio_export_path);
            return false;
        }

        node.close();

        return true;
    } catch (...) {
        syslog(LOG_CRIT, "Unhandled error on setting pin to output mode!");
        return false;
    }
}

bool GPIO::init() {
    return enable(pin) && set_output(pin);
}

bool GPIO::high() {
    return write(pin, 1);
}

bool GPIO::low() {
    return write(pin, 0);
}

} // namespace RPiFanController